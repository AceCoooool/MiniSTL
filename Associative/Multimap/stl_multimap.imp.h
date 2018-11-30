#ifndef MINISTL_STL_MULTIMAP_IMP_H
#define MINISTL_STL_MULTIMAP_IMP_H

#include "stl_multimap.h"

namespace MiniSTL {
    /***********************构造, 复制, 析构相关***********************/
    template<class Key, class T, class Compare, class Alloc>
    template<class InputIterator>
    multimap<Key, T, Compare, Alloc>::multimap(InputIterator first, InputIterator last, const Compare &comp): t(comp) {
        t.insert_equal(first, last);
    }

    template<class Key, class T, class Compare, class Alloc>
    multimap<Key, T, Compare, Alloc>::multimap(std::initializer_list<value_type> l, const Compare &comp): t(comp) {
        t.insert_equal(l.begin(), l.end());
    }


    template<class Key, class T, class Compare, class Alloc>
    multimap<Key, T, Compare, Alloc> &
    multimap<Key, T, Compare, Alloc>::operator=(const multimap &x) {
        t = x.t;
        return *this;
    }

    /*************************比较操作相关***********************/
    template<class Key, class Tp, class Compare, class Alloc>
    inline bool operator==(const multimap<Key, Tp, Compare, Alloc> &lhs, const multimap<Key, Tp, Compare, Alloc> &rhs) {
        return lhs.t == rhs.t;
    }

    template<class Key, class Tp, class Compare, class Alloc>
    inline bool operator<(const multimap<Key, Tp, Compare, Alloc> &lhs, const multimap<Key, Tp, Compare, Alloc> &rhs) {
        return lhs.t < rhs.t;
    }

    /*************************修改容器相关***********************/
    // 插入
    template<class Key, class T, class Compare, class Alloc>
    typename multimap<Key, T, Compare, Alloc>::iterator
    multimap<Key, T, Compare, Alloc>::insert(const value_type &x) {
        return t.insert_equal(x);
    }

    template<class Key, class T, class Compare, class Alloc>
    typename multimap<Key, T, Compare, Alloc>::iterator
    multimap<Key, T, Compare, Alloc>::insert(iterator position, const value_type &x) {
        return t.insert_equal(position, x);
    }

    template<class Key, class T, class Compare, class Alloc>
    template<class InputIterator>
    void multimap<Key, T, Compare, Alloc>::insert(InputIterator first, InputIterator last) {
        t.insert_equal(first, last);
    }

}

#endif //MINISTL_STL_MULTIMAP_IMP_H
