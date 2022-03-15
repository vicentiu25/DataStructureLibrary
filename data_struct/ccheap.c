#include "ccheap.h"
#include "common.h"
#include "string.h"

int MaxHeapify(CC_HEAP* MaxHeap, int Root)
{
    if (NULL == MaxHeap)
    {
        return -1;
    }
    int left = 2 * Root + 1;
    int right = 2 * Root + 2;
    int largest, valLeft = -1, valRight = -1, valRoot = -1, valLargest = -1;
    int count = HpGetElementCount(MaxHeap);
    if (-1 == count)
    {
        return -1;
    }
    if (left <= count - 1)
    {
        if (-1 == VecGetValueByIndex(MaxHeap->Elements, left, &valLeft))
        {
            return -1;
        }
    }
    if (right <= count - 1)
    {
        if (-1 == VecGetValueByIndex(MaxHeap->Elements, right, &valRight))
        {
            return -1;
        }
    }
    if (-1 == VecGetValueByIndex(MaxHeap->Elements, Root, &valRoot))
    {
        return -1;
    }
    
    if (left <= count - 1 && valLeft > valRoot) {
        largest = left;
        valLargest = valLeft;
    }
    else {
        largest = Root;
        valLargest = valRoot;
    }
    if (right <= count - 1 && valRight > valLargest) {
        largest = right;
        valLargest = valRight;
    }
    if (largest != Root) {
        //MaxHeap->Elements->Array[Root] = valLargest;
        if (-1 == VecInsertAfterIndex(MaxHeap->Elements, Root, valLargest))
        {
            return -1;
        }
        if (-1 == VecRemoveByIndex(MaxHeap->Elements, Root))
        {
            return -1;
        }
        //MaxHeap->Elements->Array[largest] = valRoot;
        if (-1 == VecInsertAfterIndex(MaxHeap->Elements, largest, valRoot))
        {
            return -1;
        }
        if (-1 == VecRemoveByIndex(MaxHeap->Elements, largest))
        {
            return -1;
        }
        if (-1 == MaxHeapify(MaxHeap, largest))
        {
            return -1;
        }
    }

    return 0;
}

int MinHeapify(CC_HEAP* MinHeap, int Root)
{
    if (NULL == MinHeap)
    {
        return -1;
    }
    int left = 2 * Root + 1;
    int right = 2 * Root + 2;
    int smallest, valLeft = -1, valRight = -1, valRoot = -1, valSmallest = -1;
    int count = HpGetElementCount(MinHeap);
    if (-1 == count)
    {
        return -1;
    }
    if (left <= count - 1)
    {
        if (-1 == VecGetValueByIndex(MinHeap->Elements, left, &valLeft))
        {
            return -1;
        }
    }
    if (right <= count - 1)
    {
        if (-1 == VecGetValueByIndex(MinHeap->Elements, right, &valRight))
        {
            return -1;
        }
    }
    if (-1 == VecGetValueByIndex(MinHeap->Elements, Root, &valRoot))
    {
        return -1;
    }
    if (left <= count - 1 && valLeft < valRoot) {
        smallest = left;
        valSmallest = valLeft;
    }
    else {
        smallest = Root;
        valSmallest = valRoot;
    }
    if (right <= count - 1 && valRight < valSmallest) {
        smallest = right;
        valSmallest = valRight;
    }
    if (smallest != Root) {
        //MaxHeap->Elements->Array[Root] = valSmallest;
        if (-1 == VecInsertAfterIndex(MinHeap->Elements, Root, valSmallest))
        {
            return -1;
        }
        if (-1 == VecRemoveByIndex(MinHeap->Elements, Root))
        {
            return -1;
        }
        //MaxHeap->Elements->Array[smallest] = valRoot;
        if (-1 == VecInsertAfterIndex(MinHeap->Elements, smallest, valRoot))
        {
            return -1;
        }
        if (-1 == VecRemoveByIndex(MinHeap->Elements, smallest))
        {
            return -1;
        }
        if (-1 == MinHeapify(MinHeap, smallest))
        {
            return -1;
        }
    }

    return 0;
}

int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
    CC_HEAP* mH = NULL;

    CC_UNREFERENCED_PARAMETER(MaxHeap);
    CC_UNREFERENCED_PARAMETER(InitialElements);

    if (NULL == MaxHeap)
    {
        return -1;
    }

    mH = (CC_HEAP*)malloc(sizeof(CC_HEAP));
    if (NULL == mH)
    {
        return -1;
    }
    memset(mH, 0, sizeof(*mH));

    if (-1 == VecCreate(&mH->Elements))
    {
        return -1;
    }
    mH->Type = 1;

    if (NULL != InitialElements)
    {
        if (-1 == VecAppend(InitialElements, mH->Elements))
        {
            return -1;
        }

        for (int i = VecGetCount(mH->Elements) / 2 - 1; i >= 0; i--)
        {
            if (-1 == MaxHeapify(mH, i))
            {
                return -1;
            }
        }
    }

    *MaxHeap = mH;

    return 0;
}

int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
    CC_HEAP* mH = NULL;

    CC_UNREFERENCED_PARAMETER(MinHeap);
    CC_UNREFERENCED_PARAMETER(InitialElements);

    if (NULL == MinHeap)
    {
        return -1;
    }

    mH = (CC_HEAP*)malloc(sizeof(CC_HEAP));
    if (NULL == mH)
    {
        return -1;
    }
    memset(mH, 0, sizeof(*mH));

    if (-1 == VecCreate(&mH->Elements))
    {
        return -1;
    }

    mH->Type = 0;

    if (NULL != InitialElements)
    {
        if (-1 == VecAppend(InitialElements, mH->Elements))
        {
            return -1;
        }

        int count = HpGetElementCount(mH);
        if (-1 == count)
        {
            return -1;
        }
        for (int i = count / 2 - 1; i >= 0; i--)
        {
            if (-1 == MinHeapify(mH, i))
            {
                return -1;
            }
        }
    }

    *MinHeap = mH;

    return 0;
}

int HpDestroy(CC_HEAP **Heap)
{
    CC_HEAP* hp = *Heap;
    CC_UNREFERENCED_PARAMETER(Heap);

    if (NULL == Heap)
    {
        return -1;
    }

    if (-1 == VecDestroy(&hp->Elements))
    {
        return -1;
    }
    free(hp);

    *Heap = NULL;

    return 0;
}

int HpPush(CC_HEAP* Heap, int Index)
{
    int parent = (Index - 1) / 2;
    if (0 != Index)
    {
        int indexValue = -1, parentValue = -1;
        if (-1 == VecGetValueByIndex(Heap->Elements, Index, &indexValue))
        {
            return -1;
        }
        if (-1 == VecGetValueByIndex(Heap->Elements, parent, &parentValue))
        {
            return -1;
        }
        if (Heap->Type == 1)
        {
            if (parentValue < indexValue)
            {
                //Heap->Elements->Array[parent] = indexValue;
                if (-1 == VecInsertAfterIndex(Heap->Elements, parent, indexValue))
                {
                    return -1;
                }
                if (-1 == VecRemoveByIndex(Heap->Elements, parent))
                {
                    return -1;
                }
                //Heap->Elements->Array[Index] = parentValue;
                if (-1 == VecInsertAfterIndex(Heap->Elements, Index, parentValue))
                {
                    return -1;
                }
                if (-1 == VecRemoveByIndex(Heap->Elements, Index))
                {
                    return -1;
                }
                return HpPush(Heap, parent);
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if (parentValue > indexValue)
            {
                //Heap->Elements->Array[parent] = indexValue;
                if (-1 == VecInsertAfterIndex(Heap->Elements, parent, indexValue))
                {
                    return -1;
                }
                if (-1 == VecRemoveByIndex(Heap->Elements, parent))
                {
                    return -1;
                }
                //Heap->Elements->Array[Index] = parentValue;
                if (-1 == VecInsertAfterIndex(Heap->Elements, Index, parentValue))
                {
                    return -1;
                }
                if (-1 == VecRemoveByIndex(Heap->Elements, Index))
                {
                    return -1;
                }
                return HpPush(Heap, parent);
            }
            else
            {
                return 0;
            }
        }
    }

    return 0;
}

int HpInsert(CC_HEAP *Heap, int Value)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Heap)
    {
        return -1;
    }

    if (-1 == VecInsertTail(Heap->Elements, Value))
    {
        return -1;
    }

    int count = HpGetElementCount(Heap);
    if (-1 == count)
    {
        return -1;
    }
    HpPush(Heap, count - 1);

    return 0;
}

int HpDelete(CC_HEAP* Heap, int Index)
{
    int count = HpGetElementCount(Heap);
    
    if (-1 == count)
    {
        return -1;
    }
    if (Index != count - 1)
    {
        //Heap->Elements->Array[Index] = Heap->Elements->Array[count - 1];
        int valCount = -1;
        if (-1 == VecGetValueByIndex(Heap->Elements, count - 1, &valCount))
        {
            return -1;
        }
        if (-1 == VecInsertAfterIndex(Heap->Elements, Index, valCount))
        {
            return -1;
        }
        if (-1 == VecRemoveByIndex(Heap->Elements, Index))
        {
            return -1;
        }
        //delete element
        if (-1 == VecRemoveByIndex(Heap->Elements, count - 1))
        {
            return -1;
        }
        //heapify/push index
        if (1 == Heap->Type)
        {
            if (-1 == HpPush(Heap, Index))
            {
                return -1;
            }
            if (-1 == MaxHeapify(Heap, Index))
            {
                return -1;
            }
        }
        else
        {
            if (-1 == HpPush(Heap, Index))
            {
                return -1;
            }
            if (-1 == MinHeapify(Heap, Index))
            {
                return -1;
            }
        }
    }
    else
    {
        if (-1 == VecRemoveByIndex(Heap->Elements, count - 1))
        {
            return -1;
        }
    }

    return 0;
}

int HpRemove(CC_HEAP *Heap, int Value)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Heap)
    {
        return -1;
    }

    int count = HpGetElementCount(Heap);
    if (-1 == count)
    {
        return -1;
    }

    int element = -1;
    for (int i = 0; i < count; i++)
    {
        if (-1 == VecGetValueByIndex(Heap->Elements, i, &element))
        {
            return -1;
        }
        if (Value == element)
        {
            if (-1 == HpDelete(Heap, i))
            {
                return -1;
            }
            i--; // new element on position [i] after delete
        }
        count = HpGetElementCount(Heap);
        if (-1 == count)
        {
            return -1;
        }
    }

    return 0;
}

int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(ExtremeValue);

    if (NULL == Heap)
    {
        return -1;
    }

    if (0 >= HpGetElementCount(Heap))
    {
        return -1;
    }

    if (-1 == VecGetValueByIndex(Heap->Elements, 0, ExtremeValue))
    {
        return -1;
    }

    return 0;
}

int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(ExtremeValue);

    if (NULL == Heap)
    {
        return -1;
    }

    if (-1 == HpGetExtreme(Heap, ExtremeValue))
    {
        return -1;
    }

    if (-1 == HpRemove(Heap, *ExtremeValue))
    {
        return -1;
    }

    return 0;
}

int HpGetElementCount(CC_HEAP *Heap)
{
    CC_UNREFERENCED_PARAMETER(Heap);

    if (NULL == Heap)
    {
        return -1;
    }

    return VecGetCount(Heap->Elements);
}

int HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector)
{
    CC_UNREFERENCED_PARAMETER(Heap);
    CC_UNREFERENCED_PARAMETER(SortedVector);

    if (NULL == Heap)
    {
        return -1;
    }

    if (NULL == SortedVector)
    {
        return -1;
    }

    CC_HEAP* sortingHeap = NULL;
    if (Heap->Type == 1)
    {
        if (-1 == HpCreateMaxHeap(&sortingHeap, Heap->Elements))
        {
            return -1;
        }
    }
    else
    {
        if (-1 == HpCreateMinHeap(&sortingHeap, Heap->Elements))
        {
            return -1;
        }
    }
    int value = -1;
    while (0 != HpGetElementCount(sortingHeap))
    {
        if (-1 == HpGetExtreme(sortingHeap, &value))
        {
            return -1;
        }
        if (-1 == HpDelete(sortingHeap, 0))
        {
            return -1;
        }
        if (Heap->Type == 1)
        {
            if (-1 == VecInsertHead(SortedVector, value))
            {
                return -1;
            }
        }
        else
        {
            if (-1 == VecInsertTail(SortedVector, value))
            {
                return -1;
            }
        }
    }

    if (NULL != sortingHeap)
    {
        if (-1 == HpDestroy(&sortingHeap))
        {
            return -1;
        }
    }

    return 0;
}
