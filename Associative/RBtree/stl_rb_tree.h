#ifndef MINISTL_STL_RB_TREE_H
#define MINISTL_STL_RB_TREE_H

#include <functional>
#include "../../Allocator/allocator.h"
#include "stl_rb_tree_iterator.imp.h"

namespace MiniSTL {
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc=allocator<Value>>
    class rb_tree {
    protected:
        // Internal alias declarations
        using void_pointer = void *;
        using base_ptr = rb_tree_node_base *;
        using rb_tree_node_ = rb_tree_node<Value>;
        using rb_tree_node_allocator = allocator<rb_tree_node_>;
        using color_type = rb_tree_color_type;

    public:
        // Basic types
        using key_type = Key;
        using value_type = Value;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using reference = value_type &;
        using const_reference = const value_type &;
        using link_type = rb_tree_node_ *;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        using iterator = rb_tree_iterator<value_type, reference, pointer>;
        using const_iterator = rb_tree_iterator<value_type, const_reference, const_pointer>;

    private:
        // 成员变量
        size_type node_count;     // 用节点数量表征树的大小
        link_type header;         // root的父亲, 实现技巧
        Compare key_compare;      // 比较器

        // 辅助函数(内存管理)
        link_type get_node() { return rb_tree_node_allocator::allocate(); }
        void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }
        link_type create_node(const value_type &value);
        void destroy_node(link_type p);
        link_type clone_node(link_type p);

        // 获得header的成员
        link_type &root() const { return reinterpret_cast<link_type &>(header->parent); }
        link_type &leftmost() const { return reinterpret_cast<link_type &>(header->left); }
        link_type &rightmost() const { return reinterpret_cast<link_type &>(header->right); }

        // 普通node的快速访问与设定
        static link_type &left(link_type p) { return reinterpret_cast<link_type &>(p->left); }
        static link_type &right(link_type p) { return reinterpret_cast<link_type &>(p->right); }
        static link_type &parent(link_type p) { return reinterpret_cast<link_type &>(p->parent); }
        static reference &value(link_type p) { return p->value_field; }
        static const Key &key(link_type p) { return KeyOfValue()(value(p)); }  // KeyofValue是一个函数对象
        static color_type &color(link_type p) { return static_cast<color_type &>(p->color); }

        // base_node的快速访问与设定
        static link_type &left(base_ptr p) { return reinterpret_cast<link_type &>(p->left); }
        static link_type &right(base_ptr p) { return reinterpret_cast<link_type &>(p->right); }
        static link_type &parent(base_ptr p) { return reinterpret_cast<link_type &>(p->parent); }
        static reference &value(base_ptr p) { return static_cast<link_type>(p)->value_field; }
        static const Key &key(base_ptr p) { return KeyOfValue()(value(static_cast<link_type>(p))); }
        static color_type &color(base_ptr p) { return static_cast<color_type &>(static_cast<link_type>(p)->color); }

        // 求取极值(转交node_base)
        static link_type minimum(link_type p) { return static_cast<link_type>(rb_tree_node_base::minimum(p)); }
        static link_type maximum(link_type p) { return static_cast<link_type>(rb_tree_node_base::maximum(p)); }

        // 左旋, 右旋, 平衡
        void rb_tree_rotate_left(base_ptr x, base_ptr &root);
        void rb_tree_rotate_right(base_ptr x, base_ptr &root);
        void rb_tree_rebalance(base_ptr x, base_ptr &root);

        // 插入和删除的辅助函数
        iterator _insert(base_ptr x, base_ptr y, const value_type &v);
        void _erase(link_type x);  // erase without rebalancing
        base_ptr rb_tree_rebalance_for_erase(base_ptr z, base_ptr &root, base_ptr &leftmost, base_ptr &rightmost);

        // 将x及其子节点复制到y中(即作为y的子节点)
        link_type _copy(link_type x, link_type y);

        // 初始化+擦除
        void init();               // 初始化hearder

    public:
        // 构造, 复制, 析构相关函数
        rb_tree() : node_count(0), key_compare() { init(); }
        rb_tree(const Compare &comp = Compare()) : node_count(0), key_compare(comp) { init(); }
        rb_tree(const rb_tree &lhs);

        rb_tree &operator=(const rb_tree &rhs);

        ~rb_tree();

        // 比较操作
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend bool operator==(const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &lhs,
                               const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &rhs);
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend bool operator!=(const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &lhs,
                               const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &rhs);
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend bool operator<(const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &lhs,
                              const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &rhs);
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend bool operator>(const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &lhs,
                              const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &rhs);
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend bool operator<=(const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &lhs,
                               const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &rhs);
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend bool operator>=(const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &lhs,
                               const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_> &rhs);

        // 迭代器相关
        iterator begin() { return leftmost(); }
        iterator end() { return header; }
        const_iterator cbegin() const noexcept { return leftmost(); }
        const_iterator cend() const noexcept { return header; }

        // 访问元素相关(或者getter)
        Compare key_comp() const noexcept { return key_compare; }

        iterator find(const key_type &) noexcept;
        const_iterator find(const key_type &) const noexcept;

        size_type count(const key_type &) const noexcept;
        iterator lower_bound(const key_type &) noexcept;
        const_iterator lower_bound(const key_type &) const noexcept;
        iterator upper_bound(const key_type &) noexcept;
        const_iterator upper_bound(const key_type &) const noexcept;
        std::pair<iterator, iterator> equal_range(const key_type &) noexcept;
        std::pair<const_iterator, const_iterator> equal_range(const key_type &) const noexcept;

        // 修改容器相关的操作
        std::pair<iterator, bool> insert_unique(const value_type &value);
        iterator insert_unique(iterator, const value_type &);
        template<class _InputIterator>
        void insert_unique(_InputIterator first, _InputIterator last);
        iterator insert_equal(iterator, const value_type &);
        iterator insert_equal(const value_type &);
        template<class _InputIterator>
        void insert_equal(_InputIterator first, _InputIterator last);

        void erase(iterator);
        size_type erase(const key_type &);
        void erase(iterator, iterator);

        void clear();

        void swap(rb_tree &rhs) noexcept;

        // 与容量相关
        bool empty() const { return node_count == 0; }
        size_type size() const { return node_count; }
        size_type max_size() const { return static_cast<size_type>(-1); }
    };

}

#endif //MINISTL_STL_RB_TREE_H
