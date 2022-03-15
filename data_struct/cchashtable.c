#include <string.h>
#include "cchashtable.h"
#include "common.h"

#define SIZE 1013
#define CONST 33
#define CONST1 11
#define CONST2 13

int HtStrlen(char* Key)
{
    if (NULL == Key)
    {
        return -1;
    }
    int count = 0;
    while ('\0' != Key[count])
    {
        count++;
    }
    return count;
}

int HtStrcmp(char* Key1, char* Key2)
{
    if (NULL == Key1)
    {
        return -1;
    }
    if (NULL == Key2)
    {
        return -1;
    }

    int len1 = HtStrlen(Key1);
    int len2 = HtStrlen(Key2);
    if (-1 == len1)
    {
        return -1;
    }
    if (-1 == len2)
    {
        return -1;
    }

    if (len1 != len2)
    {
        return 1;
    }

    for (int i = 0; i < len1; i++)
    {
        if (Key1[i] != Key2[i])
        {
            return 1;
        }
    }

    return 0;
}

int HtStrcpy(char** DestKey, char* SrcKey)
{
    if (NULL == DestKey)
    {
        return -1;
    }
    if (NULL == SrcKey)
    {
        return -1;
    }

    int len = HtStrlen(SrcKey);
    if (-1 == len)
    {
        return -1;
    }
    
    char* key  = (char*)realloc(*DestKey, sizeof(char) * (len + 1));
    if (NULL == key)
    {
        return -1;
    }
    *DestKey = key;

    for (int i = 0; i < len; i++)
    {
        (*DestKey)[i] = SrcKey[i];
    }
    (*DestKey)[len] = '\0';
    return 0;
}

int HtHashFunction(char* Key, int I)
{
    CC_UNREFERENCED_PARAMETER(Key);

    if (NULL == Key)
    {
        return -1;
    }

    int sum = 0;
    int x = CONST;
    int len = HtStrlen(Key);
    if (-1 == len)
    {
        return -1;
    }
    for (int j = 0; j < len; j++)
    {
        sum = (sum + x * Key[j] % SIZE) % SIZE;
        x = (x * CONST) % SIZE;
    }

    return (sum + I*CONST1 + I*I*CONST2)%SIZE;
}

int HtCreateItem(CC_HASH_ITEM** HashItem)
{
    CC_HASH_ITEM* hI = NULL;

    CC_UNREFERENCED_PARAMETER(HashItem);

    if (NULL == HashItem)
    {
        return -1;
    }

    hI = (CC_HASH_ITEM*)malloc(sizeof(CC_HASH_ITEM));
    if (NULL == hI)
    {
        return -1;
    }

    memset(hI, 0, sizeof(*hI));

    hI->Value = 0;
    hI->Key = NULL;
    hI->Status = 0;

    *HashItem = hI;

    return 0;
}

int HtCreate(CC_HASH_TABLE **HashTable)
{
    CC_HASH_TABLE* ht = NULL;

    CC_UNREFERENCED_PARAMETER(HashTable);

    if (NULL == HashTable)
    {
        return -1;
    }

    ht = (CC_HASH_TABLE*)malloc(sizeof(CC_HASH_TABLE));
    if (NULL == ht)
    {
        return -1;
    }

    memset(ht, 0, sizeof(*ht));

    ht->Count = 0;
    ht->Array = (CC_HASH_ITEM**)malloc(sizeof(CC_HASH_ITEM*)*SIZE);
    if (NULL == ht->Array)
    {
        free(ht);
        return -1;
    }

    for (int i = 0; i < SIZE; i++)
    {
        if (-1 == HtCreateItem(&ht->Array[i]))
        {
            return -1;
        }
    }

    *HashTable = ht;

    return 0;
}

int HtSetItem(CC_HASH_ITEM* HashItem, char* Key, int Value)
{
    CC_UNREFERENCED_PARAMETER(HashItem);

    if (NULL == HashItem)
    {
        return -1;
    }

    HashItem->Value = Value;
    int len = HtStrlen(Key);
    if (-1 == len)
    {
        return -1;
    }
    HashItem->Key = (char*)malloc(sizeof(char) * (len + 1));
    int retVal = HtStrcpy(&HashItem->Key, Key);
    if (-1 == retVal)
    {
        return -1;
    }
    HashItem->Status = 1;

    return 0;
}

int HtDestroyItem(CC_HASH_ITEM** HashItem)
{
    CC_UNREFERENCED_PARAMETER(HashItem);
    CC_HASH_ITEM* ht = *HashItem;

    if (NULL == HashItem)
    {
        return -1;
    }
    if (NULL != ht->Key)
    {
        //free(ht->Key);
    }
    free(ht);
    *HashItem = NULL;

    return 0;
}

int HtDestroy(CC_HASH_TABLE **HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_HASH_TABLE* ht = *HashTable;

    if (NULL == HashTable)
    {
        return -1;
    }

    for (int i = 0; i < SIZE; i++)
    {
        if (-1 == HtDestroyItem(&ht->Array[i]))
        {
            return -1;
        }
    }
    free(ht->Array);
    free(ht);

    *HashTable = NULL;

    return 0;
}

int HtSetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int Value)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == HashTable)
    {
        return -1;
    }

    if (NULL == Key)
    {
        return -1;
    }

    int i = 0;
    do {
        int pos = HtHashFunction(Key, i);
        if (1 != HashTable->Array[pos]->Status)
        {
            if (-1 == HtSetItem(HashTable->Array[pos], Key, Value))
            {
                return -1;
            }
            HashTable->Count++;
            return 0;
        }
        else
        {
            if (0 >= HtStrcmp(Key, HashTable->Array[pos]->Key))
            {
                return -1;
            }
            else
            {
                i++;
            }
        }
    } while (i < SIZE);

    return -1;
}

int HtGetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int *Value)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }
    if (NULL == Value)
    {
        return -1;
    }

    int i = 0;
    do {
        int pos = HtHashFunction(Key, i);
        if (0 == HashTable->Array[pos]->Status)
        {
            return -1;
        }
        else
        {
            int cmp = HtStrcmp(Key, HashTable->Array[pos]->Key);
            if (-1 == cmp)
            {
                return -1;
            }
            if (1 == HashTable->Array[pos]->Status && 0 == cmp)
            {
                *Value = HashTable->Array[pos]->Value;
                return 0;
            }
            else
            {
                i++;
            }
        }
    } while (i < SIZE);

    return -1;
}

int HtRemoveKey(CC_HASH_TABLE *HashTable, char *Key)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);

    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    int i = 0;
    do {
        int pos = HtHashFunction(Key, i);
        if (0 ==  HashTable->Array[pos]->Status)
        {
            return -1;
        }
        else
        {
            int cmp = HtStrcmp(Key, HashTable->Array[pos]->Key);
            if (-1 == cmp)
            {
                return -1;
            }
            if (1 == HashTable->Array[pos]->Status && 0 == cmp)
            {
                HashTable->Array[pos]->Status = 2;
                free(HashTable->Array[pos]->Key);
                HashTable->Array[pos]->Key = NULL;
                HashTable->Count--;
                return 0;
            }
            else
            {
                i++;
            }
        }
    } while (i < SIZE);

    return -1;
}

int HtHasKey(CC_HASH_TABLE *HashTable, char *Key)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    CC_UNREFERENCED_PARAMETER(Key);

    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    int i = 0;
    do {
        int pos = HtHashFunction(Key, i);
        if (0 == HashTable->Array[pos]->Status)
        {
            return 0;
        }
        else
        {
            int cmp = HtStrcmp(Key, HashTable->Array[pos]->Key);
            if (-1 == cmp)
            {
                return -1;
            }
            if (1 == HashTable->Array[pos]->Status && 0 == cmp)
            {
                return 1;
            }
            else
            {
                i++;
            }
        }
    } while (i < SIZE);

    return 0;
}

int HtGetFirstKey(CC_HASH_TABLE* HashTable, CC_HASH_TABLE_ITERATOR **Iterator, char **Key)
{
    CC_HASH_TABLE_ITERATOR *iterator = NULL;

    CC_UNREFERENCED_PARAMETER(Key);

    if (NULL == HashTable)
    {
        return -1;
    }
    if (NULL == Iterator)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }

    iterator = (CC_HASH_TABLE_ITERATOR*)malloc(sizeof(CC_HASH_TABLE_ITERATOR));
    if (NULL == iterator)
    {
        return -1;
    }

    memset(iterator, 0, sizeof(*iterator));

    iterator->HashTable = HashTable;
    iterator->Index = -1;

    *Iterator = iterator;

    int count = HtGetKeyCount(HashTable);
    if (-1 == count)
    {
        return -1;
    }
    if (0 == count)
    {
        return -2;
    }

    int i = 0;
    do
    {
        if (1 == HashTable->Array[i]->Status)
        {
            iterator->Index = i;
            int retVal = HtStrcpy(Key, HashTable->Array[i]->Key);
            if (-1 == retVal)
            {
                return -1;
            }
            return 0;
        }
        i++;
    } while (i < SIZE);

    return -2;
}

int HtGetNextKey(CC_HASH_TABLE_ITERATOR *Iterator, char **Key)
{
    CC_UNREFERENCED_PARAMETER(Key);
    CC_UNREFERENCED_PARAMETER(Iterator);

    if (NULL == Iterator)
    {
        return -1;
    }
    if (NULL == Key)
    {
        return -1;
    }
    int count = HtGetKeyCount(Iterator->HashTable);
    if (-1 == count)
    {
        return -1;
    }
    if (0 == count)
    {
        return -2;
    }

    int i = Iterator->Index + 1;
    do
    {
        if (1 == Iterator->HashTable->Array[i]->Status)
        {
            Iterator->Index = i;
            int retVal = HtStrcpy(Key, Iterator->HashTable->Array[i]->Key);
            if (-1 == retVal)
            {
                return -1;
            }
            return 0;
        }
        i++;
    } while (i < SIZE);

    i = 0;
    do
    {
        if (1 == Iterator->HashTable->Array[i]->Status)
        {
            Iterator->Index = i;
            int retVal = HtStrcpy(Key, Iterator->HashTable->Array[i]->Key);
            if (-1 == retVal)
            {
                return -1;
            }
            return 0;
        }
        i++;
    } while (i < Iterator->Index);

    return -2;
}

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR **Iterator)
{
    CC_UNREFERENCED_PARAMETER(Iterator);
    CC_HASH_TABLE_ITERATOR* it = *Iterator;

    if (NULL == Iterator)
    {
        return -1;
    }

    free(it);

    *Iterator = NULL;
    return 0;
}

int HtClear(CC_HASH_TABLE *HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    
    if (NULL == HashTable)
    {
        return -1;
    }

    int i = 0;
    do 
    {
        HashTable->Array[i]->Status = 0;
        free(HashTable->Array[i]->Key);
        HashTable->Array[i]->Key = NULL;
        i++;
    } while (i < SIZE);
    HashTable->Count = 0;
    return 0;
}

int HtGetKeyCount(CC_HASH_TABLE *HashTable)
{
    CC_UNREFERENCED_PARAMETER(HashTable);
    if (NULL == HashTable)
    {
        return -1;
    }

    return HashTable->Count;
}
