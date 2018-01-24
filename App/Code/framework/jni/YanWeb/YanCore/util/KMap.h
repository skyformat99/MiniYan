/*
============================================================================
 Name        : PlatformLib.h
 Author      : yanbo
 Version     : kmap v1.0
 Copyright   : All Copyright Reserved
 Description : support Original
============================================================================
*/


#ifndef KMap_h
#define KMap_h

#include "PlatformLib.h"

namespace util
{
    // this code is use red and black tree
    typedef enum NodeColor
    {
        BLACK = 0,
        RED
    } NodeColor;

    template<class KeyType, class ValueType>
    class MapPair
    {
    public:
        KeyType     m_key;
        ValueType   m_value;
    };

    template<class KeyType, class ValueType>
    struct TreeNode
    {
        MapPair<KeyType, ValueType>             m_pair;
        TreeNode<KeyType, ValueType>*           m_left;
        TreeNode<KeyType, ValueType>*           m_right;
        NodeColor                               m_color;
    };

    template<class KeyType, class ValueType>
    class KMap
    {
    public:
        typedef TreeNode<KeyType, ValueType>* TreeNodePtr;

    public:
        KMap()
                : m_rootPtr(NULL)
        {
        }

        ~KMap() { clear(); }

        ValueType& operator[](const KeyType& key)
        {
            TreeNodePtr result = searchAndInsert(key);
            return result->m_pair.m_value;
        }

        ValueType operator[](const KeyType& key) const
        {
            TreeNodePtr result = searchAndInsert(key);
            return result ? result->m_pair.m_value : ValueType();
        }

        ValueType get(const KeyType& key) const
        {
            TreeNodePtr result = searchAndInsert(key);
            return result ? result->m_pair.m_value : ValueType();
        }

        ValueType& get(const KeyType& key)
        {
            TreeNodePtr result = searchAndInsert(key);
            return result->m_pair.m_value;
        }

        LVoid set(const KeyType& key, const ValueType& value)
        {
            TreeNodePtr noderPtr = searchAndInsert(key);
            noderPtr->m_pair.m_value = value;
        }

        LBool isEmpty()
        {
            if (m_rootPtr)
            {
                return LFalse;
            }

            return LTrue;
        }

        const LBool isEmpty() const
        {
            if (m_rootPtr)
            {
                return LFalse;
            }

            return LTrue;
        }

        LVoid clear()
        {
            deleteTree(m_rootPtr);
        }

    private:
        TreeNodePtr newNode(const KeyType& key, NodeColor color)
        {
            TreeNodePtr tnptr = new TreeNode<KeyType, ValueType>;
            tnptr->m_left = NULL;
            tnptr->m_right = NULL;
            tnptr->m_pair.m_key = key;
            tnptr->m_color = BLACK;
            LMemset(&(tnptr->m_pair.m_value), 0, sizeof(ValueType));
            return tnptr;
        }

        TreeNodePtr searchAndInsert(const KeyType& key)
        {
            TreeNodePtr ptr = NULL;
            insertNode(m_rootPtr, key, BLACK, &ptr);
            if (!m_rootPtr) { m_rootPtr = ptr; }
            return ptr;
        }

        NodeColor leftColor(TreeNodePtr tnptr)
        {
            return tnptr->m_left ? tnptr->m_left->m_color : BLACK;
        }

        NodeColor rightColor(TreeNodePtr tnptr)
        {
            return tnptr->m_right ? tnptr->m_right->m_color : BLACK;
        }

        TreeNodePtr insertNode(TreeNodePtr tnptr, const KeyType& key, NodeColor color, TreeNodePtr* ptr)
        {
            if (!tnptr)
            {
                TreeNodePtr nptr = newNode(key, RED);
                *ptr = nptr;
                return *ptr;
            }

            if (leftColor(tnptr) && rightColor(tnptr))
            {
                tnptr->m_color = RED;
                tnptr->m_left->m_color = BLACK;
                tnptr->m_right->m_color = BLACK;
            }

            if (key < tnptr->m_pair.m_key)
            {
                tnptr->m_left = insertNode(tnptr->m_left, key, BLACK, ptr);
                if (tnptr->m_color && leftColor(tnptr) && color)
                {
                    tnptr = rightRotate(tnptr);
                }
                if (leftColor(tnptr) && leftColor(tnptr->m_left))
                {
                    tnptr = rightRotate(tnptr); tnptr->m_color = BLACK; tnptr->m_right->m_color = RED;
                }
            }
            else if (key > tnptr->m_pair.m_key)
            {
                tnptr->m_right = insertNode(tnptr->m_right, key, RED, ptr);
                if (tnptr->m_color && rightColor(tnptr) && !color)
                {
                    tnptr = leftRotate(tnptr);
                }
                if (rightColor(tnptr) && rightColor(tnptr->m_right))
                {
                    tnptr = leftRotate(tnptr); tnptr->m_color = BLACK; tnptr->m_left->m_color = RED;
                }
            }
            else if (key == tnptr->m_pair.m_key)
            {
                *ptr = tnptr;
            }

            return tnptr;
        }

        TreeNodePtr leftRotate(TreeNodePtr tnptr)
        {
            TreeNodePtr right = tnptr->m_right;
            tnptr->m_right = right->m_left;
            right->m_left = tnptr;
            return right;
        }

        TreeNodePtr rightRotate(TreeNodePtr tnptr)
        {
            TreeNodePtr left = tnptr->m_left;
            tnptr->m_left = left->m_right;
            left->m_right = tnptr;
            return left;
        }

        LVoid deleteTree(TreeNodePtr nodePtr)
        {
            if (nodePtr)
            {
                TreeNodePtr rightchild = nodePtr->m_right;
                TreeNodePtr leftchild = nodePtr->m_left;

                delete nodePtr;
                nodePtr = NULL;

                deleteTree(leftchild);
                deleteTree(rightchild);
            }

        }
    private:
        TreeNodePtr m_rootPtr;
    };
}

using util::KMap;
#endif
