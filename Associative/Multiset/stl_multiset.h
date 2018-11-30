#ifndef MINISTL_STL_MULTISET_H
#define MINISTL_STL_MULTISET_H

#include "../RBtree/stl_rb_tree.imp.h"
#include "../../Allocator/allocator.h"
#include "../../Functor/stl_function.h"

namespace MiniSTL {
    template<class Key, class Compare = std::less<Key>, class Alloc = allocator<Key>>
    class multiset {
    public:
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
        rep_type t;     // 底层红黑树

    public:
        // alias declaration
        using pointer = typename rep_type::const_pointer;
        using const_pointer = typename rep_type::const_pointer;
        using reference = typename rep_type::const_reference;
        using const_reference = typename rep_type::const_reference;
        using iterator = typename rep_type::const_iterator;
        using const_iterator = typename rep_type::const_iterator;
//        using reverse_iterator = typename rep_type::const_reverse_iterator;
//        using const_reverse_iterator = typename rep_type::const_reverse_iterator;

        using size_type = typename rep_type::size_type;
        using difference_type = typename rep_type::difference_type;

        // 构造, 复制, 析构
        multiset() : t(key_compare()) {}
        explicit multiset(const key_compare &comp) : t(comp) {}
        template<class InputIterator>
        multiset(InputIterator first, InputIterator last, const Compare &comp = Compare());
        multiset(const multiset &x) : t(x.t) {}

        multiset<Key, Compare, Alloc> &operator=(const multiset &x);

        ~multiset() = default;

        // 比较相关
        template<class Key_, class Compare_, class Alloc_>
        friend bool operator==(const multiset &, const multiset &);
        template<class Key_, class Compare_, class Alloc_>
        friend bool operator<(const multiset &, const multiset &);

        // 迭代器相关
        iterator begin() const { return t.cbegin(); }
        iterator end() const { return t.cend(); }


        // 容量以及getter相关
        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return t.key_comp(); }
        bool empty() const { return t.empty(); }
        size_type size() const { return t.size(); }
        size_type max_size() const { return t.max_size(); }

        // 访问元素相关
        iterator find(const key_type &x) const { return t.find(x); }
        size_type count(const key_type &x) const { return t.find(x) == t.cend() ? 0 : 1; }
        //返回不小于当前元素的第一个可插入的位置
        iterator lower_bound(const key_type &x) const { return t.lower_bound(x); }
        // 返回大于当前元素的第一个可插入的位置
        iterator upper_bound(const key_type &x) const { return t.upper_bound(x); }
        std::pair<iterator, iterator> equal_range(const key_type &x) const { return t.equal_range(x); }

        // 修改容器相关
        iterator insert(const value_type &x);
        iterator insert(iterator pos, const value_type &x);
        template<class InputIterator>
        void insert(InputIterator first, InputIterator last);

        void erase(iterator pos);
        size_type erase(const key_type &x) { return t.erase(x); }
        void erase(iterator first, iterator last);

        void clear() { t.clear(); }
    };
}

#endif //MINISTL_STL_MULTISET_H
