#ifndef MINISTL_STL_MAP_IMP_H
#define MINISTL_STL_MAP_IMP_H

#include "stl_map.h"

namespace MiniSTL {
    /***********************构造, 复制, 析构相关***********************/
    // 构造函数
    template<class Key, class T, class Compare, class Alloc>
    template<class InputIterator>
    map<Key, T, Compare, Alloc>::map(InputIterator first, InputIterator last, const key_compare &comp) {
        t = comp;
        t.insert_unique(first, last);
    }

    template<class Key, class T, class Compare, class Alloc>
    map<Key, T, Compare, Alloc>::map(std::initializer_list<value_type> l, const key_compare &comp) {
        t = comp;
        t.insert_unique(l.begin(), l.end());
    }

    // 复制
    template<class Key, class T, class Compare, class Alloc>
    map<Key, T, Compare, Alloc> &
    map<Key, T, Compare, Alloc>::operator=(const map &rhs) {
        t = rhs.t;
        return *this;
    }

    /******************************比较相关*************************/
    template<class Key, class Tp, class Compare, class Alloc>
    inline bool operator==(const map<Key, Tp, Compare, Alloc> &lhs, const map<Key, Tp, Compare, Alloc> &rhs) {
        return lhs.t == rhs.t;
    }

    template<class Key, class Tp, class Compare, class Alloc>
    inline bool operator!=(const map<Key, Tp, Compare, Alloc> &lhs, const map<Key, Tp, Compare, Alloc> &rhs) {
        return !(lhs.t == rhs.t);
    }

    template<class Key, class Tp, class Compare, class Alloc>
    inline bool operator<(const map<Key, Tp, Compare, Alloc> &lhs, const map<Key, Tp, Compare, Alloc> &rhs) {
        return lhs.t < rhs.t;
    }

    template<class Key, class Tp, class Compare, class Alloc>
    inline bool operator>(const map<Key, Tp, Compare, Alloc> &lhs, const map<Key, Tp, Compare, Alloc> &rhs) {
        return rhs < lhs;
    }

    template<class Key, class Tp, class Compare, class Alloc>
    inline bool operator<=(const map<Key, Tp, Compare, Alloc> &lhs, const map<Key, Tp, Compare, Alloc> &rhs) {
        return !(rhs < lhs);
    }

    template<class Key, class Tp, class Compare, class Alloc>
    inline bool operator>=(const map<Key, Tp, Compare, Alloc> &lhs, const map<Key, Tp, Compare, Alloc> &rhs) {
        return !(lhs < rhs);
    }


    /****************************访问元素相关************************/
    template<class Key, class T, class Compare, class Alloc>
    std::pair<typename map<Key, T, Compare, Alloc>::const_iterator,
        typename map<Key, T, Compare, Alloc>::const_iterator>
    map<Key, T, Compare, Alloc>::equal_range(const key_type &x) const noexcept {
        return t.equal_range(x);
    }

}

#endif //MINISTL_STL_MAP_IMP_H
