/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: C-algorithms Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-08
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

#include "c_algorithms_demo.h"
#include <stdio.h>
#include <stdlib.h>
#include "los_task.h"
#include "los_vfs.h"
#include "hash-int.h"
#include "queue.h"
#include "arraylist.h"
#include "compare-int.h"
#include "hash-table.h"
#include "list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define ALGORITHMS_TASK_PRIORITY    6
#define ALGORITHMS_TASK_STACK_SIZE  0x2000
#define JUDGERET(ret, msg) \
    if ((ret) != LOS_NOK) {  \
        (VOID)printf("%s\n", msg); \
    }

STATIC UINT32 g_demoTaskId;

STATIC VOID QueueDemo(VOID)
{
    INT32 ret;
    QueueValue qValue;
    Queue *q = NULL;
    q = queue_new();
    if (q == NULL) {
        return;
    }
    ret = queue_push_tail(q, "xiaohong");
    JUDGERET(ret, "Push to queue tail failed.");
    ret = queue_push_tail(q, "xiaoming");
    JUDGERET(ret, "Push to queue tail failed.");
    ret = queue_push_tail(q, "xiaoqiang");
    JUDGERET(ret, "Push to queue tail failed.");
    qValue = queue_peek_head(q);
    if (qValue == NULL) {
        queue_free(q);
        return;
    }
    printf("First in line is %s.\n", (CHAR *)qValue);
    qValue = queue_peek_tail(q);
    if (qValue == NULL) {
        queue_free(q);
        return;
    }
    printf("Last in line is %s.\n", (CHAR *)qValue);
    qValue = queue_pop_head(q);
    if (qValue == NULL) {
        queue_free(q);
        return;
    }
    printf("%s bought a sandwich.\n", (CHAR *)qValue);
    qValue = queue_peek_head(q);
    if (qValue == NULL) {
        queue_free(q);
        return;
    }
    printf("Now %s turn is it to buy rice.\n", (CHAR *)qValue);
    ret = queue_push_head(q, "xiaobing");
    JUDGERET(ret, "Cutting to the front of the line failed.");
    qValue = queue_peek_head(q);
    if (qValue == NULL) {
        queue_free(q);
        return;
    }
    printf("%s cutting to the front change first in line.\n", (CHAR *)qValue);
    queue_free(q);
}

STATIC VOID ArrayDemoPrt(ArrayList *a)
{
    INT32 i;
    for (i = 0; i < a->length; i++) {
        printf("position[%d]->value[%d]\n", arraylist_index_of(a, int_equal, a->data[i]),
               *(INT32 *)a->data[i]);
    }
}

STATIC VOID ArrayDemo(VOID)
{
    INT32 ret;
    /* Num 5,2,7 for testing */
    INT32 a = 5;
    INT32 b = 2;
    INT32 c = 7;

    ArrayList *arrayList = NULL;
    arrayList = arraylist_new(0);
    if (arrayList == NULL) {
        return;
    }
    ret = arraylist_append(arrayList, &a);
    if (ret != LOS_NOK) {
        printf("Arraylist append %d failed.\n", a);
        arraylist_free(arrayList);
        return;
    }
    ret = arraylist_append(arrayList, &c);
    if (ret != LOS_NOK) {
        printf("Arraylist append %d failed.\n", c);
        arraylist_free(arrayList);
        return;
    }
    ArrayDemoPrt(arrayList);
    printf("Insert the number %d in the second digit\n", b);
    ret = arraylist_insert(arrayList, 1, &b);
    if (ret != LOS_NOK) {
        printf("Arraylist insert %d failed.\n", b);
        arraylist_free(arrayList);
        return;
    }
    ArrayDemoPrt(arrayList);
    printf("Arraylist remove first num.\n");
    arraylist_remove(arrayList, 0);
    ArrayDemoPrt(arrayList);
    arraylist_free(arrayList);
}

STATIC VOID HashDemo(VOID)
{
    INT32 ret;
    char *hashValue = NULL;
    HashTable *hash = NULL;
    hash = hash_table_new(int_hash, int_equal);
    if (hash == NULL) {
        return;
    }
    ret = hash_table_insert(hash, "1", "one");
    if (ret != LOS_NOK) {
        printf("Hash table insert failed.\n");
        hash_table_free(hash);
        return;
    }
    ret = hash_table_insert(hash, "2", "four");
    if (ret != LOS_NOK) {
        printf("Hash table insert failed.\n");
        hash_table_free(hash);
        return;
    }
    ret = hash_table_insert(hash, "3", "nine");
    if (ret != LOS_NOK) {
        printf("Hash table insert failed.\n");
        hash_table_free(hash);
        return;
    }
    hashValue = hash_table_lookup(hash, "1");
    if (hashValue == HASH_TABLE_NULL) {
        hash_table_free(hash);
        return;
    }
    printf("%s \n", hashValue);
    hash_table_free(hash);
}

STATIC VOID ListDemoPrt(ListEntry *list, INT32 len)
{
    INT32 i;
    INT32 *listValue = NULL;
    for (i = 0; i < len; i++) {
        listValue = (INT32 *)list_nth_data(list, i);
        if (listValue != NULL) {
            printf("%d ", *listValue);
        }
    }
    printf("\n");
}

STATIC VOID ListAppendDemo(INT32 *a, INT32 len)
{
    ListEntry *appendList = NULL;
    ListEntry *findRet = NULL;
    INT32 *findData = NULL;
    INT32 listLen;
    INT32 ret;
    INT32 i;

    for (i = 0; i < len; i++) {
        (VOID)list_append(&appendList, &a[i]);
    }
    printf("Array a append to list:\n");
    ListDemoPrt(appendList, len);
    list_sort(&appendList, int_compare);
    printf("Sort num :\n");
    ListDemoPrt(appendList, len);
    ret = list_remove_data(&appendList, int_equal, &a[2]); // 2:the index of array
    if (ret != LOS_NOK) {
        printf("List remove data failed.\n");
        list_free(appendList);
        return;
    }
    listLen = list_length(appendList);
    printf("After remove array a third num:\n");
    ListDemoPrt(appendList, listLen);
    findRet = list_find_data(appendList, int_equal, &a[1]);
    findData = (INT32 *)list_data(findRet);
    printf("Find array a second data is [%d].\n", *findData);

    list_free(appendList);
}

STATIC VOID ListPrependDemo(INT32 *a, INT32 len)
{
    ListEntry *prependList = NULL;
    ListEntry *entry = NULL;
    INT32 listLen;
    INT32 ret;
    INT32 i;

    for (i = 0; i < len; i++) {
        (VOID)list_prepend(&prependList, &a[i]);
    }
    printf("Array a prepend to list:\n");
    ListDemoPrt(prependList, len);
    entry = list_nth_entry(prependList, 2); // 2:list position
    if (entry == NULL) {
        list_free(prependList);
        return;
    }
    ret = list_remove_entry(&prependList, entry);
    if (ret != LOS_NOK) {
        printf("List remove entry failed.\n");
        list_free(prependList);
        return;
    }
    listLen = list_length(prependList);
    printf("After remove, len is [%d], list is:\n", listLen);
    ListDemoPrt(prependList, listLen);

    list_free(prependList);
}

STATIC VOID ListDemo(VOID)
{
    INT32 a[] = {1, 3, 5, 2, 4, 6};
    INT32 len;
    len = (INT32)(sizeof(a) / sizeof(INT32));
    ListAppendDemo(a, len);
    ListPrependDemo(a, len);
}

STATIC VOID DemoTaskEntry(VOID)
{
    printf("C algorithms demo task to run.\n");
    printf("Queue demo start to run.\n");
    QueueDemo();
    printf("Array demo start to run.\n");
    ArrayDemo();
    printf("Hash demo start to run.\n");
    HashDemo();
    printf("List demo start to run.\n");
    ListDemo();
    printf("C algorithms demo task to finished.\n");
}

VOID AlgorithmsDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = ALGORITHMS_TASK_PRIORITY;
    taskInitParam.pcName = "AlgorithmsDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = ALGORITHMS_TASK_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create Algorithms demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
