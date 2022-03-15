#include "cctree.h"
#include "common.h"
#include "string.h"

int NodeCreate(CC_NODE **Node, int Value)
{
    CC_NODE* nd = *Node;
    CC_UNREFERENCED_PARAMETER(Node);

    if (NULL == Node)
    {
        return -1;
    }

    nd = (CC_NODE*)malloc(sizeof(CC_NODE));
    if (NULL == nd)
    {
        return -1;
    }

    memset(nd, 0, sizeof(*nd));

    nd->Left = NULL;
    nd->Right = NULL;
    nd->Parent = NULL;
    nd->Height = 0;
    nd->Key = Value;

    *Node = nd;

    return 0;
}

int NodeDestroy(CC_NODE** Node)
{
    CC_NODE* nd = *Node;
    CC_UNREFERENCED_PARAMETER(Node);

    if (NULL == Node)
    {
        return -1;
    }

    if (NULL != nd->Left)
    {
        if (-1 == NodeDestroy(&nd->Left))
        {
            return -1;
        }
    }

    if (NULL != nd->Right)
    {
        if (-1 == NodeDestroy(&nd->Right))
        {
            return -1;
        }
    }

    free(nd);

    *Node = NULL;

    return 0;
}

int TreeCreate(CC_TREE **Tree)
{
    CC_TREE* tr = *Tree;
    CC_UNREFERENCED_PARAMETER(Tree);

    if (NULL == Tree)
    {
        return -1;
    }

    if (NULL != *Tree)
    {
        if (-1 == TreeDestroy(Tree))
        {
            return -1;
        }
    }

    tr = (CC_TREE*)malloc(sizeof(CC_TREE));
    if (NULL == tr)
    {
        return -1;
    }

    memset(tr, 0, sizeof(*tr));

    tr->Root = NULL;
    tr->Count = 0;

    *Tree = tr;

    return 0;
}

int TreeDestroy(CC_TREE **Tree)
{
    CC_TREE* tr = *Tree;
    CC_UNREFERENCED_PARAMETER(Tree);

    if (NULL == Tree)
    {
        return -1;
    }

    if (NULL != tr->Root)
    {
        if (-1 == NodeDestroy(&tr->Root))
        {
            return -1;
        }
    }
    
    *Tree = NULL;

    return 0;
}

int Max2(int x, int y)
{
    if (x > y)
    {
        return x;
    }
    return y;
}

int NodeHeight(CC_NODE* x)
{
    if (NULL == x)
    {
        return -1;
    }
    return x->Height;
}

CC_NODE* NodeLeftRotate(CC_NODE* X)
{
    CC_NODE* rh = X->Right;
    CC_NODE* rhLf = rh->Left;

    rh->Left = X;
    X->Right = rhLf;
    X->Parent = rh;
    if (NULL != rhLf)
    {
        rhLf->Parent = X;
    }

    X->Height = Max2(NodeHeight(X->Left), NodeHeight(X->Right)) + 1;
    rh->Height = Max2(NodeHeight(rh->Left), NodeHeight(rh->Right)) + 1;

    return rh;
}

CC_NODE* NodeRightRotate(CC_NODE* Y)
{
    CC_NODE* lf = Y->Left;
    CC_NODE* lfRh = lf->Right;

    lf->Right = Y;
    Y->Left = lfRh;
    Y->Parent = lf;
    if (NULL != lfRh)
    {
        lfRh->Parent = Y;
    }

    Y->Height = Max2(NodeHeight(Y->Left), NodeHeight(Y->Right)) + 1;
    lf->Height = Max2(NodeHeight(lf->Left), NodeHeight(lf->Right)) + 1;

    return lf;
}

CC_NODE* NodeLeftRightRotate(CC_NODE* X)
{
    CC_NODE* lf = NodeLeftRotate(X->Left);
    lf->Parent = X;
    X->Left = lf;
    return NodeRightRotate(X);
}

CC_NODE* NodeRightLeftRotate(CC_NODE* X)
{
    CC_NODE* rh = NodeRightRotate(X->Right);
    rh->Parent = X;
    X->Right = rh;
    return NodeLeftRotate(X);
}

int TreeInsert(CC_TREE *Tree, int Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Tree)
    {
        return -1;
    }

    Tree->Count++;

    if (Tree->Root == NULL)
    {
        CC_NODE *node;
        if (-1 == NodeCreate(&node, Value))
        {
            return -1;
        }
        Tree->Root = node;
    }
    else
    {
        int found = 0;
        CC_NODE* newNode;
        if (-1 == NodeCreate(&newNode, Value))
        {
            return -1;
        }
        CC_NODE* node = Tree->Root;
        while (!found)
        {
            if (Value <= node->Key)
            {
                if (NULL == node->Left)
                {
                    newNode->Parent = node;
                    node->Left = newNode;
                    found = 1;
                }
                else
                {
                    node = node->Left;
                }
            }
            else
            {
                if (NULL == node->Right)
                {
                    newNode->Parent = node;
                    node->Right = newNode;
                    found = 1;
                }
                else
                {
                    node = node->Right;
                }
            }
        }

        int balP = 0, balNode = 0;
        node = newNode;
        CC_NODE* p = node->Parent;
        CC_NODE* pp = NULL;
        CC_NODE* newRoot = NULL;
        while (NULL != p)
        {
            balP = NodeHeight(p->Left) - NodeHeight(p->Right);
            balNode = NodeHeight(node->Left) - NodeHeight(node->Right);
            if (node == p->Right)
            {
                if (-2 == balP)
                {
                    pp = p->Parent;
                    if (1 == balNode)
                    {
                        newRoot = NodeRightLeftRotate(p);
                    }
                    else
                    {
                        newRoot = NodeLeftRotate(p);
                    }
                }
                else
                {
                    if (balP == 0)
                    {
                        break;
                    }
                    else
                    {   //balP == -1
                        p->Height++;
                        node = p;
                        p = node->Parent;
                        continue;
                    }
                }
            }
            else
            {
                if (balP == 2)
                {
                    pp = p->Parent;
                    if (-1 == balNode)
                    {
                        newRoot = NodeLeftRightRotate(p);
                    }
                    else
                    {
                        newRoot = NodeRightRotate(p);
                    }
                }
                else
                {
                    if (balP == 0)
                    {
                        break;
                    }
                    else
                    {   //balP == 1
                        p->Height++;
                        node = p;
                        p = node->Parent;
                        continue;
                    }
                }
            }
            newRoot->Parent = pp;
            if (NULL != pp)
            {
                if (p == pp->Left)
                {
                    pp->Left = newRoot;
                }
                else
                {
                    pp->Right = newRoot;
                }
            }
            else
            {
                Tree->Root = newRoot;
            }
            break;
        }
    }
    return 0;
}

CC_NODE* TreeMinimum(CC_NODE* x)
{
    while (NULL != x->Left)
    {
        x = x->Left;
    }
    return x;
}

CC_NODE* TreeSuccesor(CC_NODE* x)
{
    if (NULL != x->Right)
    {
        return TreeMinimum(x->Right);
    }
    CC_NODE* y = x->Parent;
    while (NULL != y && x == y->Right)
    {
        x = y;
        y = y->Parent;
    }
    return y;
}

void SubtreeShift(CC_TREE* Tree, CC_NODE* u, CC_NODE* v)
{
    if (NULL == u->Parent)
    {
        Tree->Root = v;
    }
    else if (u == u->Parent->Left)
    {
        u->Parent->Left = v;
    }
    else
    {
        u->Parent->Right = v;
    }
    if (NULL != v)
    {
        v->Parent = u->Parent;
    }
}

void NodeDelete(CC_TREE* Tree, CC_NODE* node)
{
    CC_NODE* N, * NP;
    int dir = -1;
    if (NULL == node->Left)
    {
        NP = node->Parent;
        if (node == NP->Left)
        {
            dir = 0;
        }
        else
        {
            dir = 1;
        }
        SubtreeShift(Tree, node, node->Right);
    }
    else if (NULL == node->Right)
    {
        NP = node->Parent;
        if (node == NP->Left)
        {
            dir = 0;
        }
        else
        {
            dir = 1;
        }
        SubtreeShift(Tree, node, node->Left);
    }
    else
    {
        CC_NODE* y = TreeSuccesor(node);
        NP = y->Parent;
        if (y == NP->Left)
        {
            dir = 0;
        }
        else
        {
            dir = 1;
        }
        if (node != y->Parent)
        {
            SubtreeShift(Tree, y, y->Right);
            y->Right = node->Right;
            y->Right->Parent = y;
        }
        SubtreeShift(Tree, node, y);
        y->Left = node->Left;
        y->Left->Parent = y;
        y->Height = Max2(NodeHeight(y->Left), NodeHeight(y->Right)) + 1;
    }
    if (0 == dir)
    {
        N = NP->Left;
    }
    else
    {
        N = NP->Right;
    }
    CC_NODE* X = NP;
    CC_NODE* G, * Z;
    int balZ = 0, balX = 0;
    while (NULL != X)
    {
        G = X->Parent;
        X->Height = Max2(NodeHeight(X->Left), NodeHeight(X->Right)) + 1;
        balX = NodeHeight(X->Left) - NodeHeight(X->Right);
        if (1!=dir && N == X->Left)
        {
            if (0 == dir)
            {
                dir = -1;
            }
            if (balX == -2)
            {
                Z = X->Right;
                if (NULL != Z)
                {
                    balZ = NodeHeight(Z->Left) - NodeHeight(Z->Right);
                }
                else
                {
                    balZ = -1;
                }
                if (balZ > 0)
                {
                    N = NodeRightLeftRotate(X);
                }
                else
                {
                    N = NodeLeftRotate(X);
                }
            }
            else
            {
                if (balX == -1)
                {
                    break;
                }
                else
                {
                    N = X;
                    X = G;
                    continue;
                }
            }
        }
        else
        {
            if (1 == dir)
            {
                dir = -1;
            }
            if (balX == 2)
            {
                Z = X->Left;
                if (NULL != Z)
                {
                    balZ = NodeHeight(Z->Left) - NodeHeight(Z->Right);
                }
                else
                {
                    balZ = 1;
                }
                
                if (balZ < 0)
                {
                    N = NodeLeftRightRotate(X);
                }
                else
                {
                    N = NodeRightRotate(X);
                }
            }
            else
            {
                if (balX == 1)
                {
                    break;
                }
                else
                {
                    N = X;
                    X = G;
                    continue;
                }
            }
        }
        N->Parent = G;
        if (NULL != G)
        {
            if (X == G->Left)
            {
                G->Left = N;
            }
            else
            {
                G->Right = N;
            }
        }
        else
        {
            Tree->Root = N;
        }
        if (0 == balZ)
        {
            break;
        }
        X = G;
    }
}

int TreeRemove(CC_TREE *Tree, int Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Tree)
    {
        return -1;
    }
    if (0 >= TreeContains(Tree, Value))
    {
        return -1;
    }

    CC_NODE* node = Tree->Root;
    CC_NODE* delNode;
    while (NULL != node)
    {
        if (Value == node->Key)
        {
            if (Tree->Root == node)
            {
                NodeDelete(Tree, node);
                node = Tree->Root;
            }
            else
            {
                delNode = node;
                node = node->Parent;
                NodeDelete(Tree, delNode);
            }
        }
        else
        {
            if (Value < node->Key)
            {
                node = node->Left;
            }
            else
            {
                node = node->Right;
            }
        }
    }
    return 0;
}

int TreeContains(CC_TREE *Tree, int Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Value);
    
    if (NULL == Tree)
    {
        return -1;
    }

    CC_NODE* node = Tree->Root;

    while (NULL != node)
    {
        if (Value == node->Key)
        {
            return 1;
        }
        else if (Value < node->Key)
        {
            node = node->Left;
        }
        else if (Value > node->Key)
        {
            node = node->Right;
        }
    }

    return 0;
}

int TreeGetCount(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if (NULL == Tree)
    {
        return -1;
    }

    return Tree->Count;
}

int TreeGetHeight(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if (NULL == Tree)
    {
        return -1;
    }

    return Tree->Root->Height;
}

int TreeClear(CC_TREE *Tree)
{
    CC_UNREFERENCED_PARAMETER(Tree);

    if (NULL == Tree)
    {
        return -1;
    }

    if (NULL != Tree->Root)
    {
        if (-1 == NodeDestroy(&Tree->Root))
        {
            return -1;
        }
    }
    
    Tree->Count = 0;

    return 0;
}

void Preorder(CC_NODE* Root, int* Index, int* Value)
{
    if (0 == *Index)
    {
        *Value = Root->Key;
    }
    (*Index)--;
    if (-1 != *Index)
    {
        if (NULL != Root->Left)
        {
            Preorder(Root->Left, Index, Value);
        }
    }
    if (-1 != *Index)
    {
        if (NULL != Root->Right)
        {
            Preorder(Root->Right, Index, Value);
        }
    }
}

int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Tree)
    {
        return -1;
    }
    if (NULL == Value)
    {
        return -1;
    }

    if (Index >= Tree->Count)
    {
        return -1;
    }

    int i = Index;
    Preorder(Tree->Root, &i, Value);
    return 0;
}

void Inorder(CC_NODE* Root, int* Index, int* Value)
{
    if (-1 != *Index)
    {
        if (NULL != Root->Left)
        {
            Inorder(Root->Left, Index, Value);
        }
    }
    if (0 == *Index)
    {
        *Value = Root->Key;
    }
    (*Index)--;
    if (-1 != *Index)
    {
        if (NULL != Root->Right)
        {
            Inorder(Root->Right, Index, Value);
        }
    }
}

int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Tree)
    {
        return -1;
    }
    if (NULL == Value)
    {
        return -1;
    }

    if (Index >= Tree->Count)
    {
        return -1;
    }

    int i = Index;
    Inorder(Tree->Root, &i, Value);
    return 0;
}

void Postorder(CC_NODE* Root, int* Index, int* Value)
{
    if (-1 != *Index)
    {
        if (NULL != Root->Left)
        {
            Postorder(Root->Left, Index, Value);
        }
    }
    if (-1 != *Index)
    {
        if (NULL != Root->Right)
        {
            Postorder(Root->Right, Index, Value);
        }
    }
    if (0 == *Index)
    {
        *Value = Root->Key;
    }
    (*Index)--;
}

int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value)
{
    CC_UNREFERENCED_PARAMETER(Tree);
    CC_UNREFERENCED_PARAMETER(Index);
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Tree)
    {
        return -1;
    }
    if (NULL == Value)
    {
        return -1;
    }

    if (Index >= Tree->Count)
    {
        return -1;
    }

    int i = Index;
    Postorder(Tree->Root, &i, Value);
    return 0;
}

