#ifndef MINISTL_STL_SET_H
#define MINISTL_STL_SET_H

#include "../RBtree/stl_rb_tree.imp.h"
#include "../../Functor/stl_function.h"

namespace MiniSTL {
    template<class Key, class Compare = std::less<Key>, class Alloc = allocator<Key>>
    class set {
    public:
        // alias declaration
        using key_type = Key;
        using value_type = Key;
        using key_compare = Compare;
        using value_compare = Compare;

    private:
        template<class T>
        struct identity : public unary_function<T, T> {
            const T &operator()(const T &x) const { return x; }
        };

        using rep_type = rb_tree<key_type, value_type, identity<value_type>, Compare, Alloc>;
        rep_type t;            // 底层红黑树

    public:
        using size_type = typename rep_type::size_type;
        using difference_type = typename rep_type::difference_type;

        using pointer = typename rep_type::const_pointer;
        using const_pointer = typename rep_type::const_pointer;
        using reference = typename rep_type::const_reference;
        using const_reference = typename rep_type::const_reference;
        using iterator = typename rep_type::const_iterator;
        using const_iterator = typename rep_type::const_iterator;

    public:
        // 构造, 复制, 析构相关函数
        set() : t(Compare()) {}
        explicit set(const Compare &comp) : t(comp) {}
        template<class InputIterator>
        set(InputIterator first, InputIterator last, const Compare &comp=Compare());
        set(std::initializer_list<value_type > l, const Compare& comp = Compare());
        set(const set &rhs) : t(rhs.t) {}

        set &operator=(const set &rhs);

        ~set() = default;

        // 比较相关
        template<class _Key, class _Compare, class _Alloc>
        friend bool operator==(const set<_Key, _Compare, _Alloc> &lhs, const set<_Key, _Compare, _Alloc> &rhs);
        template<class _Key, class _Compare, class _Alloc>
        friend bool operator<(const set<_Key, _Compare, _Alloc> &lhs, const set<_Key, _Compare, _Alloc> &rhs);

        // 迭代器相关
        iterator begin() const noexcept { return t.cbegin(); }
        iterator end() const noexcept { return t.cend(); }
        iterator cbegin() const noexcept { return t.cbegin(); }
        iterator cend() const noexcept { return t.cend(); }

        // 与容量以及getter相关
        key_compare key_comp() const noexcept { return t.key_comp(); }
        value_compare value_comp() const noexcept { return t.key_comp(); } //set的value_comp即为rb-tree的key_comp
        bool empty() const noexcept { return t.empty(); }
        size_type size() const noexcept { return t.size(); }
        size_type max_size() const noexcept { return t.max_size(); }

        // 访问元素相关
        iterator find(const key_type &x) const noexcept { return t.find(x); }
        size_type count(const key_type &x) const noexcept { return t.find(x) == t.cend() ? 0 : 1; }
        iterator lower_bound(const key_type &x) const noexcept { return t.lower_bound(x); }
        iterator upper_bound(const key_type &x) const noexcept { return t.upper_bound(x); }
        std::pair<iterator, iterator> equal_range(const key_type &x) const { return t.equal_range(x); }

        // 修改容器相关
        std::pair<iterator, bool> insert(const value_type &x);
        iterator insert(iterator pos, const value_type &x);
        template<class InputIterator>
        void insert(InputIterator first, InputIterator last);

        void erase(iterator pos);
        size_type erase(const key_type &x) { return t.erase(x); }
        void erase(iterator first, iterator last);
        void clear() { t.clear(); }
    };
}

#endif //MINISTL_STL_SET_H
