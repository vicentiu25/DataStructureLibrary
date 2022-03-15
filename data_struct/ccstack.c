#include "ccstack.h"
#include "common.h"
#include "string.h"

#define INITIAL_SIZE    100

int StCreate(CC_STACK **Stack)
{
    CC_STACK* st = NULL;

    CC_UNREFERENCED_PARAMETER(Stack);

    if (NULL == Stack)
    {
        return -1;
    }

    st = (CC_STACK*)malloc(sizeof(CC_STACK));
    if (NULL == st)
    {
        return -1;
    }

    memset(st, 0, sizeof(*st));

    st->Top = 0;
    st->Size = INITIAL_SIZE;
    st->Array = (int*)malloc(sizeof(int) * INITIAL_SIZE);
    if (NULL == st->Array)
    {
        free(st);
        return -1;
    }

    *Stack = st;

    return 0;
}

int StDestroy(CC_STACK **Stack)
{
    CC_STACK* st = *Stack;

    CC_UNREFERENCED_PARAMETER(Stack);

    if (NULL == Stack)
    {
        return -1;
    }

    free(st->Array);
    free(st);

    *Stack = NULL;

    return 0;
}

int StPush(CC_STACK *Stack, int Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Stack) 
    {
        return -1;
    }

    if (Stack->Top >= Stack->Size)
    {
        /// REALLOC
        Stack->Size = (Stack->Size) * 2;
        int* st = (int*)realloc(Stack->Array, sizeof(int) * Stack->Size);
        if (NULL == st)
        {
            return -1;
        }
        Stack->Array = st;
    }

    Stack->Array[Stack->Top] = Value;
    Stack->Top++;

    return 0;
}

int StPop(CC_STACK *Stack, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Stack)
    {
        return -1;
    }

    if (0 == Stack->Top)
    {
        return -1;
    }

    Stack->Top--;
    *Value = Stack->Array[Stack->Top];

    return 0;
}

int StPeek(CC_STACK *Stack, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(Value);
    
    if (NULL == Stack)
    {
        return -1;
    }

    *Value = Stack->Array[Stack->Top - 1];

    return 0;
}

int StIsEmpty(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    if (NULL == Stack)
    {
        return -1;
    }

    if (0 == Stack->Top)
    {
        return 1;
    }

    return 0;
}

int StGetCount(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    if (NULL == Stack)
    {
        return -1;
    }

    return Stack->Top;
}

int StClear(CC_STACK *Stack)
{
    CC_UNREFERENCED_PARAMETER(Stack);

    if (NULL == Stack)
    {
        return -1;
    }

    memset(Stack, 0, sizeof(*Stack));

    Stack->Top = 0;
    Stack->Size = INITIAL_SIZE;
    int* st = (int*)realloc(Stack->Array, sizeof(int) * INITIAL_SIZE);
    if (NULL == st)
    {
        return -1;
    }
    Stack->Array = st;

    return 0;
}

int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush)
{
    CC_UNREFERENCED_PARAMETER(Stack);
    CC_UNREFERENCED_PARAMETER(StackToPush);

    if (NULL == Stack || NULL == StackToPush)
    {
        return -1;
    }

    CC_STACK *auxStack;
    if (-1 == StCreate(&auxStack))
    {
        return -1;
    }

    int val = -1;
    while (0 < StackToPush->Top)
    {
        if (-1 == StPop(StackToPush, &val))
        {
            return -1;
        }
        if (-1 == StPush(auxStack, val))
        {
            return -1;
        }
    }

    while (0 < auxStack->Top)
    {
        if (-1 == StPop(auxStack, &val))
        {
            return -1;
        }
        if (-1 == StPush(Stack, val))
        {
            return -1;
        }
    }

    if (-1 == StDestroy(&auxStack))
    {
        return -1;
    }

    return 0;
}
