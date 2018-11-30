#ifndef MINISTL_STL_MULTIMAP_H
#define MINISTL_STL_MULTIMAP_H

#include "../RBtree/stl_rb_tree.imp.h"
#include "../../Allocator/allocator.h"
#include "../../Functor/stl_function.h"

namespace MiniSTL {
    template<class Key, class T, class Compare = std::less<Key>, class Alloc = allocator<T>>
    class multimap {
    public:
        using key_type = Key;
        using data_type = T;
        using value_type = std::pair<const Key, T>;
        using key_compare = Compare;

        class value_compare : public std::binary_function<value_type, value_type, bool> {
            friend class multimap<Key, T, Compare, Alloc>;

        protected:
            Compare comp;
            value_compare(Compare c) : comp(c) {}
        public:
            bool operator()(const value_type &x, const value_type &y) const {
                return comp(x.first, y.first);
            }
        };

    private:
        using rep_type = rb_tree <key_type, value_type, select1st<value_type>, key_compare, Alloc>;
        rep_type t;

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
        multimap() : t(key_compare()) {}
        explicit multimap(const key_compare &comp) : t(comp) {}
        template<class InputIterator>
        multimap(InputIterator first, InputIterator last, const Compare &comp = Compare());
        multimap(std::initializer_list<value_type> l, const Compare &comp = Compare());
        multimap(const multimap &x) : t(x.t) {}

        multimap &operator=(const multimap &x);

        ~multimap() = default;

        // 比较相关
        template<class Key_, class T_, class Compare_, class Alloc_>
        friend bool operator==(const multimap<Key_, T_, Compare_, Alloc_> &,
                               const multimap<Key_, T_, Compare_, Alloc_> &);
        template<class Key_, class T_, class Compare_, class Alloc_>
        friend bool operator<(const multimap<Key_, T_, Compare_, Alloc_> &,
                              const multimap<Key_, T_, Compare_, Alloc_> &);

        // 迭代器相关
        iterator begin() { return t.begin(); }
        iterator end() { return t.end(); }
        const_iterator cbegin() const { return t.cbegin(); }
        const_iterator cend() const { return t.cend(); }

        // 容量以及getter相关
        key_compare key_comp() const { return t.key_comp(); }
        value_compare value_comp() const { return value_compare(t.key_comp()); }
        bool empty() const { return t.empty(); }
        size_type size() const { return t.size(); }
        size_type max_size() const { return t.max_size(); }


        // 访问元素相关
        iterator find(const key_type &x) { return t.find(x); }
        const_iterator find(const key_type &x) const { return t.find(x); }
        size_type count(const key_type &x) const { return t.find(x) == t.end() ? 0 : 1; }
        iterator lower_bound(const key_type &x) { return t.lower_bound(x); }
        const_iterator lower_bound(const key_type &x) const { return t.lower_bound(x); }
        iterator upper_bound(const key_type &x) { return t.upper_bound(x); }
        const_iterator upper_bound(const key_type &x) const { return t.upper_bound(x); }
        std::pair<iterator, iterator> equal_range(const key_type &x) { return t.equal_range(x); }
        std::pair<const_iterator, const_iterator> equal_range(const key_type &x) const { return t.equal_range(x); }

        // 修改容器相关
        iterator insert(const value_type &x);
        iterator insert(iterator position, const value_type &x);
        template<class InputIterator>
        void insert(InputIterator first, InputIterator last);

        void erase(iterator position) { t.erase(position); }
        size_type erase(const key_type &x) { return t.erase(x); }
        void erase(iterator first, iterator last) { t.erase(first, last); }

        void clear() { t.clear(); }
        void swap(multimap &x) { t.swap(x.t); }
    };
}

#endif //MINISTL_STL_MULTIMAP_H
