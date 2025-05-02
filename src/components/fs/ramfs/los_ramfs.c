/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: Ram Fs Implementation
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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __GNUC__
#include <errno.h>
#endif

#if defined(__GNUC__) || defined(__CC_ARM)
#include <fcntl.h>
#include <los_memory.h>
#endif

#include "los_vfs.h"
#include "los_ramfs.h"
#include "los_base.h"

#define RAMFS_TYPE_DIR VFS_TYPE_DIR
#define RAMFS_TYPE_FILE VFS_TYPE_FILE

struct ramfs_element {
    char name[LOS_MAX_FILE_NAME_LEN];
    uint32_t type;
    struct ramfs_element *sibling;
    struct ramfs_element *parent;
    volatile uint32_t refs;
    union {
        struct {
            size_t size;
            char *content;
        } f;
        struct {
            struct ramfs_element *child;
        } d;
    };
};

struct ramfs_mount_point {
    struct ramfs_element root;
    void *memory;
};

static struct ramfs_element *RamfsFileFind(struct mount_point *mp, const char *pathInMp, const char **pathUnresolved)
{
    struct ramfs_element *walk;

    /* walk every dir */
    walk = &((struct ramfs_mount_point *)mp->m_data)->root;

    while (1) {
        const char *c;
        struct ramfs_element *t;
        int l;

        if (walk->type != RAMFS_TYPE_DIR) {
            VFS_ERRNO_SET(ENOTDIR);

            return NULL;
        }

        while (*pathInMp == '/') {
            pathInMp++;
        }

        c = strchr(pathInMp, '/');
        if (c == NULL) {
            l = strlen(pathInMp);
        } else {
            l = c - pathInMp;
        }

        if (LOS_MAX_FILE_NAME_LEN <= l) {
            VFS_ERRNO_SET(ENAMETOOLONG);

            return NULL;
        }

        for (t = walk->d.child; t != NULL; t = t->sibling) {
            if ((strncmp(t->name, pathInMp, l) == 0) && (t->name[l] == '\0')) {
                break;
            }
        }

        if (t == NULL) {
            break; /* no match */
        }

        pathInMp += l;
        walk = t;

        if (c == NULL) {
            break;
        }
    }

    *pathUnresolved = pathInMp;

    return walk;
}

static int RamfsOpen(struct file *file, const char *pathInMp, int flags)
{
    struct ramfs_element *ramfsFile;
    struct ramfs_element *walk;
    int ret = -1;

    /* openning dir like "/romfs/ not support " */
    if (*pathInMp == '\0') {
        VFS_ERRNO_SET(EISDIR);
        return ret;
    }

    walk = RamfsFileFind(file->f_mp, pathInMp, &pathInMp);
    if (walk == NULL) {
        /* errno set by RamfsFileFind */
        return ret;
    }

    if ((walk->type == RAMFS_TYPE_DIR) && (*pathInMp == '\0')) {
        VFS_ERRNO_SET(EISDIR);
        return -1;
    }

    if (*pathInMp == '\0') { /* file already exist, we found it */
        ramfsFile = walk;

        if (ramfsFile->type != RAMFS_TYPE_FILE) {
            VFS_ERRNO_SET(EISDIR);
            return -1;
        }

        if ((flags & O_CREAT) && (flags & O_EXCL)) {
            VFS_ERRNO_SET(EEXIST);
            return -1;
        }

        if (flags & O_APPEND) {
            file->f_offset = ramfsFile->f.size;
        }

        ramfsFile->refs++;

        file->f_data = (void *)ramfsFile;

        return 0;
    }

    /*
     * file not found, ramfsFile holds the most dir matched, pathInMp holds
     * the left path not resolved
     */
    if ((flags & O_CREAT) == 0) {
        VFS_ERRNO_SET(ENOENT);
        return -1;
    }

    if (walk->type != RAMFS_TYPE_DIR) {
        /* if here, BUG! */
        VFS_ERRNO_SET(ENOTDIR);
        return -1;
    }

    if (strchr(pathInMp, '/') != NULL) {
        VFS_ERRNO_SET(ENOENT); /* parent dir not exist */
        return -1;
    }

    if (strlen(pathInMp) >= LOS_MAX_FILE_NAME_LEN) {
        VFS_ERRNO_SET(ENAMETOOLONG);
        return -1;
    }

    ramfsFile = malloc(sizeof(struct ramfs_element));
    if (ramfsFile == NULL) {
        PRINT_ERR("Fail to malloc memory in ramfs.\n");
        VFS_ERRNO_SET(ENOMEM);
        return -1;
    }

    strcpy(ramfsFile->name, pathInMp); /* length of pathInMp is already verified */

    ramfsFile->refs = 1;

    ramfsFile->type = RAMFS_TYPE_FILE;
    ramfsFile->sibling = walk->d.child;
    walk->d.child = ramfsFile;
    ramfsFile->f.content = NULL;
    ramfsFile->f.size = 0;
    ramfsFile->parent = walk;

    file->f_data = (void *)ramfsFile;

    return 0;
}

static int RamfsClose(struct file *file)
{
    struct ramfs_element *ramfsFile = (struct ramfs_element *)file->f_data;

    ramfsFile->refs--;

    return 0; /* not file delete, do not free the content */
}

static ssize_t RamfsRead(struct file *file, char *buff, size_t bytes)
{
    struct ramfs_element *ramfsFile = (struct ramfs_element *)file->f_data;

    if (file->f_offset < 0) {
        file->f_offset = 0;
    }

    if (ramfsFile->f.size <= (size_t)file->f_offset) { /* nothing to read */
        return 0;
    }

    if (ramfsFile->f.size - file->f_offset < bytes) {
        bytes = ramfsFile->f.size - file->f_offset;
    }

    memcpy(buff, ramfsFile->f.content + file->f_offset, bytes);

    file->f_offset += bytes;

    return bytes;
}

static ssize_t RamfsWrite(struct file *file, const char *buff, size_t bytes)
{
    struct mount_point *mp = file->f_mp;
    struct ramfs_element *ramfsFile = (struct ramfs_element *)file->f_data;

    if (file->f_offset < 0) {
        file->f_offset = 0;
    }

    if (file->f_offset + bytes > ramfsFile->f.size) {
        char *p;

        p = LOS_MemRealloc(((struct ramfs_mount_point *)mp->m_data)->memory, ramfsFile->f.content,
            file->f_offset + bytes);
        if (p != NULL) {
            ramfsFile->f.content = p;
            ramfsFile->f.size = file->f_offset + bytes;
        } else {
            if (ramfsFile->f.size <= (size_t)file->f_offset) {
                VFS_ERRNO_SET(ENOMEM);
                return (ssize_t)-1;
            }

            bytes = ramfsFile->f.size - file->f_offset;
        }
    }

    memcpy(ramfsFile->f.content + file->f_offset, buff, bytes);

    file->f_offset += bytes;

    return bytes;
}

static off_t RamfsLseek(struct file *file, off_t off, int whence)
{
    struct ramfs_element *ramfsFile = (struct ramfs_element *)file->f_data;

    switch (whence) {
        case SEEK_SET:
            file->f_offset = off;
            break;
        case SEEK_CUR:
            file->f_offset += off;
            break;
        case SEEK_END:
            file->f_offset = ramfsFile->f.size;
            break;
        default:
            VFS_ERRNO_SET(EINVAL);
            return -1;
    }

    if (file->f_offset < 0) {
        file->f_offset = 0;
    }

    if ((size_t)file->f_offset > ramfsFile->f.size) {
        file->f_offset = ramfsFile->f.size;
    }

    return file->f_offset;
}

static off64_t RamfsLseek64(struct file *filep, off64_t offset, int whence)
{
    return (off64_t)RamfsLseek(filep, (off_t)offset, whence);
}
static void RamfsDel(struct ramfs_element *e)
{
    struct ramfs_element *dir;
    struct ramfs_element *t;

    if (e->parent == NULL) { /* root element, do not delete */
        return;
    }

    dir = e->parent;

    t = dir->d.child;

    if (t == e) {
        dir->d.child = e->sibling;
    } else {
        while (t->sibling != e) {
            t = t->sibling;
        }

        t->sibling = e->sibling;
    }

    free(e);
}

static int RamfsUnlink(struct mount_point *mp, const char *pathInMp)
{
    struct ramfs_element *ramfsFile;

    ramfsFile = RamfsFileFind(mp, pathInMp, &pathInMp);

    if ((ramfsFile == NULL) || (*pathInMp != '\0')) {
        VFS_ERRNO_SET(ENOENT);
        return -1;
    }

    if (ramfsFile->refs != 0) {
        VFS_ERRNO_SET(EBUSY);
        return -1;
    }

    if (ramfsFile->type == RAMFS_TYPE_DIR) {
        if (ramfsFile->d.child != NULL) {
            VFS_ERRNO_SET(EBUSY); /* have file under it busy */
            return -1;
        }
    } else {
        if (ramfsFile->f.content != NULL) {
            LOS_MemFree(((struct ramfs_mount_point *)mp->m_data)->memory, ramfsFile->f.content);
            ramfsFile->f.content = NULL;
        }
    }

    RamfsDel(ramfsFile);

    return 0;
}

static int RamfsRename(struct mount_point *mp, const char *pathInMpOld, const char *pathInMpNew)
{
    struct ramfs_element *ramfsFileOld;
    struct ramfs_element *ramfsFileNew;

    ramfsFileOld = RamfsFileFind(mp, pathInMpOld, &pathInMpOld);

    if ((ramfsFileOld == NULL) || (*pathInMpOld != '\0')) {
        VFS_ERRNO_SET(ENOENT);
        return -1;
    }

    ramfsFileNew = RamfsFileFind(mp, pathInMpNew, &pathInMpNew);

    /*
     * ramfsFileNew == NULL means at least parent dir not found
     * *pathInMpNew == '\0' means file already exist
     */
    if ((ramfsFileNew == NULL) || (*pathInMpNew == '\0')) {
        VFS_ERRNO_SET(ENOENT);
        return -1;
    }

    /* must in the same dir */
    if (strchr(pathInMpNew, '/') != NULL) {
        VFS_ERRNO_SET(EISDIR);
        return -1;
    }

    /* must in the same dir */
    if (ramfsFileNew != ramfsFileOld->parent) {
        VFS_ERRNO_SET(EISDIR);
        return -1;
    }

    if (strlen(pathInMpNew) >= LOS_MAX_FILE_NAME_LEN) {
        VFS_ERRNO_SET(ENAMETOOLONG);
        return -1;
    }

    strcpy(ramfsFileOld->name, pathInMpNew);

    return 0;
}

static int RamfsOpendir(struct dir *dir, const char *pathInMp)
{
    struct ramfs_element *ramfsDir;
    struct mount_point *mp = dir->d_mp;

    ramfsDir = RamfsFileFind(mp, pathInMp, &pathInMp);

    if ((ramfsDir == NULL) || (*pathInMp != '\0')) {
        VFS_ERRNO_SET(ENOENT);
        return -1;
    }

    if (ramfsDir->type != RAMFS_TYPE_DIR) {
        VFS_ERRNO_SET(ENOTDIR);
        return -1;
    }

    ramfsDir->refs++;

    dir->d_data = (void *)ramfsDir;
    dir->d_offset = 0;

    return 0;
}

static int RamfsReaddir(struct dir *dir, struct dirent *dent)
{
    struct ramfs_element *ramfsDir = (struct ramfs_element *)dir->d_data;
    struct ramfs_element *child;
    off_t i;

    for (i = 0, child = ramfsDir->d.child; i < dir->d_offset && child != NULL; i++, child = child->sibling) {
        /* nop */
    }

    if (child == NULL) {
        VFS_ERRNO_SET(ENOENT);
        return -1;
    }

    strncpy_s(dent->name, LOS_MAX_DIR_NAME_LEN - 1, child->name, LOS_MAX_FILE_NAME_LEN - 1);
    dent->name[LOS_MAX_FILE_NAME_LEN - 1] = '\0';
    dent->size = 0;

    if (child->type == RAMFS_TYPE_DIR) {
        dent->type = VFS_TYPE_DIR;
    } else {
        dent->type = VFS_TYPE_FILE;
        dent->size = child->f.size;
    }

    dir->d_offset++;

    return 0;
}

static int RamfsClosedir(struct dir *dir)
{
    struct ramfs_element *ramfsDir = (struct ramfs_element *)dir->d_data;
    ramfsDir->refs--;
    return 0;
}

static int RamfsMkdir(struct mount_point *mp, const char *pathInMp)
{
    struct ramfs_element *ramfsParent;
    struct ramfs_element *ramfsDir;
    const char *t;
    int len;

    ramfsParent = RamfsFileFind(mp, pathInMp, &pathInMp);

    if ((ramfsParent == NULL) || (*pathInMp == '\0')) {
        return -1; /* dir already exist */
    }

    t = strchr(pathInMp, '/');

    if (t != NULL) {
        len = t - pathInMp;
        while (*t == '/') {
            t++;
        }

        if (*t != '\0') {
            return -1; /* creating dir under non-existed dir */
        }
    } else {
        len = strlen(pathInMp);
    }

    if (len >= LOS_MAX_FILE_NAME_LEN) {
        return -1;
    }

    ramfsDir = (struct ramfs_element *)malloc(sizeof(struct ramfs_element));

    if (ramfsDir == NULL) {
        PRINT_ERR("Fail to malloc memory in ramfs.\n");
        return -1;
    }

    (void)memset_s(ramfsDir, sizeof(struct ramfs_element), 0, sizeof(struct ramfs_element));

    strncpy_s(ramfsDir->name, LOS_MAX_FILE_NAME_LEN - 1, pathInMp, len);
    ramfsDir->type = RAMFS_TYPE_DIR;
    ramfsDir->sibling = ramfsParent->d.child;
    ramfsParent->d.child = ramfsDir;
    ramfsDir->parent = ramfsParent;

    return 0;
}

static struct file_ops g_ramfsOps = {
    RamfsOpen,
    RamfsClose,
    RamfsRead,
    RamfsWrite,
    RamfsLseek,
    RamfsLseek64,
    NULL,           /* stat not supported */
    RamfsUnlink,
    RamfsRename,
    NULL,           /* ioctl not supported */
    NULL,           /* sync not supported */
    RamfsOpendir,
    RamfsReaddir,
    RamfsClosedir,
    RamfsMkdir
};

static struct file_system g_ramfsFs = {"ramfs", &g_ramfsOps, NULL, 0};

int RamfsMount(const char *path, size_t blockSize)
{
    struct ramfs_mount_point *rmp;

    if (strlen(path) >= LOS_MAX_FILE_NAME_LEN) {
        return LOS_NOK;
    }

    rmp = (struct ramfs_mount_point *)malloc(sizeof(struct ramfs_mount_point));
    if (rmp == NULL) {
        PRINT_ERR("Fail to malloc memory in ramfs.\n");
        return LOS_NOK;
    }

    (void)memset_s(rmp, sizeof(struct ramfs_mount_point), 0, sizeof(struct ramfs_mount_point));
    rmp->root.type = RAMFS_TYPE_DIR;
    strncpy_s(rmp->root.name, LOS_MAX_FILE_NAME_LEN, path, LOS_MAX_FILE_NAME_LEN);
    rmp->memory = malloc(blockSize);

    if (rmp->memory == NULL) {
        PRINT_ERR("Fail to malloc memory in ramfs.\n");
        return LOS_NOK;
    }

    if (LOS_MemInit(rmp->memory, blockSize) != LOS_OK) {
        PRINT_ERR("Failed to init pool.\n");
        free(rmp->memory);
        return LOS_NOK;
    }

    if (LOS_FsMount("ramfs", path, rmp) == LOS_OK) {
        PRINT_INFO("Ramfs mount at %s done.\n", path);
        return LOS_OK;
    }

    PRINT_ERR("Failed to register fs.\n");

    free(rmp->memory);
    free(rmp);

    return LOS_NOK;
}

int RamfsInit(void)
{
    static int ramfsInited = FALSE;

    if (ramfsInited) {
        return LOS_OK;
    }

    if (LOS_VfsInit() != LOS_OK) {
        PRINT_ERR("vfs init fail!\n");
        return LOS_NOK;
    }

    if (LOS_FsRegister(&g_ramfsFs) != LOS_OK) {
        PRINT_ERR("failed to register fs!\n");
        return LOS_NOK;
    }

    PRINT_INFO("register fs done!\n");

    ramfsInited = TRUE;

    return LOS_OK;
}
