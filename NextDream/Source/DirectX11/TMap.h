#ifndef __BLADE_CONTAINS_TMAP_H__
#define __BLADE_CONTAINS_TMAP_H__

#include <Utility.h>

namespace BladeEngine
{
    namespace Contains
    {
        template<typename Type>
        struct DefaultCompareFunc
        {
            int32 Compare(const Type& lh, const Type& rh)
            {
                return (lh < rh ? 1 : (rh < lh ? -1 : 0));
            }
        };
    }

    template<typename KeyType, typename ValueType, typename CompareFunc = Contains::DefaultCompareFunc<KeyType>>
    class TMap : public INoncopyable
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
                    parentNode->RightNode = rightNode£»
                }
            }

            inNode->ParentNode = rightNode;
            rightNode->LeftNode = inNode;

            if (rightLeftNode != NULL)
            {
                rightLeftNode->ParentNode = inNode;
                inNode->RightNode = rightLeftNode;
            }
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

        _RBTree_Node* _RBTreeSreachNode(KeyType inKey, _RBTree_Node* inRoot)
        {
            _RBTree_Node* node = inRoot;
            int32 result = 0;

            while (node == NULL)
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
            _RBTree_Node* insertNode = NULL;
            int32 result = 0;

            while (node == NULL)
            {
                insertNode = node;
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
                    BladeAssert(0);
                }
            }

            return insertNode;
        }

        _RBTree_Node* _RBTreeInsert(const KeyType& inKey, const ValueType& inValue, _RBTree_Node** inRoot)
        {
            _RBTree_Node* insertNode = (_RBTree_Node*)SystemMalloc::GetInstance().Alloc(sizeof(_RBTree_Node));
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

            _RBTreeInsert_SreachParentNode(insertNode, inRoot);
        }

        void _RBTreeInsertBalance(_RBTree_Node inNode, _RBTree_Node* inRoot)
        {
            _RBTree_Node* node = inNode;
            _RBTree_Node* parent = node->ParentNode;
            _RBTree_Node* grandParent = parent->ParentNode;
            _RBTree_Node* uncle = grandParetn->LeftNode == parent ? grandParetn->RightNode : grandParetn->LeftNode;

            while (grandParent != NULL && parent != NULL && parent->Color != Black)
            {
                if (uncle == NULL || uncle->Color == Black)
                {
                    if (grandParent->LeftNode == parent)
                    {
                        if (parent->RightNode = node)
                        {
                            _RBTreeRotateLeft(parent, &inRoot);
                            _RBTree_Node* temp = node;
                            node = parent;
                            parent = node;
                        }

                        grandParent->Color = Red;
                        parent->Color = Black;
                        _RBTreeRotateLeft(grandParent, &inRoot);
                    }
                    else
                    {
                        if (parent->RightNode = node)
                        {
                            _RBTreeRotateLeft(parent, &inRoot);
                            _RBTree_Node* temp = node;
                            node = parent;
                            parent = node;
                        }

                        grandParent->Color = Red;
                        parent->Color = Black;
                        _RBTreeRotateRight(grandParent, &inRoot);
                    }
                }
                else
                {
                    grandParent->Color = Red;
                    parent->Color = Black;
                    uncle->Color = Black;

                    node = grandParent;
                }

                parent = node->ParentNode;
                grandParent = parent->ParentNode;
                uncle = grandParent->LeftNode == parent ? grandParent->RightNode : parent;
            }

            parent->Color = Black;
        }

        void _RBTreeErase(const KeyType& inKey, _RBTree_Node** inRoot)
        {
            _RBTree_Node* eraseNode = _RBTreeSreachNode(inKey, inRoot);
            if (eraseNode == NULL)
            {
                return;
            }

            // greater than erase node but less than other children in erase node
            _RBTree_Node* replaceNode = eraseNode->RightNode;  
            while (replaceNode->LeftNode != NULL)
            {
                replaceNode = replaceNode->LeftNode;
            }

            // swap replace node and erase node
            if( replaceNode->ParentNode->LeftNode == replaceNode )
            {
                replaceNode->ParentNode->LeftNode = NULL;
            }
            else
            {
                replaceNode->ParentNode->RightNode = NULL;
            }
            
            replaceNode->ParentNode = eraseNode->ParentNode;
            replaceNode->LeftNode = eraseNode->LeftNode;
            replaceNode->RightNode = eraseNode->RightNode;

            SystemMalloc::GetInstance().Free(eraseNode);

            _RBTreeEraseBalance(replaceNode, inRoot);
        }

        void _RBTreeEraseBalance(_RBTree_Node inNode, _RBTree_Node* inRoot)
        {
            _RBTree_Node* node = inNode;
            _RBTree_Node* parent = node->ParentNode;
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
                        (brother->RightNode == NULL || brother->RightNode->Color)
                    {
                        brother->Color = Red;

                        if (parent->Color == Red)
                        {
                            // node's subtree the number of black node is equal than which before erase node
                            parent->Color == Black;
                            break;
                        }
                        else
                        {
                            // the number of parent's subtree decrease one black node but is balanced. set parent as node.
                            parent->Color == Black;
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
                            brother->RightNode = Black;
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
                        brother->RightNode =
                            parent->Color = Red;
                        // node's subtree the number of black node is equal than which before erase node
                        _RBTreeRotateLeft(parent, inRoot);
                        break;
                    }
                }
                else
                {
                    if(brother->Color == Red)
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
                    if( (brother->LeftNode == NULL || brother->LeftNode->Color == Black) &&
                        (brother->RightNode == NULL || brother->RightNode->Color == Black) )
                    {
                        brother->Color = Red;
                        if(parent->Color == Red)
                        {
                            parent->Color = Black;
                            break;
                        }
                        else
                        {
                            parent->Color == Black;
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
        bool TryGetValue(const KeyType& inKey, ValueType* outValue)
        {
            _RBTree_Node* node = _RBTreeSreachNode(inKey, &m_Root);
            if (node == NULL)
            {
                return false;
            }

            if(outValue != NULL)
            {
                *outValue = node->Value;
            }
            return true;
        }

        void Insert(const KeyType& inKey, const ValueType& inValue)
        {
            _RBTreeInsert(inKey, inValue);
        }

        void Erase(const KeyType& inKey)
        {
            _RBTreeErase(inKey);
        }
    };

}

#endif // !__BLADE_CONTAINS_TMAP_H__
