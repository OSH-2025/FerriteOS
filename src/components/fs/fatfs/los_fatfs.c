/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Fat Fs Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

/* Includes ----------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "los_vfs.h"
#include "los_fatfs.h"
#include "los_printf.h"
#include "los_base.h"

/* Defines ------------------------------------------------------------------ */
/* Typedefs ----------------------------------------------------------------- */

/* Macros ------------------------------------------------------------------- */
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef POINTER_ASSERT
#define POINTER_ASSERT(p) \
    if (p == NULL) {      \
        return -EINVAL;   \
    }
#endif
/* Local variables ---------------------------------------------------------- */
struct disk_mnt disk;

/* Extern variables --------------------------------------------------------- */
/* Global variables --------------------------------------------------------- */
/* Private function prototypes ---------------------------------------------- */
/* Public functions --------------------------------------------------------- */

static int RetToErrno(FRESULT result)
{
    int err = 0;

    switch (result) {
        case FR_OK:
            return 0;

        case FR_NO_PATH:
            err = ENOTDIR;
            break;

        case FR_NO_FILE:
            err = ENOENT;
            break;

        case FR_NO_FILESYSTEM:
            err = ENODEV;
            break;

        case FR_TOO_MANY_OPEN_FILES:
            err = ENFILE;
            break;

        case FR_INVALID_NAME:
            err = ENAMETOOLONG;
            break;

        case FR_INVALID_PARAMETER:
        case FR_INVALID_OBJECT:
        case FR_INT_ERR:
            err = EINVAL;
            break;

        case FR_INVALID_DRIVE:
        case FR_NOT_ENABLED:
            err = ENXIO;
            break;

        case FR_EXIST:
            err = EEXIST;
            break;

        case FR_DISK_ERR:
        case FR_NOT_READY:
            err = EIO;
            break;

        case FR_WRITE_PROTECTED:
            err = EROFS;
            break;

        case FR_LOCKED:
            err = EBUSY;
            break;

        case FR_DENIED:
            err = EISDIR;
            break;

        case FR_MKFS_ABORTED:
            err = EBUSY;
            break;

        case FR_NOT_ENOUGH_CORE:
            err = ENOMEM;
            break;

        case FR_TIMEOUT:
            err = ETIMEDOUT;
            break;

        default:
            err = EIO;
            break;
    }

    VFS_ERRNO_SET(err);
    return -err;
}

/**
 * @brief  Links a compatible diskio driver/lun id and increments the number of active
 *         linked drivers.
 * @note   The number of linked drivers (volumes) is up to 10 due to FatFs limits.
 * @param  drv: pointer to the disk IO Driver structure
 * @param  lun : only used for USB Key Disk to add multi-lun management
            else the parameter must be equal to 0
 * @retval Returns -1 in case of failure, otherwise return the drive (0 to volumes).
 */
static int FatfsLinkDriver(const struct diskio_drv *drv, uint8_t lun)
{
    int ret = -1;
    int i;

    if (disk.num >= FF_VOLUMES) {
        return ret;
    }

    for (i = 0; i < FF_VOLUMES; i++) {
        if (disk.dev[i].drv != 0) {
            continue;
        }

        disk.dev[disk.num].state = 0;
        disk.dev[disk.num].drv = drv;
        disk.dev[disk.num].lun = lun;
        disk.num++;
        return i;
    }
    return ret;
}

/**
 * @brief  Unlinks a diskio driver and decrements the number of active linked
 * drivers.
 * @param  drive: the disk drive (0 to volumes)
 * @param  lun : not used
 * @retval Returns -1 in case of failure, otherwise return the drive (0 to volumes).
 */
static int FatfsUnlinkDriver(uint8_t drive, uint8_t lun)
{
    int ret = -1;

    if (disk.num >= 1 && drive < FF_VOLUMES) {
        if (disk.dev[drive].drv != 0) {
            disk.dev[drive].state = 0;
            disk.dev[drive].drv = 0;
            disk.dev[drive].lun = 0;
            disk.num--;
            return drive;
        }
    }

    return ret;
}

int FatfsRegister(const struct diskio_drv *drv)
{
    return FatfsLinkDriver(drv, 0);
}

int FatfsUnregister(uint8_t drive)
{
    return FatfsUnlinkDriver(drive, 0);
}

static int FatfsFlagsGet(int oflags)
{
    int flags = 0;

    switch (oflags & O_ACCMODE) {
        case O_RDONLY:
            flags |= FA_READ;
            break;
        case O_WRONLY:
            flags |= FA_WRITE;
            break;
        case O_RDWR:
            flags |= FA_READ | FA_WRITE;
            break;
        default:
            break;
    }

    if (oflags & O_CREAT) {
        flags |= FA_OPEN_ALWAYS;
    }

    if ((oflags & O_CREAT) && (oflags & O_EXCL)) {
        flags |= FA_CREATE_NEW;
    }

    if (oflags & O_TRUNC) {
        flags |= FA_CREATE_ALWAYS;
    }

    if (oflags & O_APPEND) {
        flags |= FA_READ | FA_WRITE | FA_OPEN_APPEND;
    }

    return flags;
}

static int FatfsOperationOpen(struct file *file, const char *pathInMp, int flags)
{
    FRESULT res;
    FIL *fp;
    FILINFO info = { 0 };

    fp = (FIL *)malloc(sizeof(FIL));
    if (fp == NULL) {
        PRINT_ERR("Failed to malloc memory in fatfs.\n");
        return -EINVAL;
    }

    if (!(flags & O_CREAT) && (flags & O_TRUNC)) {
        res = f_stat(pathInMp, &info);
        if (res != FR_OK) {
            free(fp);
            return res;
        }
    }

    res = f_open(fp, pathInMp, FatfsFlagsGet(flags));
    if (res == FR_OK) {
        file->f_data = (void *)fp;
    } else {
        free(fp);
    }
    if (res == FR_LOCKED) {
        int err = 0;
        VFS_ERRNO_SET(EACCES);
        err = EACCES;
        return -err;
    } else {
        return RetToErrno(res);
    }
}

static int FatfsOperationClose(struct file *file)
{
    FRESULT res;
    FIL *fp = (FIL *)file->f_data;

    POINTER_ASSERT(fp);

    res = f_close(fp);
    if (res == FR_OK) {
        free(fp);
        file->f_data = NULL;
    }

    return RetToErrno(res);
}

static ssize_t FatfsOperationRead(struct file *file, char *buff, size_t bytes)
{
    ssize_t size = 0;
    FRESULT res;
    FIL *fp = (FIL *)file->f_data;

    if ((buff == NULL) || (bytes == 0)) {
        return -EINVAL;
    }

    POINTER_ASSERT(fp);
    res = f_read(fp, buff, bytes, (UINT *)&size);
    if (res != FR_OK) {
        PRINT_ERR("Failed to read, res=%d\n", res);
        return RetToErrno(res);
    }
    return size;
}

static ssize_t FatfsOperationWrite(struct file *file, const char *buff, size_t bytes)
{
    ssize_t size = 0;
    FRESULT res;
    FIL *fp = (FIL *)file->f_data;

    if ((buff == NULL) || (bytes == 0)) {
        return -EINVAL;
    }

    POINTER_ASSERT(fp);
    res = f_write(fp, buff, bytes, (UINT *)&size);
    if ((res != FR_OK) || (size == 0)) {
        PRINT_ERR("Failed to write, res=%d\n", res);
        return RetToErrno(res);
    }
    return size;
}

static off_t FatfsOperationLseek(struct file *file, off_t off, int whence)
{
    FIL *fp = (FIL *)file->f_data;

    POINTER_ASSERT(fp);
    switch (whence) {
        case 0: // SEEK_SET
            break;
        case 1: // SEEK_CUR
            off += f_tell(fp);
            break;
        case 2: // SEEK_END
            off += f_size(fp);
            break;
        default:
            RetToErrno(FR_INVALID_PARAMETER);
            return -1;
    }

    if (off < 0) {
        return RetToErrno(FR_INVALID_PARAMETER);
    }

    FRESULT res = f_lseek(fp, off);
    if (res == FR_OK) {
        return off;
    } else
        return RetToErrno(res);
}

static off64_t FatfsOperationLseek64(struct file *file, off64_t off, int whence)
{
    return (off64_t)FatfsOperationLseek(file, (off_t)off, whence);
}

int FatfsOperationStat(struct mount_point *mp, const char *pathInMp, struct stat *stat)
{
    FRESULT res;
    FILINFO info = { 0 };

    (void)memset_s(stat, sizeof(struct stat), 0, sizeof(struct stat));
    res = f_stat(pathInMp, &info);
    if (res == FR_OK) {
        stat->st_size = info.fsize;
        if (info.fattrib & AM_DIR) {
            stat->st_mode = S_IFDIR;
        } else {
            stat->st_mode = S_IFREG;
        }
    }

    return RetToErrno(res);
}

static int FatfsOperationUnlink(struct mount_point *mp, const char *pathInMp)
{
    FRESULT res = f_unlink(pathInMp);
    if (res == FR_NO_PATH) {
        int err = 0;
        VFS_ERRNO_SET(ENOENT);
        err = ENOENT;
        return -err;
    } else {
        return RetToErrno(res);
    }
}

static int FatfsOperationRename(struct mount_point *mp, const char *pathInMpOld, const char *pathInMpNew)
{
    FRESULT res = f_rename(pathInMpOld, pathInMpNew);
    return RetToErrno(res);
}

static int FatfsOperationSync(struct file *file)
{
    FIL *fp = (FIL *)file->f_data;
    FRESULT res;

    POINTER_ASSERT(fp);

    res = f_sync(fp);
    return RetToErrno(res);
}

static int FatfsOperationOpendir(struct dir *dir, const char *path)
{
    FRESULT res;
    DIR *dp;

    dp = (DIR *)malloc(sizeof(DIR));
    if (dp == NULL) {
        PRINT_ERR("Fail to malloc memory in spiffs.\n");
        return -ENOMEM;
    }

    res = f_opendir(dp, path);
    if (res != FR_OK) {
        free(dp);
        return RetToErrno(res);
    }

    dir->d_data = dp;
    dir->d_offset = 0;

    return FR_OK;
}

static int FatfsOperationReaddir(struct dir *dir, struct dirent *dent)
{
    FRESULT res;
    DIR *dp = (DIR *)dir->d_data;
    FILINFO e;
    int len;

    POINTER_ASSERT(dp);

    res = f_readdir(dp, &e);
    if (res != FR_OK) {
        return RetToErrno(res);
    }

    len = MIN(sizeof(e.fname), LOS_MAX_DIR_NAME_LEN + 1) - 1;
    strncpy_s((char *)dent->name, LOS_MAX_DIR_NAME_LEN, (const char *)e.fname, len);
    dent->name[len] = '\0';
    dent->size = e.fsize;

    if (e.fattrib == AM_DIR) {
        dent->type = VFS_TYPE_DIR;
    } else {
        dent->type = VFS_TYPE_FILE;
    }

    return FR_OK;
}

static int FatfsOperationClosedir(struct dir *dir)
{
    FRESULT res;
    DIR *dp = (DIR *)dir->d_data;

    POINTER_ASSERT(dp);

    res = f_closedir(dp);
    if (res == FR_OK) {
        free(dp);
        dir->d_data = NULL;
    }

    return RetToErrno(res);
}

static int FatfsOperationMkdir(struct mount_point *mp, const char *path)
{
    FRESULT res = f_mkdir(path);
    if (res == FR_NO_PATH) {
        int err = 0;
        VFS_ERRNO_SET(ENOENT);
        err = ENOENT;
        return -err;
    } else {
        return RetToErrno(res);
    }
}

static struct file_ops g_fatfsOps = {
    FatfsOperationOpen,
    FatfsOperationClose,
    FatfsOperationRead,
    FatfsOperationWrite,
    FatfsOperationLseek,
    FatfsOperationLseek64,
    FatfsOperationStat,
    FatfsOperationUnlink,
    FatfsOperationRename,
    NULL,                   /* ioctl not supported for now */
    FatfsOperationSync,
    FatfsOperationOpendir,
    FatfsOperationReaddir,
    FatfsOperationClosedir,
    FatfsOperationMkdir
};

static struct file_system g_fatfsFs = { "fatfs", &g_fatfsOps, NULL, 0 };

static FATFS *g_fatfsPtr = NULL;

int FatfsMount(const char *path, struct diskio_drv *drv, uint8_t *drive)
{
    int sDriver;
    char dpath[4] = {0};
    int ret = -1;
    BYTE *workBuff = NULL;
    FRESULT res;
    FATFS *fs = NULL;

    sDriver = FatfsRegister(drv);
    if (sDriver < 0) {
        PRINT_ERR("Failed to register diskio.\n");
        return sDriver;
    }
    fs = (FATFS *)malloc(sizeof(FATFS));
    if (fs == NULL) {
        PRINT_ERR("Fail to malloc memory in fatfs.\n");
        goto err;
    }
    (void)memset_s(fs, sizeof(FATFS), 0, sizeof(FATFS));
    sprintf(dpath, "%d:/", sDriver);
    res = f_mount(fs, (const TCHAR *)dpath, 1);
    if (res == FR_NO_FILESYSTEM) {
        printf("SD no filesystem, format...\n");
        workBuff = (BYTE *)malloc(FF_MAX_SS);
        if (workBuff == NULL) {
            goto err_free;
        }
        (void)memset_s(workBuff, FF_MAX_SS, 0, FF_MAX_SS);
        res = f_mkfs((const TCHAR *)dpath, FM_ANY, 0, workBuff, FF_MAX_SS);
        if (res == FR_OK) {
            printf("SD format success.\n");
            res = f_mount(NULL, (const TCHAR *)dpath, 1);
            res = f_mount(fs, (const TCHAR *)dpath, 1);
        } else {
            printf("SD format fail.\n");
        }
        free(workBuff);
    } else if (res != FR_OK) {
        printf("SD mount fail (%d).\n", res);
        PRINT_ERR("Failed to mount fatfs, res=%d!\n", res);
        goto err_free;
    } else {
        printf("File system mount success.\n");
    }
    ret = LOS_FsMount("fatfs", path, fs);
    if (ret == LOS_OK) {
        PRINT_INFO("Fatfs mount at %s done.\n", path);
        *drive = sDriver;
        g_fatfsPtr = fs;
        return LOS_OK;
    }

    PRINT_ERR("Failed to mount.\n");

err_free:
    if (fs != NULL) {
        free(fs);
    }
err:
    FatfsUnregister(sDriver);
    return ret;
}

int FatfsUnmount(const char *path, uint8_t drive)
{
    char dpath[10] = {0};

    sprintf(dpath, "%d:/", drive);
    FatfsUnregister(drive);
    f_mount(NULL, (const TCHAR *)dpath, 1);
    LOS_FsUnmount(path);
    if (g_fatfsPtr) {
        free(g_fatfsPtr);
        g_fatfsPtr = NULL;
    }

    return 0;
}


int FatfsInit(int needErase, struct diskio_drv *drv, uint8_t *drive)
{
    int ret;
    static int fatfsInited = FALSE;

    if (fatfsInited) {
        return LOS_OK;
    }

    if (LOS_VfsInit() != LOS_OK) {
        return LOS_NOK;
    }

    if (LOS_FsRegister(&g_fatfsFs) != LOS_OK) {
        PRINT_ERR("Failed to register fs!\n");
        return LOS_NOK;
    }

    PRINT_INFO("Register fatfs done!\n");

    FatfsDriverInit(needErase);

    ret = FatfsMount("/fatfs/", drv, drive);
    if (ret == LOS_OK) {
        fatfsInited = TRUE;
        return LOS_OK;
    }

    return ret;
}

/* Private functions -------------------------------------------------------- */
