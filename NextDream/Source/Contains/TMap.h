#ifndef __BLADE_CONTAINS_TMAP_H__
#define __BLADE_CONTAINS_TMAP_H__

#include <Utility.h>
#include <new>

namespace BladeEngine
{
    namespace Contains
    {
        template<typename Type>
        struct DefaultCompareFunc
        {
            static int32 Compare(const Type& lh, const Type& rh)
            {
                return (lh < rh ? 1 : (rh < lh ? -1 : 0));
            }
        };
    }

    template<typename KeyType, typename ValueType, typename CompareFunc = Contains::DefaultCompareFunc<KeyType>>
    class TMap
    {
    private:
        const uint32 Black = 0;
        const uint32 Red = 1;

        struct _RBTree_Node
        {
            uint32 Color;
            _RBTree_Node* ParentNode;
            _RBTree_Node* LeftNode;
            _RBTree_Node* RightNode;
            KeyType Key;
            ValueType Value;
        };

    private:
        _RBTree_Node* m_Root;

    private:
        _RBTree_Node* _RBTreeCreateNode(const KeyType& inKey, const ValueType& inValue)
        {
            _RBTree_Node* node = (_RBTree_Node*)SystemMalloc::GetInstance().Alloc(sizeof(_RBTree_Node));
            BladeConstruct(node, _RBTree_Node);

            node->Color = Red;
            node->LeftNode = NULL;
            node->RightNode = NULL;
            node->ParentNode = NULL;

            return node;
        }

        void _RBTreeDestoryNode(_RBTree_Node* inNode)
        {
            SystemMalloc::GetInstance().Free(eraseNode);
        }

        void _RBTreeRotateLeft(_RBTree_Node* inNode, _RBTree_Node** inRoot)
        {
            BladeAssert(inNode->RightNode != NULL);

            _RBTree_Node* parentNode = inNode->ParentNode;
            _RBTree_Node* rightNode = inNode->RightNode;
            _RBTree_Node* rightLeftNode = inNode->RightNode->LeftNode;

            if ((rightNode->ParentNode = parentNode) == NULL)
            {
                *inRoot = rightNode;
            }
            else
            {
                if (parentNode->LeftNode == inNode)
                {
                    parentNode->LeftNode = rightNode;
                }
                else
                {
                    parentNode->RightNode = rightNode;
                }
            }

            inNode->ParentNode = rightNode;
            rightNode->LeftNode = inNode;

            if (rightLeftNode != NULL)
            {
                rightLeftNode->ParentNode = inNode;
            }
            inNode->RightNode = rightLeftNode;
        }

        void _RBTreeRotateRight(_RBTree_Node* inNode, _RBTree_Node** inRoot)
        {
            BladeAssert(inNode->LeftNode != NULL);

            _RBTree_Node* parentNode = inNode->ParentNode;
            _RBTree_Node* leftNode = inNode->LeftNode;
            _RBTree_Node* leftRightNode = inNode->LeftNode->RightNode;

            if ((leftNode->ParentNode = parentNode) == NULL)
            {
                *inRoot = leftNode;
            }
            else
            {
                if (parentNode->LeftNode == inNode)
                {
                    parentNode->LeftNode = leftNode;
                }
                else
                {
                    parentNode->RightNode = leftNode;
                }
            }

            inNode->ParentNode = leftNode;
            leftNode->RightNode = inNode;

            if (leftRightNode != NULL)
            {
                leftRightNode->ParentNode = inNode;
            }
            inNode->LeftNode = leftRightNode;
        }

        _RBTree_Node* _RBTreeSreachNode(const KeyType& inKey, _RBTree_Node* inRoot)
        {
            _RBTree_Node* node = inRoot;
            int32 result = 0;

            while (node != NULL)
            {
                int32 result = CompareFunc::Compare(node->Key, inKey);
                if (result > 0)
                {
                    node = node->RightNode;
                }
                else if (result < 0)
                {
                    node = node->LeftNode;
                }
                else
                {
                    break;
                }
            }

            return node;
        }

        int32 _RBTreeInsert_SreachParentNode(KeyType inKey, _RBTree_Node* inRoot, _RBTree_Node** inParent)
        {
            _RBTree_Node* node = inRoot;
            _RBTree_Node* insertParentNode = NULL;
            int32 result = 0;

            while (node != NULL)
            {
                insertParentNode = node;
                result = CompareFunc::Compare(node->Key, inKey);

                if (result > 0)
                {
                    node = node->RightNode;
                }
                else if (result < 0)
                {
                    node = node->LeftNode;
                }
                else
                {
                    BladeAssert(0);
                }
            }

            if (inParent != nullptr)
            {
                *inParent = insertParentNode;
            }
            return result;
        }

        _RBTree_Node* _RBTreeInsert(const KeyType& inKey, const ValueType& inValue, _RBTree_Node** inRoot)
        {
            _RBTree_Node* insertNode = _RBTreeCreateNode(inKey, inValue);


            insertNode->Key = inKey;
            insertNode->Value = inValue;
            insertNode->ParentNode = NULL;
            insertNode->LeftNode = NULL;
            insertNode->RightNode = NULL;
            insertNode->Color = Red;

            _RBTree_Node* parentNode = NULL;
            int32 result = _RBTreeInsert_SreachParentNode(inKey, *inRoot, &parentNode);
            if (parentNode == NULL)
            {
                *inRoot = insertNode;
                return insertNode;
            }

            if (result > 0)
            {
                parentNode->RightNode = insertNode;
            }
            else
            {
                parentNode->LeftNode = insertNode;
            }
            insertNode->ParentNode = parentNode;

            _RBTreeInsertBalance(insertNode, inRoot);

            return insertNode;
        }

        void _RBTreeInsertBalance(_RBTree_Node* inNode, _RBTree_Node** inRoot)
        {
            if (inNode->ParentNode == NULL || inNode->ParentNode->ParentNode == NULL)
            {
                return;
            }

            _RBTree_Node* node = inNode;
            _RBTree_Node* parent = node->ParentNode;
            _RBTree_Node* grandParent = parent->ParentNode;
            _RBTree_Node* uncle = grandParent->LeftNode == parent ? grandParent->RightNode : grandParent->LeftNode;

            while (parent->Color != Black)
            {
                if (uncle == NULL || uncle->Color == Black)
                {
                    if (grandParent->LeftNode == parent)
                    {
                        if (parent->RightNode == node)
                        {
                            _RBTreeRotateLeft(parent, inRoot);
                            _RBTree_Node* temp = node;
                            node = parent;
                            parent = temp;
                        }

                        grandParent->Color = Red;
                        parent->Color = Black;
                        _RBTreeRotateRight(grandParent, inRoot);
                    }
                    else
                    {
                        if (parent->LeftNode == node)
                        {
                            _RBTreeRotateRight(parent, inRoot);
                            _RBTree_Node* temp = node;
                            node = parent;
                            parent = temp;
                        }

                        grandParent->Color = Red;
                        parent->Color = Black;
                        _RBTreeRotateLeft(grandParent, inRoot);
                    }
                }
                else
                {
                    grandParent->Color = Red;
                    parent->Color = Black;
                    uncle->Color = Black;

                    node = grandParent;
                }
            }

            parent->Color = Black;
        }

        void _RBTreeErase(const KeyType& inKey, _RBTree_Node** inRoot)
        {
            _RBTree_Node* eraseNode = _RBTreeSreachNode(inKey, *inRoot);
            if (eraseNode == NULL)
            {
                return;
            }

            int32 balanceColor = Red;
            _RBTree_Node* balanceNode = NULL;
            _RBTree_Node* balanceParentNode = NULL;

            if (eraseNode->LeftNode != NULL && eraseNode->RightNode != NULL)
            {
                // greater than erase node but less than other children in erase node
                _RBTree_Node* replaceNode = eraseNode->RightNode;
                while (replaceNode->LeftNode != NULL)
                {
                    replaceNode = replaceNode->LeftNode;
                }

                balanceColor = replaceNode->Color;
                balanceNode = replaceNode->RightNode;

                if (replaceNode != eraseNode->RightNode)
                {
                    // swap replace node and erase node
                    if (replaceNode->ParentNode->LeftNode == replaceNode)
                    {
                        replaceNode->ParentNode->LeftNode = replaceNode->RightNode;
                    }
                    else
                    {
                        replaceNode->ParentNode->RightNode = replaceNode->RightNode;
                    }

                    if (replaceNode->RightNode != NULL)
                    {
                        replaceNode->RightNode->ParentNode = replaceNode->ParentNode;
                    }

                    replaceNode->Color = eraseNode->Color;
                    replaceNode->ParentNode = eraseNode->ParentNode;
                    replaceNode->LeftNode = eraseNode->LeftNode;
                    replaceNode->RightNode = eraseNode->RightNode;

                    eraseNode->LeftNode->ParentNode = replaceNode;
                    eraseNode->RightNode->ParentNode = replaceNode;

                    balanceParentNode = eraseNode->RightNode;
                }
                else
                {
                    replaceNode->Color = eraseNode->Color;
                    replaceNode->ParentNode = eraseNode->ParentNode;
                    replaceNode->LeftNode = eraseNode->LeftNode;

                    eraseNode->LeftNode->ParentNode = replaceNode;

                    balanceParentNode = eraseNode->RightNode;
                }

                if (eraseNode == *inRoot)
                {
                    *inRoot = replaceNode;
                }
                else
                {
                    if (eraseNode->ParentNode->LeftNode == eraseNode)
                    {
                        eraseNode->ParentNode->LeftNode = replaceNode;
                    }
                    else
                    {
                        eraseNode->ParentNode->RightNode = replaceNode;
                    }
                }
            }
            else
            {
                balanceColor = eraseNode->Color;

                if (eraseNode->RightNode != NULL)
                {
                    balanceNode = eraseNode->RightNode;
                }
                else
                {
                    balanceNode = eraseNode->LeftNode;
                }

                if (balanceNode != NULL)
                {
                    balanceNode->ParentNode = eraseNode->ParentNode;
                }
                balanceParentNode = eraseNode->ParentNode;

                if (eraseNode == *inRoot)
                {
                    *inRoot = balanceNode;
                }
                else
                {
                    if (eraseNode->ParentNode->LeftNode == eraseNode)
                    {
                        eraseNode->ParentNode->LeftNode = balanceNode;
                    }
                    else
                    {
                        eraseNode->ParentNode->RightNode = balanceNode;
                    }
                }
            }

            BladeDestruct(eraseNode, _RBTree_Node);
            SystemMalloc::GetInstance().Free(eraseNode);

            if (balanceColor == Black)
            {
                _RBTreeEraseBalance(balanceNode, balanceParentNode, inRoot);
            }
        }

        void _RBTreeEraseBalance(_RBTree_Node* inNode, _RBTree_Node* inParent, _RBTree_Node** inRoot)
        {
            if (inParent == NULL)
            {
                return;
            }

            _RBTree_Node* node = inNode;
            _RBTree_Node* parent = inParent;
            _RBTree_Node* brother = parent->LeftNode == inNode ? parent->RightNode : parent->LeftNode;

            while (node != NULL && node->Color == Black && node != *inRoot)
            {
                if (parent->LeftNode == node)
                {
                    // change       P(b)       to          B(b)
                    //          N(b)    B(r)           P(r)
                    //                              B(b)    BL(b)
                    // change case1 to case2\3\4 
                    if (brother->Color == Red)
                    {
                        BladeAssert(parent->Color == Black);

                        // make brother's color to black
                        parent->Color = Red;
                        brother->Color = Black;
                        _RBTreeRotateLeft(parent, inRoot);
                        // update brother
                        brother = parent->LeftNode;
                    }

                    BladeAssert(brother->Color == Black);

                    // brother must bleck at this moment
                    // change       P(r/b)          to          P(b)
                    //          N(b)    B(b)                N(b)    B(r)
                    //              BL(b)   BR(b)               BL(b)   BR(b)
                    if ((brother->LeftNode == NULL || brother->LeftNode->Color) &&
                        (brother->RightNode == NULL || brother->RightNode->Color))
                    {
                        brother->Color = Red;

                        if (parent->Color == Red)
                        {
                            // node's subtree the number of black node is equal than which before erase node
                            parent->Color = Black;
                            break;
                        }
                        else
                        {
                            // the number of parent's subtree decrease one black node but is balanced. set parent as node.
                            parent->Color = Black;
                            node = parent;
                            continue;
                        }

                    }
                    else
                    {
                        // P's color must black
                        //                  P(?,?)
                        //      N(X-1,b)                B(X,b)
                        //  ...     ...         BL(X-1,?)       BR(X-1,?)
                        if (brother->RightNode != NULL && brother->RightNode->Color == Black)
                        {
                            // P's color must black. if color of brother's left node isn't Red
                            //                  P(?,?)
                            //      N(X-1,b)                B(X,b)
                            //  ...     ...         BL(X-1,r)           BR(X-1,b)
                            //             BLL(x-1,?)      BLR(x-1,?)
                            // change to 
                            //                  P(?,?)
                            //      N(X-1,b)                BL(X,b)
                            //  ...     ...         BLL(X-1,?)       B(X-1,r)
                            //                                 BLR(x-1,?)    BR(X-1,b)
                            brother->Color = Red;
                            brother->RightNode->Color = Black;
                            // left rotate
                            _RBTreeRotateRight(brother, inRoot);
                            // update brother node
                            brother = parent->RightNode;
                        }

                        BladeAssert(brother->Color == Black);
                        BladeAssert(brother->RightNode != NULL);
                        BladeAssert(brother->RightNode->Color == Red);
                        // P's color must black. if color of brother's left node is red
                        //                  P(X+1,b)                            or                      P(X,r)                            
                        //      N(X-1,b)                B(X,b)                                N(X-1,b)                B(X,b)
                        //  ...     ...         BL(X-1,b)       BR(X-1,r)                ...     ...         BL(X-1,b)       BR(X-1,r)
                        // change to                                                 change to        
                        //                  B(X+1,b)                                                    B(X,r)    
                        //          P(X,b)                BR(X,b)                            P(X,b)                BR(X,b)
                        //  N(X-1,b)     B(X-1,r)                                     N(X-1,b)     B(X-1,r)    
                        // 
                        //  
                        brother->Color = parent->Color;
                        brother->RightNode->Color = Red;
                        // node's subtree the number of black node is equal than which before erase node
                        _RBTreeRotateLeft(parent, inRoot);
                        break;
                    }
                }
                else
                {
                    if (brother->Color == Red)
                    {
                        BladeAssert(parent->Color == Black);
                        // make brother's color to black
                        parent->Color = Red;
                        brother->Color = Black;
                        _RBTreeRotateRight(parent, inRoot);
                        // update brother
                        brother = parent->LeftNode;
                    }

                    BladeAssert(brother->Color == Black);
                    if ((brother->LeftNode == NULL || brother->LeftNode->Color == Black) &&
                        (brother->RightNode == NULL || brother->RightNode->Color == Black))
                    {
                        brother->Color = Red;
                        if (parent->Color == Red)
                        {
                            parent->Color = Black;
                            break;
                        }
                        else
                        {
                            parent->Color = Black;
                            node = parent;
                            continue;
                        }
                    }
                    else
                    {
                        if (brother->LeftNode == NULL || brother->LeftNode->Color == Black)
                        {
                            // make color of brother's left node to red
                            brother->Color = Red;
                            brother->RightNode->Color = Black;
                            _RBTreeRotateLeft(brother, inRoot);
                            // updete brother
                            brother = parent->LeftNode;
                        }

                        BladeAssert(brother->Color == Black);
                        BladeAssert(brother->LeftNode != NULL);
                        BladeAssert(brother->LeftNode->Color == Red);

                        // balance
                        parent->Color = Black;
                        brother->Color = parent->Color;
                        brother->LeftNode->Color = Black;
                        _RBTreeRotateRight(parent, inRoot);
                        break;
                    }
                }
            }
        }

    public:
        TMap() : m_Root(NULL)
        {}

        bool TryGetValue(const KeyType& inKey, ValueType* outValue)
        {
            _RBTree_Node* node = _RBTreeSreachNode(inKey, &m_Root);
            if (node == NULL)
            {
                return false;
            }

            if (outValue != NULL)
            {
                *outValue = node->Value;
            }
            return true;
        }

        void Insert(const KeyType& inKey, const ValueType& inValue)
        {
            _RBTreeInsert(inKey, inValue, &m_Root);
        }

        void Erase(const KeyType& inKey)
        {
            _RBTreeErase(inKey, &m_Root);
        }
    };

    /*void main()
    {
        TMap<int, int> map;
        map.Insert(4, 000);
        map.Insert(1, 100);
        map.Insert(9, 200);
        map.Insert(5, 300);
        map.Insert(2, 400);
        map.Insert(0, 500);
        map.Insert(8, 600);
        map.Insert(7, 700);
        map.Insert(6, 800);
        map.Insert(3, 900);

        map.Erase(4);
        map.Erase(5);
        map.Erase(6);
        map.Erase(8);
        map.Erase(0);
        map.Erase(1);
        map.Erase(9);
        map.Erase(3);
        map.Erase(7);
        map.Erase(2);
    }*/
}

#endif // !__BLADE_CONTAINS_TMAP_H__
