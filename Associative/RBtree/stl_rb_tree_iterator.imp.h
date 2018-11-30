#ifndef MINISTL_STL_RB_TREE_ITERATOR_IMP_H
#define MINISTL_STL_RB_TREE_ITERATOR_IMP_H

#include "stl_rb_tree_iterator.h"

namespace MiniSTL {
    /*------------------------------节点-------------------------*/
    rb_tree_node_base::base_ptr rb_tree_node_base::minimum(base_ptr root) {
        while (root->left)
            root = root->left;
        return root;
    }

    rb_tree_node_base::base_ptr rb_tree_node_base::maximum(base_ptr root) {
        while (root->right) root = root->right;
        return root;
    }

    /*----------------------------iterator----------------------*/
    void rb_tree_base_iterator::increment() {
        if (node->right) {              // 情况1: 存在右子, 则下一节点必为右子树的最左下角
            node = node->right;
            while (node->left) node = node->left;
        } else {                        // 情况2: 不存在右子
            base_ptr p = node->parent;
            while (node == p->right) {  // 如果当前节点本身是个右子节点, 就一直上溯直到"不为右子节点"
                node = p;
                p = p->parent;
            }

            if (node->right != p)       // 情况2.1: 正常情况下该祖先之父即为结果(即大部分会进入该if)
                node = p;

            // 情况4.2. 若右子等于父节点, node即为自增结果(此为特殊情况, 适用于迭代器指向root而后者无right)
        }
    }

    void rb_tree_base_iterator::decrement() {
        if (node->color == rb_tree_red && node->parent->parent == node)
            // 情况1: 此为特例, 当迭代器指向end()将触发此情况(即node为header)
            // root存在一个父节点header, 其颜色为红色, 左右子均为root
            node = node->right;
        else if (node->left) {         // 情况2: 存在左子, 前一节点必然是左子树的最右下角
            base_ptr l = node->left;
            while (l->right) {
                l = l->right;
            }
            node = l;
        } else {                       // 情况3: 既非root, 亦无左子
            base_ptr p = node->parent;
            while (node == p->left) {  // 不断上溯, 直至找到第一个不为左子的祖先
                node = p;
                p = p->parent;
            }
            node = p;                  // 该祖先的父亲即为答案
        }
    }

    template<class T, class Ref, class Ptr>
    typename rb_tree_iterator<T, Ref, Ptr>::self &rb_tree_iterator<T, Ref, Ptr>::operator++() {
        increment();
        return *this;
    }

    template<class T, class Ref, class Ptr>
    typename rb_tree_iterator<T, Ref, Ptr>::self rb_tree_iterator<T, Ref, Ptr>::operator++(int) {
        self temp = *this;
        increment();
        return temp;
    }

    template<class T, class Ref, class Ptr>
    typename rb_tree_iterator<T, Ref, Ptr>::self &rb_tree_iterator<T, Ref, Ptr>::operator--() {
        decrement();
        return *this;
    }

    template<class T, class Ref, class Ptr>
    typename rb_tree_iterator<T, Ref, Ptr>::self rb_tree_iterator<T, Ref, Ptr>::operator--(int) {
        self temp = *this;
        decrement();
        return temp;
    }

    // 比较操作
    inline bool operator==(const rb_tree_base_iterator& lhs,const rb_tree_base_iterator& rhs){
        return lhs.node == rhs.node;
    }

    inline bool operator!=(const rb_tree_base_iterator& lhs,const rb_tree_base_iterator& rhs){
        return lhs.node != rhs.node;
    }
}

#endif //MINISTL_STL_RB_TREE_ITERATOR_IMP_H
