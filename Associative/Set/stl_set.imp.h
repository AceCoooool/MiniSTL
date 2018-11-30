#ifndef MINISTL_STL_SET_IMP_H
#define MINISTL_STL_SET_IMP_H

#include "stl_set.h"

namespace MiniSTL {

    /***********************构造, 复制, 析构相关***********************/
    // 构造函数
    template<class Key, class Compare, class Alloc>
    template<class InputIterator>
    set<Key, Compare, Alloc>::set(InputIterator first, InputIterator last, const key_compare &comp): t(comp) {
        t.insert_unique(first, last);
    }

    template<class Key, class Compare, class Alloc>
    set<Key, Compare, Alloc>::set(std::initializer_list<value_type> l, const key_compare &comp): t(comp) {
        t.insert_unique(l.begin(), l.end());
    }

    // 复制
    template<class Key, class Compare, class Alloc>
    set<Key, Compare, Alloc> &
    set<Key, Compare, Alloc>::operator=(const set &rhs) {
        t = rhs.t;
        return *this;
    }

    /******************************比较相关***************************/
    template<class Key, class Compare, class Alloc>
    inline bool operator==(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs) {
        return lhs.t == rhs.t;
    }

    template<class Key, class Compare, class Alloc>
    inline bool operator!=(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs) {
        return !(lhs.t == rhs.t);
    }

    template<class Key, class Compare, class Alloc>
    inline bool operator<(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs) {
        return lhs.t < rhs.t;
    }

    template<class Key, class Compare, class Alloc>
    inline bool operator>(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs) {
        return rhs < lhs;
    }

    template<class Key, class Compare, class Alloc>
    inline bool operator<=(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs) {
        return !(rhs < lhs);
    }

    template<class Key, class Compare, class Alloc>
    inline bool operator>=(const set<Key, Compare, Alloc> &lhs, const set<Key, Compare, Alloc> &rhs) {
        return !(lhs < rhs);
    }

    /***************************修改容器相关**************************/
    // 插入
    template<class Key, class Compare, class Alloc>
    std::pair<typename set<Key, Compare, Alloc>::iterator, bool>
    set<Key, Compare, Alloc>::insert(const value_type &x) {
        std::pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
        return std::pair<iterator, bool>(p.first, p.second);
    }

    template<class Key, class Compare, class Alloc>
    typename set<Key, Compare, Alloc>::iterator
    set<Key, Compare, Alloc>::insert(iterator pos, const value_type &x) {
        using rep_iterator = typename rep_type::iterator;
        return t.insert_unique(reinterpret_cast<rep_iterator &>(pos), x);
    }

    template<class Key, class Compare, class Alloc>
    template<class InputIterator>
    void set<Key, Compare, Alloc>::insert(InputIterator first, InputIterator last) {
        t.insert_unique(first, last);
    }

    // 删除
    template<class Key, class Compare, class Alloc>
    void set<Key, Compare, Alloc>::erase(iterator pos) {
        using rep_iterator = typename rep_type::iterator;
        t.erase(reinterpret_cast<rep_iterator &>(pos));
    }

    template<class Key, class Compare, class Alloc>
    void set<Key, Compare, Alloc>::erase(iterator first, iterator last) {
        using rep_iterator = typename rep_type::iterator;
        t.erase(reinterpret_cast<rep_iterator &>(first), reinterpret_cast<rep_iterator &>(last));
    }


}

#endif //MINISTL_STL_SET_IMP_H
