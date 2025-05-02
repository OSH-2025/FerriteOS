/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Spi Flash Fs Implementation
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errno.h"
#include "fcntl.h"
#include "sys/stat.h"
#include "los_vfs.h"
#include "los_spiffs.h"
#include "los_printf.h"
#include "los_base.h"

#include "spiffs.h"
#include "spiffs_nucleus.h"

static int RetToErrno(int ret)
{
    int err = 0;

    switch (ret) {
        case SPIFFS_OK:
            return 0;

        case SPIFFS_ERR_DATA_SPAN_MISMATCH:
        case SPIFFS_ERR_IS_INDEX:
        case SPIFFS_ERR_INDEX_REF_FREE:
        case SPIFFS_ERR_INDEX_REF_LU:
        case SPIFFS_ERR_INDEX_REF_INVALID:
        case SPIFFS_ERR_INDEX_FREE:
        case SPIFFS_ERR_INDEX_LU:
        case SPIFFS_ERR_INDEX_INVALID:
        case SPIFFS_ERR_CONFLICTING_NAME:
        case SPIFFS_ERR_INVALID_PARA:
            err = EINVAL;
            break;

        case SPIFFS_ERR_NOT_WRITABLE:
        case SPIFFS_ERR_NOT_READABLE:
        case SPIFFS_ERR_NOT_CONFIGURED:
            err = EACCES;
            break;

        case SPIFFS_ERR_NOT_MOUNTED:
        case SPIFFS_ERR_NOT_A_FS:
        case SPIFFS_ERR_PROBE_NOT_A_FS:
        case SPIFFS_ERR_MAGIC_NOT_POSSIBLE:
            err = ENODEV;
            break;

        case SPIFFS_ERR_FULL:
        case SPIFFS_ERR_PROBE_TOO_FEW_BLOCKS:
            err = ENOSPC;
            break;

        case SPIFFS_ERR_BAD_DESCRIPTOR:
        case SPIFFS_ERR_OUT_OF_FILE_DESCS:
            err = EBADF;
            break;

        case SPIFFS_ERR_MOUNTED:
        case SPIFFS_ERR_FILE_EXISTS:
            err = EEXIST;
            break;

        case SPIFFS_ERR_NOT_FOUND:
        case SPIFFS_ERR_NOT_A_FILE:
        case SPIFFS_ERR_DELETED:
        case SPIFFS_ERR_FILE_DELETED:
        case SPIFFS_ERR_NOT_FINALIZED:
        case SPIFFS_ERR_NOT_INDEX:
        case SPIFFS_ERR_IS_FREE:
        case SPIFFS_ERR_INDEX_SPAN_MISMATCH:
        case SPIFFS_ERR_FILE_CLOSED:
            err = ENOENT;
            break;

        case SPIFFS_ERR_NAME_TOO_LONG:
            err = ENAMETOOLONG;
            break;

        case SPIFFS_ERR_RO_NOT_IMPL:
        case SPIFFS_ERR_RO_ABORTED_OPERATION:
            err = EROFS;
            break;

        case SPIFFS_ERR_SEEK_BOUNDS:
            err = EOVERFLOW;
            break;

        case SPIFFS_ERR_END_OF_OBJECT:
        case SPIFFS_ERR_NO_DELETED_BLOCKS:
            err = ENODATA;
            break;

        case SPIFFS_ERR_ERASE_FAIL:
            err = EIO;
            break;

        default:
            err = EIO;
            break;
    }

    VFS_ERRNO_SET(err);
    return -err;
}

static int SpiffsFlagsGet(int oflags)
{
    int flags = 0;

    switch (oflags & O_ACCMODE) {
        case O_RDONLY:
            flags |= SPIFFS_O_RDONLY;
            break;
        case O_WRONLY:
            flags |= SPIFFS_O_WRONLY;
            break;
        case O_RDWR:
            flags |= SPIFFS_O_RDWR;
            break;
        default:
            break;
    }

    if (oflags & O_CREAT) {
        flags |= SPIFFS_O_CREAT;
    }

    if (oflags & O_EXCL) {
        flags |= SPIFFS_O_EXCL;
    }

    if (oflags & O_TRUNC) {
        flags |= SPIFFS_O_TRUNC;
    }

    if (oflags & O_APPEND) {
        flags |= SPIFFS_O_CREAT | SPIFFS_O_APPEND;
    }

    return flags;
}

static int SpiffsOperationOpen(struct file *file, const char *pathInMp, int flags)
{
    spiffs *fs = (spiffs *)file->f_mp->m_data;
    spiffs_file s_file;

    s_file = SPIFFS_open(fs, pathInMp, SpiffsFlagsGet(flags), 0);

    if (s_file < SPIFFS_OK) {
        return RetToErrno(s_file);
    }

    file->f_data = (void *)(uintptr_t)s_file;

    return 0;
}

static spiffs_file SpifdFromFile(struct file *file)
{
    return (spiffs_file)(uintptr_t)file->f_data;
}

static int SpiffsOperationClose(struct file *file)
{
    spiffs_file s_file = SpifdFromFile(file);
    spiffs *fs = (spiffs *)file->f_mp->m_data;
    s32_t res = SPIFFS_close(fs, s_file);

    return RetToErrno(res);
}

static ssize_t SpiffsOperationRead(struct file *file, char *buff, size_t bytes)
{
    if ((buff == NULL) || (bytes == 0)) {
        return -EINVAL;
    }

    spiffs_file s_file = SpifdFromFile(file);
    spiffs *fs = (spiffs *)file->f_mp->m_data;
    s32_t res = SPIFFS_read(fs, s_file, buff, bytes);

    return res < 0 ? RetToErrno(res) : res;
}

static ssize_t SpiffsOperationWrite(struct file *file, const char *buff, size_t bytes)
{
    if ((buff == NULL) || (bytes == 0)) {
        return -EINVAL;
    }

    spiffs_file s_file = SpifdFromFile(file);
    spiffs *fs = (spiffs *)file->f_mp->m_data;
    s32_t res = SPIFFS_write(fs, s_file, (void *)buff, bytes);

    return res < 0 ? RetToErrno(res) : res;
}

static off_t SpiffsOperationLseek(struct file *file, off_t off, int whence)
{
    spiffs_file s_file = SpifdFromFile(file);
    spiffs *fs = (spiffs *)file->f_mp->m_data;
    s32_t res = SPIFFS_lseek(fs, s_file, off, whence);

    return res < 0 ? RetToErrno(res) : res;
}

static off64_t SpiffsOperationLseek64(struct file *filep, off64_t offset, int whence)
{
    return (off64_t)SpiffsOperationLseek(filep, (off_t)offset, whence);
}

int SpiffsOperationStat(struct mount_point *mp, const char *pathInMp, struct stat *stat)
{
    spiffs_stat s = { 0 };

    (void)memset_s(stat, sizeof(struct stat), 0, sizeof(struct stat));
    s32_t res = SPIFFS_stat((spiffs *)mp->m_data, pathInMp, &s);
    if (res == SPIFFS_OK) {
        stat->st_size = s.size;
        if (s.type == SPIFFS_TYPE_DIR) {
            stat->st_mode = S_IFDIR;
        } else {
            stat->st_mode = S_IFREG;
        }
    }

    return RetToErrno(res);
}

static int SpiffsOperationUlink(struct mount_point *mp, const char *pathInMp)
{
    s32_t res = SPIFFS_remove((spiffs *)mp->m_data, pathInMp);
    return RetToErrno(res);
}

static int SpiffsOperationRename(struct mount_point *mp, const char *pathInMpOld, const char *pathInMpNew)
{
    s32_t res = SPIFFS_rename((spiffs *)mp->m_data, pathInMpOld, pathInMpNew);
    return RetToErrno(res);
}

static int SpiffsOperationSync(struct file *file)
{
    spiffs_file s_file = SpifdFromFile(file);
    spiffs *fs = (spiffs *)file->f_mp->m_data;
    s32_t res = SPIFFS_fflush(fs, s_file);

    return res < 0 ? RetToErrno(res) : res;
}

static int SpiffsOperationOpendir(struct dir *dir, const char *path)
{
    spiffs *fs = (spiffs *)dir->d_mp->m_data;
    spiffs_DIR *sdir;

    sdir = (spiffs_DIR *)malloc(sizeof(spiffs_DIR));

    if (sdir == NULL) {
        PRINT_ERR("Fail to malloc memory in spiffs\n");
        return -ENOMEM;
    }

    dir->d_data = (void *)SPIFFS_opendir(fs, path, sdir);
    dir->d_offset = 0;

    if (dir->d_data == 0) {
        free(sdir);
        return -ENOENT;
    }

    return LOS_OK;
}

int SpiffsOperationReaddir(struct dir *dir, struct dirent *dent)
{
    struct spiffs_dirent e;

    if (SPIFFS_readdir((spiffs_DIR *)dir->d_data, &e) == NULL) {
        return -ENOENT;
    }

    strncpy_s(dent->name, LOS_MAX_DIR_NAME_LEN - 1, (const char *)e.name, LOS_MAX_FILE_NAME_LEN - 1);
    dent->name[LOS_MAX_FILE_NAME_LEN - 1] = '\0';
    dent->size = e.size;

    if (e.type == SPIFFS_TYPE_DIR) {
        dent->type = VFS_TYPE_DIR;
    } else {
        dent->type = VFS_TYPE_FILE;
    }

    return LOS_OK;
}

static int SpiffsOperationClosedir(struct dir *dir)
{
    spiffs_DIR *sdir = (spiffs_DIR *)dir->d_data;

    s32_t res = SPIFFS_closedir(sdir);

    free(sdir);

    return RetToErrno(res);
}

static struct file_ops g_spiffsOps = {
    SpiffsOperationOpen,
    SpiffsOperationClose,
    SpiffsOperationRead,
    SpiffsOperationWrite,
    SpiffsOperationLseek,
    SpiffsOperationLseek64,
    SpiffsOperationStat,
    SpiffsOperationUlink,
    SpiffsOperationRename,
    NULL, /* ioctl not supported for now */
    SpiffsOperationSync,
    SpiffsOperationOpendir,
    SpiffsOperationReaddir,
    SpiffsOperationClosedir,
    NULL /* spiffs do not support mkdir */
};

static struct file_system g_spiffsFs = { "spiffs", &g_spiffsOps, NULL, 0 };

static spiffs *g_fsPtr = NULL;
static u8_t *g_wbufPtr = NULL;
static u8_t *g_fdsPtr = NULL;
static u8_t *g_cachePtr = NULL;

int SpiffsMount(const char *path, struct spiffs_drv_t *spiffsDrv)
{
    spiffs *fs;
    spiffs_config c;
    u8_t *wbuf;
    u8_t *fds;
    u8_t *cache;
    int ret = -1;

#define LOS_SPIFFS_FD_SIZE (sizeof(spiffs_fd) * 8)
#define LOS_SPIFFS_CACHE_SIZE (((spiffsDrv->logPageSize + 32) * 4) + 40)

    fs = (spiffs *)malloc(sizeof(spiffs));
    wbuf = (u8_t *)malloc(spiffsDrv->logPageSize * 2);
    fds = (u8_t *)malloc(LOS_SPIFFS_FD_SIZE);
    cache = (u8_t *)malloc(LOS_SPIFFS_CACHE_SIZE);

    if ((fs == NULL) || (wbuf == NULL) || (fds == NULL) || (cache == NULL)) {
        PRINT_ERR("Fail to malloc memory in spiffs.\n");
        goto err_free;
    }

    memset_s(fs, sizeof(spiffs), 0, sizeof(spiffs));

    c.hal_read_f = spiffsDrv->SpiRead;
    c.hal_write_f = spiffsDrv->SpiWrite;
    c.hal_erase_f = spiffsDrv->SpiErase;
    c.log_block_size = spiffsDrv->logBlockSize;
    c.log_page_size = spiffsDrv->logPageSize;
    c.phys_addr = spiffsDrv->physAddr;
    c.phys_erase_block = spiffsDrv->phyEraseBlock;
    c.phys_size = spiffsDrv->physSize;
    c.fh_ix_offset = TEST_SPIFFS_FILEHDL_OFFSET;

    ret = SPIFFS_mount(fs, &c, wbuf, fds, LOS_SPIFFS_FD_SIZE, cache, LOS_SPIFFS_CACHE_SIZE, NULL);

    if (ret == SPIFFS_ERR_NOT_A_FS) {
        PRINT_INFO("Formating fs...\n");
        SPIFFS_format(fs);
        ret = SPIFFS_mount(fs, &c, wbuf, fds, LOS_SPIFFS_FD_SIZE, cache, LOS_SPIFFS_CACHE_SIZE, NULL);
    }

    if (ret != SPIFFS_OK) {
        PRINT_ERR("Format fail.\n");
        goto err_unmount;
    }

    ret = LOS_FsMount("spiffs", path, fs);

    if (ret == LOS_OK) {
        g_fsPtr = fs;
        g_wbufPtr = wbuf;
        g_fdsPtr = fds;
        g_cachePtr = cache;
        PRINT_INFO("Spiffs mount at %s done!\n", path);
        return LOS_OK;
    }

    PRINT_ERR("Failed to mount!\n");

err_unmount:
    SPIFFS_unmount(fs);
err_free:
    if (fs != NULL) {
        free(fs);
    }
    if (wbuf != NULL) {
        free(wbuf);
    }
    if (fds != NULL) {
        free(fds);
    }
    if (cache != NULL) {
        free(cache);
    }

    return ret;
}

int SpiffsUnmount(const char *path)
{
    if (g_fsPtr) {
        SPIFFS_unmount(g_fsPtr);
        free(g_fsPtr);
        g_fsPtr = NULL;
    }
    if (g_wbufPtr) {
        free(g_wbufPtr);
        g_wbufPtr = NULL;
    }
    if (g_fdsPtr) {
        free(g_fdsPtr);
        g_fdsPtr = NULL;
    }
    if (g_cachePtr) {
        free(g_cachePtr);
        g_cachePtr = NULL;
    }

    LOS_FsUnmount(path);

    return 0;
}

int SpiffsInit(int needErase, struct spiffs_drv_t *spiffsDriver)
{
    static int spiffsInited = FALSE;
    int ret;

    if (spiffsInited) {
        return LOS_OK;
    }

    if (LOS_VfsInit() != LOS_OK) {
        return LOS_NOK;
    }

    if (LOS_FsRegister(&g_spiffsFs) != LOS_OK) {
        PRINT_ERR("Failed to register fs.\n");
        return LOS_NOK;
    }

    spiffsDriver->SpiDriverInit(needErase);

    ret = SpiffsMount("/spiffs/", spiffsDriver);

    if (ret == LOS_OK) {
        spiffsInited = TRUE;

        PRINT_INFO("Register spiffs done.\n");

        return LOS_OK;
    }

    return ret;
}
