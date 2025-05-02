/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: PDFGen Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-07
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

#include "pdfgen_demo.h"
#include "pdfgen.h"
#include "stdio.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define FASTLZ_TASK_STACK_SIZE 0x1000
#define PDFGEN_TASK_PRIORITY   8
#define OUTPUT_FILENAME        "/fatfs/PDFGen_demo.pdf"

#define ADD_TEXT_LEN           16

STATIC UINT32 g_demoTaskId;

STATIC VOID TextDemo(struct pdf_doc *pdf)
{
    INT32 i, j;
    CHAR addText[ADD_TEXT_LEN] = {0};
    /* Generate multiplication table */
    for (i = 1; i < 10; i++) {      /* 10: multiplication table */
        for (j = i; j < 10; j++) {  /* 10: multiplication table */
            (VOID)sprintf(addText, "%d*%d=%d", i, j, (i * j));

            /* 
             * 12: text size.
             * (i * 60.0f): abscissa.
             * (PDF_A4_HEIGHT - 50 - (j * 20.0f)): ordinate.
             */
            (VOID)pdf_add_text(pdf, NULL, addText, 
                               12,                                 /* 12: text size. */
                               (i * 60.0f),                        /* (i * 60.0f): abscissa. */
                               (PDF_A4_HEIGHT - 50 - (j * 20.0f)), /* 50 - (j * 20.0f)): ordinate */
                               PDF_BLACK);
        }
    }
}

STATIC VOID LineDemo(struct pdf_doc *pdf)
{
    /* 50: x1, 500: y1, 150: x2, 500: y2, 6: width */
    (VOID)pdf_add_line(pdf, NULL, 50, 500, 150, 500, 6, PDF_RGB(0, 0xff, 0xff));
    /* 50: x1, 400: y1, 150: x2, 400: y2, 6: width */
    (VOID)pdf_add_line(pdf, NULL, 50, 400, 150, 400, 6, PDF_RGB(0xff, 0, 0));
    /* 50: x1, 500: y1, 50: x2, 400: y2, 6: width */
    (VOID)pdf_add_line(pdf, NULL, 50, 500, 50, 400, 6, PDF_BLACK);
    /* 150: x1, 500: y1, 150: x2, 400: y2, 6: width */
    (VOID)pdf_add_line(pdf, NULL, 150, 500, 150, 400, 6, PDF_RGB(0xff, 0, 0xff));

    /* 200: x1, 500: y1, 300: x2, 400: y2, 6: width */
    (VOID)pdf_add_line(pdf, NULL, 200, 500, 300, 400, 6, PDF_RGB(0, 0xff, 0));
    /* 300: x1, 500: y1, 200: x2, 400: y2, 6: width */
    (VOID)pdf_add_line(pdf, NULL, 300, 500, 200, 400, 6, PDF_RGB(0, 0, 0xff));
}

STATIC VOID RectangleDemo(struct pdf_doc *pdf)
{
    /* 350: x, 500: y, 100: width, -100: height, 4: border width */
    (VOID)pdf_add_rectangle(pdf, NULL, 350, 500, 100, -100, 4, PDF_RGB(0xff, 0xff, 0));
    /* 400: x, 450: y, 4: width, 4: height, 4: border width */
    (VOID)pdf_add_rectangle(pdf, NULL, 400, 450, 4, 4, 4, PDF_RGB(0xff, 0, 0));
    /* 500: x, 420: y, 50: width, 50: height, 4: border width */
    (VOID)pdf_add_filled_rectangle(pdf, NULL, 500, 420, 50, 50, 4, PDF_RGB(0xff, 0, 0));
}

STATIC VOID CircleAndCurveDemo(struct pdf_doc *pdf)
{
    /* 200 200: center of a circle, 50: radius, 5: width */
    (VOID)pdf_add_circle(pdf, NULL, 200, 200, 50, 5, PDF_RGB(0xff, 0, 0), PDF_TRANSPARENT);
    /* 200 200: center of a circle, 40: x radius, 30: y radius, 2: width */
    (VOID)pdf_add_ellipse(pdf, NULL, 200, 200, 40, 30, 2, PDF_RGB(0xff, 0xff, 0), PDF_RGB(0, 0, 0));

    /* 300: x1, 240: y1, 440: x2, 240: y2, 340: xq1, 260: yq1, 4: width */
    (VOID)pdf_add_quadratic_bezier(pdf, NULL, 300, 240, 440, 240, 340, 260, 4, PDF_RGB(0, 0, 0xff));
    /* 300: x1, 200: y1, 440: x2, 200: y2, 290: xq1, 130: yq1, 350: xq1, 130: yq2, 4: width */
    (VOID)pdf_add_cubic_bezier(pdf, NULL, 300, 200, 440, 200, 290, 130, 350, 130, 4, PDF_RGB(0, 0xff, 0));
    /* 350 200: center of a circle, 30: x radius, 20: y radius, 2: width */
    (VOID)pdf_add_ellipse(pdf, NULL, 350, 200, 30, 20, 2, PDF_RGB(0xff, 0xff, 0), PDF_RGB(0, 0, 0));
    (VOID)pdf_add_ellipse(pdf, NULL, 350, 200, 30, 20, 2, PDF_RGB(0xff, 0xff, 0), PDF_RGB(0, 0, 0));
}

STATIC INT32 DemoTaskEntry(VOID)
{
    struct pdf_doc *pdf = NULL;

    struct pdf_info info = {
        .creator = "PDFGen",
        .producer = "PDFGen",
        .title = "PDFGen Demo",
        .author = "Huawei LiteOS Team",
        .subject = "PDFGen Demo",
        .date = "2021-07-07"
    };

    printf("Pdfgen demo task start to run.\n");
    printf("Paper size: %f * %f.\n", PDF_A4_WIDTH, PDF_A4_HEIGHT);

    pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
    if (pdf == NULL) {
        printf("Unable to create pdf.\n");
        return -1;
    }

    pdf_set_font(pdf, "Times-Roman");
    pdf_append_page(pdf);

    TextDemo(pdf);
    LineDemo(pdf);
    RectangleDemo(pdf);
    CircleAndCurveDemo(pdf);

    pdf_save(pdf, OUTPUT_FILENAME);
    pdf_destroy(pdf);

    printf("Pdfgen demo task finished.\n");
    return 0;
}

VOID PdfgenDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = PDFGEN_TASK_PRIORITY;
    taskInitParam.pcName = "PdfgenDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = FASTLZ_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create Pdfgen demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
