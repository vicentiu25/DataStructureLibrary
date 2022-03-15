#include "ccvector.h"
#include "common.h"
#include "string.h"

#define INITIAL_SIZE 100

int VecCreate(CC_VECTOR **Vector)
{
    CC_VECTOR *vec = NULL;

    CC_UNREFERENCED_PARAMETER(Vector);

    if (NULL == Vector)
    {
        return -1;
    }

    if (-1 == VecDestroy(Vector))
    {
        return -1;
    }

    vec = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
    if (NULL == vec)
    {
        return -1;
    }

    memset(vec, 0, sizeof(*vec));

    vec->Count = 0;
    vec->Size = INITIAL_SIZE;
    vec->Array = (int*)malloc( sizeof(int) * INITIAL_SIZE );
    if (NULL == vec->Array) 
    {
        free(vec);
        return -1;
    }

    *Vector = vec;

    return 0;
}

int VecDestroy(CC_VECTOR **Vector)
{
    CC_VECTOR *vec = *Vector;

    if (NULL == Vector)
    {
        return -1;
    }

    if (NULL != vec)
    {
        free(vec->Array);
    }


    free(vec);

    *Vector = NULL;

    return 0;
}

int VecInsertTail(CC_VECTOR *Vector, int Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        /// REALLOC
        Vector->Size = (Vector->Size) * 2;
        int* vec = (int*)realloc(Vector->Array, sizeof(int) * Vector->Size);
        if (NULL == vec)
        {
            return -1;
        }
        Vector->Array = vec;
    }

    Vector->Array[Vector->Count] = Value;
    Vector->Count++;

    return 0;
}

int VecInsertHead(CC_VECTOR *Vector, int Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Vector)
    {
        return -1;
    }

    if (Vector->Count + 1 >= Vector->Size)
    {
        /// REALLOC
        Vector->Size = (Vector->Size) * 2;
        int* vec = (int*)realloc(Vector->Array, sizeof(int) * Vector->Size);
        if (NULL == vec)
        {
            return -1;
        }
        Vector->Array = vec;
    }
    Vector->Count++;
    for (int i = Vector->Count - 2; i >= 0; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[0] = Value;

    return 0;
}

int VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Vector)
    {
        return -1;
    }

    if (-1 >= Index || Vector->Count <= Index)
    {
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        /// REALLOC
        Vector->Size = (Vector->Size) * 2;
        int* vec = (int*)realloc(Vector->Array, sizeof(int) * (Vector->Size));
        if (NULL == vec)
        {
            return -1;
        }
        Vector->Array = vec;
    }

    Vector->Count++;
    for (int i = Vector->Count - 2; i > Index; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i]; 
    }
    Vector->Array[Index + 1] = Value;


    return 0;
}

int VecRemoveByIndex(CC_VECTOR *Vector, int Index)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);

    if (NULL == Vector)
    {
        return -1;
    }

    if (Index <= -1 || Index > Vector->Count - 1)
    {
        return -1;
    }

    for (int i = Index; i < Vector->Count - 1; i++)
    {
        Vector->Array[i] = Vector->Array[i + 1];
    }

    Vector->Count--;

    return 0;
}

int VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Vector);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Vector)
    {
        return -1;
    }

    if (-1 >= Index || Index > Vector->Count - 1)
    {
        return -1;
    }

    *Value = Vector->Array[Index];

    return 0;
}

int VecGetCount(CC_VECTOR *Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);

    if (NULL == Vector)
    {
        return -1;
    }

    return Vector->Count;
}

int VecClear(CC_VECTOR *Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);

    if (NULL == Vector)
    {
        return -1;
    }
    
    memset(Vector, 0, sizeof(*Vector));

    Vector->Count = 0;
    Vector->Size = INITIAL_SIZE;
    int* vec = (int*)realloc(Vector->Array, sizeof(int) * INITIAL_SIZE);
    if (NULL == vec)
    {
        return -1;
    }
    Vector->Array = vec;
    return 0;
}

int QPartition(CC_VECTOR* Vector, int left, int right)
{
    int k = rand() % (right - left + 1) + left;
    int aux = Vector->Array[right];
    Vector->Array[right] = Vector->Array[k];
    Vector->Array[k] = aux;
    int i = left - 1;
    int j = left;
    while (j < right) {
        if (Vector->Array[j] > Vector->Array[right]) {
            i++;
            aux = Vector->Array[j];
            Vector->Array[j] = Vector->Array[i];
            Vector->Array[i] = aux;
            j++;
        }
        else {
            j++;
        }
    }
    aux = Vector->Array[right];
    Vector->Array[right] = Vector->Array[i + 1];
    Vector->Array[i + 1] = aux;
    return i + 1;
}

void VecQuickSort(CC_VECTOR* Vector, int left, int right)
{
    if (left < right) {
        int q = QPartition(Vector, left, right);
        VecQuickSort(Vector, left, q - 1);
        VecQuickSort(Vector, q + 1, right);
    }
}

int VecSort(CC_VECTOR *Vector)
{
    CC_UNREFERENCED_PARAMETER(Vector);

    if (NULL == Vector)
    {
        return -1;
    }
    VecQuickSort(Vector, 0, Vector->Count - 1);

    return 0;
}

int VecAppend(CC_VECTOR *DestVector, CC_VECTOR *SrcVector)
{
    CC_UNREFERENCED_PARAMETER(DestVector);
    CC_UNREFERENCED_PARAMETER(SrcVector);

    if (NULL == DestVector || NULL == SrcVector)
    {
        return -1;
    }

    if (SrcVector->Count + DestVector->Count >= SrcVector->Size)
    {
        /// REALLOC
        while (SrcVector->Count + DestVector->Count > SrcVector->Size)
        {
            SrcVector->Size = (SrcVector->Size) * 2;
        }
        int* vec = (int*)realloc(SrcVector->Array, sizeof(int) * SrcVector->Size);
        if (NULL == vec)
        {
            return -1;
        }
        SrcVector->Array = vec;
    }

    for (int i = 0; i < DestVector->Count; i++)
    {
        SrcVector->Array[SrcVector->Count] = DestVector->Array[i];
        SrcVector->Count++;
    }

    return 0;
}
