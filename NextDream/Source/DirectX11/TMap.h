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

        struct RBTree_Node
        {
            uint32 Color;
            RBTree_Node* ParentNode;
            RBTree_Node* LeftNode;
            RBTree_Node* RightNode;
            KeyType Key;
            ValueType Value;
        };    

    private:
        void _RBTreeRotateLeft(RBTree_Node* inNode, RBTree_Node** inRoot)
        {
            BladeAssert(inNode->RightNode != NULL);

            RBTree_Node* parentNode = inNode->ParentNode;
            RBTree_Node* rightNode = inNode->RightNode;
            RBTree_Node* rightLeftNode = inNode->RightNode->LeftNode;

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

        void _RBTreeRotateRight(RBTree_Node* inNode, RBTree_Node** inRoot)
        {
            BladeAssert(inNode->LeftNode != NULL);

            RBTree_Node* parentNode = inNode->ParentNode;
            RBTree_Node* leftNode = inNode->LeftNode;
            RBTree_Node* leftRightNode = inNode->LeftNode->RightNode;

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

        RBTree_Node* _RBTreeSreachNode(KeyType inKey, RBTree_Node* inRoot)
        {
            RBTree_Node* node = inRoot;
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

        int32 _RBTreeInsert_SreachParentNode(KeyType inKey, RBTree_Node* inRoot, RBTree_Node** inParent)
        {
            RBTree_Node* node = inRoot;
            RBTree_Node* insertNode = NULL;
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

        RBTree_Node* _RBTreeInsert(const KeyType& inKey, const ValueType& inValue, RBTree_Node** inRoot)
        {
            RBTree_Node* insertNode = (RBTree_Node*)SystemMalloc::GetInstance().Alloc(sizeof(RBTree_Node));
            insertNode->Key = inKey;
            insertNode->Value = inValue;
            insertNode->ParentNode = NULL;
            insertNode->LeftNode = NULL;
            insertNode->RightNode = NULL;
            insertNode->Color = Red;

            RBTree_Node* parentNode = NULL;
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

        void _RBTreeInsertBalance(RBTree_Node inNode, RBTree_Node* inRoot)
        {
            RBTree_Node* node = inNode;
            RBTree_Node* parent = node->ParentNode;
            RBTree_Node* grandParent = parent->ParentNode;
            RBTree_Node* uncle = grandParetn->LeftNode == parent ? grandParetn->RightNode : grandParetn->LeftNode;

            while (grandParent != NULL && parent != NULL && parent->Color != Black)
            {
                if (uncle == NULL || uncle->Color == Black)
                {
                    if (grandParent->LeftNode == parent)
                    {
                        if (parent->RightNode = node)
                        {
                            _RBTreeRotateLeft(parent, &inRoot);
                            RBTree_Node* temp = node;
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
                            RBTree_Node* temp = node;
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

        void _RBTreeErase(const KeyType& inKey, RBTree_Node* inRoot)
        {
            RBTree_Node* eraseNode = _RBTreeSreachNode(inKey, inRoot);
            if (eraseNode == NULL)
            {
                return;
            }

            while ()
            {

            }
        }

    public:
        
    };

}

#endif // !__BLADE_CONTAINS_TMAP_H__
