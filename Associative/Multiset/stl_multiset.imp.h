#ifndef MINISTL_STL_MULTISET_IMP_H
#define MINISTL_STL_MULTISET_IMP_H

#include "stl_multiset.h"

namespace MiniSTL {
    /***********************构造, 复制, 析构相关***********************/
    template<class Key, class Compare, class Alloc>
    template<class InputIterator>
    multiset<Key, Compare, Alloc>::multiset(InputIterator first, InputIterator last, const Compare &comp): t(comp) {
        t.insert_equal(first, last);
    }

    template<class Key, class Compare, class Alloc>
    multiset<Key, Compare, Alloc> &
    multiset<Key, Compare, Alloc>::operator=(const multiset &x) {
        t = x.t;
        return *this;
    }

    /*************************修改容器相关***********************/
    template<class Key, class Compare, class Alloc>
    typename multiset<Key, Compare, Alloc>::iterator
    multiset<Key, Compare, Alloc>::insert(const value_type &x) {
        return t.insert_equal(x);
    }

    template<class Key, class Compare, class Alloc>
    typename multiset<Key, Compare, Alloc>::iterator
    multiset<Key, Compare, Alloc>::insert(iterator pos, const value_type &x) {
        using rep_iterator = typename rep_type::iterator;
        return t.insert_equal(static_cast<rep_iterator &>(pos), x);
    }

    template<class Key, class Compare, class Alloc>
    template<class InputIterator>
    void multiset<Key, Compare, Alloc>::insert(InputIterator first, InputIterator last) {
        t.insert_equal(first, last);
    }

    template<class Key, class Compare, class Alloc>
    void multiset<Key, Compare, Alloc>::erase(iterator pos) {
        using rep_iterator = typename rep_type::iterator;
        t.erase(static_cast<rep_iterator &>(pos));
    }

    template<class Key, class Compare, class Alloc>
    void multiset<Key, Compare, Alloc>::erase(iterator first, iterator last) {
        using rep_iterator = typename rep_type::iterator;
        t.erase(static_cast<rep_iterator &>(first), static_cast<rep_iterator &>(last));
    }

    /***************************比较相关*************************/
    template<class Key, class Compare, class Alloc>
    inline bool operator==(const multiset<Key, Compare, Alloc> &lhs, const multiset<Key, Compare, Alloc> &rhs) {
        return lhs.t == rhs.t;
    }

    template<class Key, class Compare, class Alloc>
    inline bool operator<(const multiset<Key, Compare, Alloc> &lhs, const multiset<Key, Compare, Alloc> &rhs) {
        return lhs.t < rhs.t;
    }


}

#endif //MINISTL_STL_MULTISET_IMP_H
