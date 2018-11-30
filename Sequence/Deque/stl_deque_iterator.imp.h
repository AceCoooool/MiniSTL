#ifndef MINISTL_STL_DEQUE_ITERATOR_IMP_H
#define MINISTL_STL_DEQUE_ITERATOR_IMP_H

#include <iostream>  // TODO: for debug
#include "stl_deque_iterator.h"

namespace MiniSTL {
    /*****************************辅助函数****************************/
    template<class T, class Ref, class Ptr, size_t BufSize>
    inline void deque_iterator<T, Ref, Ptr, BufSize>::set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + buffer_size;
    }

    /***********************构造, 复制, 析构相关***********************/
//    template<class T, class Ref, class Ptr, size_t BufSize>
//    inline deque_iterator<T, Ref, Ptr, BufSize>::deque_iterator(iterator &&rhs) {
//        cur = rhs.cur;
//        first = rhs.first;
//        last = rhs.last;
//        node = rhs.node;
//        rhs.cur = rhs.first = rhs.last = nullptr;
//        rhs.node = nullptr;
//    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::self &
    deque_iterator<T, Ref, Ptr, BufSize>::operator=(const iterator &deq_iter) {
        cur = deq_iter.cur;
        first = deq_iter.first;
        last = deq_iter.last;
        node = deq_iter.node;
        return *this;
    }

    /****************************重载运算子**************************/
    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::self &
    deque_iterator<T, Ref, Ptr, BufSize>::operator++() {
        ++cur;               // 单纯指针运算
        if (cur == last) {   // 若已抵达尾端, 跳转到下一个缓冲区
            set_node(++node);
            cur = first;
        }
        return *this;
    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::self
    deque_iterator<T, Ref, Ptr, BufSize>::operator++(int) {
        self temp = *this;
        ++*this;
        return temp;
    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::self &
    deque_iterator<T, Ref, Ptr, BufSize>::operator--() {
        if (cur == first) {
            set_node(--node);
            cur = last;
        }
        --cur;
        return *this;
    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::self
    deque_iterator<T, Ref, Ptr, BufSize>::operator--(int) {
        self temp = *this;
        --*this;
        return temp;
    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::self &
    deque_iterator<T, Ref, Ptr, BufSize>::operator+=(difference_type n) {
        difference_type off_set = n + (cur - first);
        if (off_set >= 0 && off_set < buffer_size) {  //不需要跳转
            cur += n;
        } else {  // 需要注意的是off_set小于0则必然需要跳转
            difference_type node_offset = off_set > 0 ? off_set / buffer_size
                                                      : -((-off_set - 1) / buffer_size) - 1;
            set_node(node + node_offset);
            cur = first + (off_set - (node_offset * buffer_size));
        }
        return *this;
    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::self
    deque_iterator<T, Ref, Ptr, BufSize>::operator+(difference_type n) {
        self temp = *this;
        return temp += (n);
    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::self
    deque_iterator<T, Ref, Ptr, BufSize>::operator-(difference_type n) {
        self temp = *this;
        return temp -= (n);
    }

    template<class T, class Ref, class Ptr, size_t BufSize>
    inline typename deque_iterator<T, Ref, Ptr, BufSize>::difference_type
    deque_iterator<T, Ref, Ptr, BufSize>::operator-(const self &rhs) const {
        return buffer_size * (node - rhs.node - 1) + (cur - first) + (rhs.last - rhs.cur);
    }

}

#endif //MINISTL_STL_DEQUE_ITERATOR_IMP_H
