#ifndef MINISTL_STL_MAP_H
#define MINISTL_STL_MAP_H

#include "../RBtree/stl_rb_tree.imp.h"
#include "../../Allocator/allocator.h"
#include "../../Functor/stl_function.h"

namespace MiniSTL {
    template<class Key, class T, class Compare = std::less<Key>, class Alloc = allocator<T>>
    class map {
    public:
        using key_type = Key;
        using data_type = T;
        using value_type = std::pair<const Key, T>;
        using key_compare = Compare;

        class value_compare : public binary_function<value_type, value_type, bool> {
            friend class map<Key, T, Compare, Alloc>;

        protected:    // 友元类能够调用
            Compare comp;
            value_compare(Compare c) : comp(c) {}
        public:
            bool operator()(const value_type &x, const value_type &y) const {
                return comp(x.first, y.first);
            }
        };

    private:
        using rep_type = rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc>;
        rep_type t;   // 底层红黑树

    public:
        // alias declaration
        using pointer = typename rep_type::pointer;
        using const_pointer = typename rep_type::const_pointer;
        using reference = typename rep_type::reference;
        using const_reference = typename rep_type::const_reference;
        using iterator = typename rep_type::iterator;
        using const_iterator = typename rep_type::const_iterator;

        using size_type = typename rep_type::size_type;
        using difference_type = typename rep_type::difference_type;

    public:
        // 构造, 复制, 析构
        map() : t(key_compare()) {}
        explicit map(const key_compare &comp) : t(comp) {}
        template<class InputIterator>
        map(InputIterator first, InputIterator last, const key_compare &comp = key_compare());
        map(std::initializer_list<value_type> l, const key_compare &comp = key_compare());
        map(const map &rhs) : t(rhs.t) {}

        map &operator=(const map &rhs);

        ~map() = default;

        // 比较相关
        template<class Key_, class Tp_, class Compare_, class Alloc_>
        friend bool operator==(const map &lhs, const map &rhs);
        template<class Key_, class Tp_, class Compare_, class Alloc_>
        friend bool operator<(const map &lhs, const map &rhs);

        // 迭代器相关
        iterator begin() noexcept { return t.begin(); }
        iterator end() noexcept { return t.end(); }
        const_iterator cbegin() const noexcept { return t.cbegin(); }
        const_iterator cend() const noexcept { return t.cend(); }

        // 容量以及getter相关
        key_compare key_comp() const noexcept { return t.key_comp(); }
        value_compare value_comp() const noexcept { return value_compare(t.key_comp()); }
        bool empty() const noexcept { return t.empty(); }
        size_type size() const noexcept { return t.size(); }
        size_type max_size() const noexcept { return t.max_size(); }

        // 访问元素相关
        iterator find(const key_type &x) noexcept { return t.find(x); }
        const_iterator find(const key_type &x) const noexcept { return t.find(x); }
        size_type count(const key_type &x) const noexcept { return t.find(x) == t.end() ? 0 : 1; }
        iterator lower_bound(const key_type &x) noexcept { return t.lower_bound(x); }
        const_iterator lower_bound(const key_type &x) const noexcept { return t.lower_bound(x); }
        iterator upper_bound(const key_type &x) noexcept { return t.upper_bound(x); }
        const_iterator upper_bound(const key_type &x) const noexcept { return t.upper_bound(x); }
        std::pair<iterator, iterator> equal_range(const key_type &x) noexcept { return t.equal_range(x); }
        std::pair<const_iterator, const_iterator> equal_range(const key_type &x) const noexcept;

        // 修改容器相关
        std::pair<iterator, bool> insert(const value_type &x) { return t.insert_unique(x); }
        iterator insert(iterator pos, const value_type &x) { return t.insert_unique(pos, x); }
        template<class InputIterator>
        void insert(InputIterator first, InputIterator last) { t.insert_unique(first, last); }

        void erase(iterator pos) { t.erase(pos); }
        size_type erase(const key_type &x) { return t.erase(x); }
        void erase(iterator first, iterator last) { t.erase(first, last); }

        void clear() { t.clear(); }

        void swap(map<Key, T, Compare, Alloc> &x) noexcept { t.swap(x.t); }

        T &operator[](const key_type &k) { return (*((insert(value_type(k, T()))).first)).second; }

    };

}

#endif //MINISTL_STL_MAP_H
