/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: FreeType Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-02-24
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

#if !defined(LOSCFG_FATFS_USE_SD_CARD) || !defined(LOSCFG_COMPONENTS_FS_FATFS)
#error This demo needs to enable Components --->FileSystem--->Enable FATFS--->Choose Peripheral Driver (SD Card)
#endif

#include "freetype_demo.h"
#include "stdio.h"
#include "string.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define FATFS_PATH "/fatfs"
#define TTF_FILE   "/fatfs/inkree.ttf"

#define FT_CHAR_WIDTH      (32 * 64)
#define FT_CHAR_HEIGHT     (32 * 64)
#define FT_HORZ_RESOLUTION 100
#define FT_VERT_RESOLUTION 100

#define FREETYPE_TASK_PRIORITY   3
#define FREETYPE_TASK_STACK_SIZE 0x2000

STATIC UINT32 g_demoTaskId;

typedef struct {
    UINT8 ftLibraryHandleFlag;
    UINT8 ftFaceHandleFlag;
    FT_Library ftLibraryHandle;
    FT_Face ftFaceHandle;
} FtHandle;

typedef struct {
    FT_F26Dot6  charWidth;
    FT_F26Dot6  charHeight;
    FT_UInt     horzResolution;
    FT_UInt     vertResolution;
} FtCharParam;

STATIC VOID FtHandleFree(FtHandle *handle)
{
    if ((handle->ftFaceHandle != NULL) && (handle->ftFaceHandleFlag)) {
        (VOID)FT_Done_Face(handle->ftFaceHandle);
    }
    if ((handle->ftLibraryHandle != NULL) && (handle->ftLibraryHandleFlag)) {
        (VOID)FT_Done_FreeType(handle->ftLibraryHandle);
    }
}

STATIC VOID ShowFtFaceInfo(FT_Face faceHandle)
{
    printf("FamilyName: %s, GlyphsNum: %u, FacesNum: %u.\n", \
        faceHandle->family_name, (UINT32)faceHandle->num_glyphs, (UINT32)faceHandle->num_faces);
}

STATIC FT_Error LoadFtNewFace(FtHandle *handle, FtCharParam *charParam, const CHAR *fileName)
{
    FT_Error ftError = FT_Init_FreeType(&handle->ftLibraryHandle);
    handle->ftLibraryHandleFlag = TRUE;
    if (ftError) {
        printf("Init freetype failed.\n");
        return LOS_NOK;
    }
    ftError = FT_New_Face(handle->ftLibraryHandle, fileName, 0, &handle->ftFaceHandle);
    handle->ftFaceHandleFlag = TRUE;
    if (ftError) {
        printf("Set ft new face failed.\n");
        return LOS_NOK;
    }
    ftError = FT_Set_Char_Size(handle->ftFaceHandle, charParam->charWidth, charParam->charHeight,
        charParam->horzResolution, charParam->vertResolution);
    if (ftError) {
        printf("Set char size failed.\n");
        return LOS_NOK;
    }
    ShowFtFaceInfo(handle->ftFaceHandle);
    return LOS_OK;
}

STATIC VOID ShowFtBitmap(FtHandle *handle, FT_ULong charCode)
{
    FT_Glyph ftGlyphHandle;
    FT_Error ftError;
    FT_UInt ftGlyphIndex;
    FT_BitmapGlyph bitMapGlyph = {0};
    FT_Bitmap bitMap;
    INT32 row;
    INT32 width;
    if (handle->ftFaceHandle == NULL) {
        return;
    }
    printf("Input charcode is character ['%c'].\n", (CHAR)charCode);
    ftGlyphIndex = FT_Get_Char_Index(handle->ftFaceHandle, charCode);
    ftError = FT_Load_Glyph(handle->ftFaceHandle, ftGlyphIndex, FT_LOAD_DEFAULT);
    if (ftError || (handle->ftFaceHandle->glyph == NULL)) {
        printf("Load glyph failed.\n");
        return;
    }

    ftError = FT_Render_Glyph(handle->ftFaceHandle->glyph, FT_RENDER_MODE_NORMAL);
    if (ftError) {
        printf("Render glyph failed.\n");
        return;
    }
    ftError = FT_Get_Glyph(handle->ftFaceHandle->glyph, &ftGlyphHandle);
    if (ftError) {
        printf("Get glyph failed.\n");
        return;
    }
    ftError = FT_Glyph_To_Bitmap(&ftGlyphHandle, ft_render_mode_normal, NULL, 1);
    if (ftError) {
        printf("Glyph to bitmap failed.\n");
        return;
    }
    bitMapGlyph = (FT_BitmapGlyph)(VOID *)(UINT8 *)ftGlyphHandle;
    bitMap = bitMapGlyph->bitmap;
    printf("Output bitmap data of charcode '%c':\n", (CHAR)charCode);
    for (row = 0; row < bitMap.rows; row++) {
        for (width = 0; width < bitMap.width; width++) {
            printf("%d ", bitMap.buffer[row * bitMap.width + width] > 0 ? 0 : 1);
        }
        printf("\n");
    }
    printf("\n");
    FT_Done_Glyph(ftGlyphHandle);
}

STATIC VOID DemoTaskEntry(VOID)
{
    FtHandle ftHandle;
    FtCharParam charParam;
    FT_Error ftError;
    INT32 ret;
    charParam.charWidth = FT_CHAR_WIDTH;
    charParam.charHeight = FT_CHAR_HEIGHT;
    charParam.horzResolution = FT_HORZ_RESOLUTION;
    charParam.vertResolution = FT_VERT_RESOLUTION;

    printf("Freetype demo task start to run.\n");
    ret = memset_s(&ftHandle, sizeof(FtHandle), 0, sizeof(FtHandle));
    if (ret != EOK) {
        printf("Ft handle memset failed.\n");
        return;
    }

    ftError = LoadFtNewFace(&ftHandle, &charParam, TTF_FILE);
    if (ftError != LOS_OK) {
        printf("Load new face failed.\n");
        FtHandleFree(&ftHandle);
    }
    ShowFtBitmap(&ftHandle, 'H');
    FtHandleFree(&ftHandle);
    printf("Freetype demo task finished.\n");
}

VOID FreeTypeDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = FREETYPE_TASK_PRIORITY;
    taskInitParam.pcName = "FreeTypeDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = FREETYPE_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create freetype demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
