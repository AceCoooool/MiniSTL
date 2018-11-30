#ifndef MINISTL_STL_RB_TREE_IMP_H
#define MINISTL_STL_RB_TREE_IMP_H

#include <exception>
#include "stl_rb_tree.h"
#include "../../Iterator/stl_iterator.h"
#include "../../Allocator/stl_construct.h"

namespace MiniSTL {
    /***********************辅助函数(private): 放到前面方便理解***********************/
    // 内存管理
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::create_node(const value_type &value) {
        link_type temp = get_node();
        try {
            construct(&temp->value_field, value);
        } catch (std::exception) {
            put_node(temp);
        }
        return temp;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::destroy_node(link_type p) {
        destroy(&p->value_field);
        put_node(p);
    }

    // 复制一个节点的值和颜色
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clone_node(link_type p) {
        link_type temp = create_node(p->value_field);
        temp->color = p->color;
        temp->left = nullptr;
        temp->right = nullptr;
        temp->parent = nullptr;
        return temp;
    }

    // -------------------------辅助函数(erase, 平衡, insert等)-------------------
    // 左旋
    // 将x的右子树绕x逆时针旋转, 其右子的左子变为了x, 而原本的左子变为了x的右子
    // 详见算法导论p176页
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_left(base_ptr x, base_ptr &root) {
        base_ptr y = x->right;    // 旋转点右子
        x->right = y->left;       // 将x的右子树替换为y的左子树
        if (y->left)              // 若存在, 则确立新的父子关系
            y->left->parent = x;
        y->parent = x->parent;

        if (x == root)            // 令y顶替x
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    // 详见算法导论p176
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_right(base_ptr x, base_ptr &root) {
        base_ptr y = x->left;
        x->left = y->right;
        if (y->right)
            x->right->parent = x;
        y->parent = x->parent;
        if (x == root)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    // 平衡 --- TODO, 还没细读
    // x: 新增节点,  root: 根节点
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
    rb_tree_rebalance(rb_tree_node_base *x, rb_tree_node_base *&root) {
        x->color = rb_tree_red;                                  //新节点必为红
        while (x != root && x->parent->color == rb_tree_red) {   //当前父节点为红
            if (x->parent == x->parent->parent->left) {          //父亲为爷爷的左子
                base_ptr y = x->parent->parent->right;           //伯父
                if (y && y->color == rb_tree_red) {              //伯父存在且为红色
                    x->parent->color = rb_tree_black;
                    y->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    x = x->parent->parent;                       //将x更新至祖父,准备上溯
                } else {                                         //不存在伯父或其颜色为黑
                    if (x == x->parent->right) {
                        x = x->parent;
                        rb_tree_rotate_left(x, root);            //左旋
                    }
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    rb_tree_rotate_right(x->parent->parent, root);
                }
            } else {                                             //父为右子，与上文对称
                base_ptr y = x->parent->parent->left;            //伯父
                if (y && y->color == rb_tree_red) {
                    x->parent->color = rb_tree_black;
                    y->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    x = x->parent->parent;                       //将x更新至祖父,准备上溯
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = rb_tree_black;
                    x->parent->parent->color = rb_tree_red;
                    rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        root->color = rb_tree_black;                            //root永远为黑色
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(base_ptr x_, base_ptr y_, const value_type &value) {
        // x为新值插入点, 参数y为插入点之父节点, 参数v为新值
        auto x = reinterpret_cast<link_type>(x_);
        auto y = reinterpret_cast<link_type>(y_);
        link_type z;
        if (y == header || x || key_compare(KeyOfValue()(value), key(y))) {
            // 情况1: 待插入节点之父为header||待插入节点自身并不为nullptr(?这个条件是指什么)||父节点明确大于待插入值
            // 此处其实可以简单理解为作为y的左子节点
            z = create_node(value);
            left(y) = z;
            if (y == header) {               // 若y为header, 此时leftmost==z
                root() = z;
                rightmost() = z;
            } else if (y == leftmost()) {    // 若y为原本的最小值的情况
                leftmost() = z;
            }
        } else {                             // 情况2: 新插入点为y的右子
            z = create_node(value);
            right(y) = z;
            if (y == rightmost())
                rightmost() = z;
        }
        parent(z) = y;
        left(z) = nullptr;                    // 默认的插入方式是到叶子节点
        right(z) = nullptr;
        rb_tree_rebalance(z, header->parent); // 重新调整红黑树(新增节点颜色在其中调整)
        ++node_count;
        return iterator(z);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase(link_type x) {
        while (x) {
            _erase(right(x));
            auto y = left(x);
            destroy_node(x);
            x = y;
        }
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance_for_erase(base_ptr z, base_ptr &root,
                                                                                 base_ptr &leftmost,
                                                                                 base_ptr &rightmost) {
        base_ptr y = z;
        base_ptr x = nullptr;
        base_ptr x_parent = nullptr;
        if (!y->left)         // z至多存在一个孩子
            x = y->right;
        else {                // z至少存在一个孩子
            if (!y->right)
                x = y->left;
            else {
                y = y->right;
                while (y->left)
                    y = y->left;
                x = y->right;
            }
        }
        if (y != z) {         // 若条件成立, 此时y为z的后代
            z->left->parent = y;
            y->left = z->left;
            if (y != z->right) {
                x_parent = y->parent;
                if (x)
                    x->parent = y->parent;
                y->parent->left = x;
                y->right = z->right;
                z->right->parent = y;
            } else
                x_parent = y;
            if (root == z)
                root = y;
            else if (z->parent->left == z)
                z->parent->left = y;
            else
                z->parent->right = y;
            y->parent = z->parent;
            std::swap(y->color, z->color);
            y = z;
        } else {              // y==z
            x_parent = y->parent;
            if (x) x->parent = y->parent;
            if (root == z)
                root = x;
            else {
                if (z->parent->left == z)
                    z->parent->left = x;
                else
                    z->parent->right = x;
            }
            if (leftmost == z)
                if (!z->right)
                    leftmost = z->parent;
                else
                    leftmost = rb_tree_node_base::minimum(x);
            if (rightmost == z)
                if (!z->left)
                    rightmost = z->parent;
                else
                    rightmost = rb_tree_node_base::maximum(x);
        }
        if (y->color != rb_tree_red) {
            while (x != root && (!x || x->color == rb_tree_black))
                if (x == x_parent->left) {
                    base_ptr w = x_parent->right;
                    if (w->color == rb_tree_red) {
                        w->color = rb_tree_black;
                        x_parent->color = rb_tree_red;
                        rb_tree_rotate_left(x_parent, root);
                        w = x_parent->right;
                    }
                    if ((!w->left || w->left->color == rb_tree_black) &&
                        (!w->right || w->right->color == rb_tree_black)) {
                        w->color = rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    } else {
                        if (!w->right || w->right->color == rb_tree_black) {
                            if (w->left)
                                w->left->color = rb_tree_black;
                            w->color = rb_tree_red;
                            rb_tree_rotate_right(w, root);
                            w = x_parent->right;
                        }
                        w->color = x_parent->color;
                        x_parent->color = rb_tree_black;
                        if (w->right)
                            w->right->color = rb_tree_black;
                        rb_tree_rotate_left(x_parent, root);
                        break;
                    }
                } else {
                    base_ptr w = x_parent->left;
                    if (w->color == rb_tree_red) {
                        w->color = rb_tree_black;
                        x_parent->color = rb_tree_red;
                        rb_tree_rotate_right(x_parent, root);
                        w = x_parent->left;
                    }
                    if ((!w->right || w->right->color == rb_tree_black) &&
                        (!w->left || w->left->color == rb_tree_black)) {
                        w->color = rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    } else {
                        if (!w->left || w->left->color == rb_tree_black) {
                            if (w->right)
                                w->right->color = rb_tree_black;
                            w->color = rb_tree_red;
                            rb_tree_rotate_left(w, root);
                            w = x_parent->left;
                        }
                        w->color = x_parent->color;
                        x_parent->color = rb_tree_black;
                        if (w->left)
                            w->left->color = rb_tree_black;
                        rb_tree_rotate_right(x_parent, root);
                        break;
                    }
                }
            if (x)
                x->color = rb_tree_black;
        }
        return y;
    }

    // 将x复制到y(作为y的子节点)---返回的是y的子结点(x的复制品)
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_copy(link_type x, link_type y) {
        auto top = clone_node(x);
        top->parent = y;
        try {
            if (x->right)
                top->right = _copy(right(x), top);
            y = top;
            x = left(x);
            while (x) {
                link_type p = clone_node(x);
                y->left = p;
                p->parent = y;
                if (x->right)
                    p->right = _copy(right(x), p);
                y = p;
                x = left(x);
            }
        } catch (std::exception &) {
            _erase(top);
        }
    }

    // 辅助函数(后续函数的"调用")
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::init() {
        header = get_node();
        header->color = rb_tree_red;
        header->parent = nullptr;
        header->left = header;
        header->right = header;
    }


    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
        if (node_count != 0) {
            _erase(root());
            leftmost() = header;
            root() = nullptr;
            rightmost() = header;
            node_count = 0;
        }
    }

    /***********************构造, 复制, 析构函数**********************/
    // 复制构造函数
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const rb_tree &lhs):
        node_count(0), key_compare(lhs.key_compare) {
        if (!lhs.root())
            init();
        else {
            header->color = rb_tree_red;
            root() = copy(lhs.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
        }
        node_count = lhs.node_count;
    }

    // 复制操作
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree &rhs) {
        if (this != &rhs) {
            clear();
            key_compare = rhs.key_compare;
            if (!rhs.root()) {
                root() = nullptr;
                leftmost() = header;
                rightmost() = header;
            } else {
                root() = _copy(rhs.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
                node_count = rhs.node_count;
            }
        }
        return *this;
    }

    // 析构
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::~rb_tree() {
        clear();
        put_node(header);
    }

    /**********************************比较相关*******************************/
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator==(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
                           const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
        return lhs.size() == rhs.size() && std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator!=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
                           const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
        return !(lhs == rhs);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator<(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
                          const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
        return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator>(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
                          const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
        return rhs < lhs;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator<=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
                           const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
        return !(rhs < lhs);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline bool operator>=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
                           const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
        return !(lhs < rhs);
    }


    /**********************************迭代器相关*******************************/
    // 在stl_rb_tree.h中

    /**********************************访问元素相关*******************************/
    // find
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) noexcept {
        link_type y = header;   // 最后一个不小于k的node
        link_type x = root();   // 当前node
        while (x)
            if (!key_compare(key(x), k)) // x的键值不小于k
                y = x, x = left(x);
            else
                x = right(x);
        iterator j = iterator(y);
        // 没找到存在两种可能
        // 1.k比最大值还大，j已经指向了end
        // 2.已经寻至叶子，但此时发现k仍然小于key(j.node) 若找到应有k==key(j.node)
        return (j == end()) || key_compare(k, key(j.node)) ? end() : j;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) const noexcept {
        link_type y = header;  // 最后一个不小于k的node
        link_type x = root();  // 当前node
        while (x)
            if (!key_compare(key(x), k))  // x的键值不小于k
                y = x, x = left(x);
            else
                x = right(x);
        const_iterator j = const_iterator(y);
        // 没找到存在两种可能
        // 1.k比最大值还大, j已经指向了end
        // 2.已经寻至叶子, 但此时发现k仍然小于key(j.node) 若找到应有k==key(j.node)
        return (j == cend()) || key_compare(k, key(j.node)) ? cend() : j;
    }

    // bound and count
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type &k) const noexcept {
        std::pair<const_iterator, const_iterator> p = equal_range(k);
        auto n = distance(p.first, p.second);
        return n;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type &k) noexcept {
        link_type y = header;
        link_type x = root();
        while (x)
            if (!key_compare(key(x), k))
                y = x, x = left(x);
            else
                x = right(x);
        return iterator(y);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type &k) const noexcept {
        link_type y = header;
        link_type x = root();
        while (x)
            if (!key_compare(key(x), k))
                y = x, x = left(x);
            else
                x = right(x);
        return const_iterator(y);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type &k) noexcept {
        link_type y = header;
        link_type x = root();
        while (x)
            if (key_compare(k, key(x)))
                y = x, x = left(x);
            else
                x = right(x);
        return iterator(y);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type &k) const noexcept {
        link_type y = header;
        link_type x = root();
        while (x)
            if (key_compare(k, key(x)))
                y = x, x = left(x);
            else
                x = right(x);
        return const_iterator(y);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
        typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type &k) noexcept {
        return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
        typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type &k) const noexcept {
        return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
    }


    /**********************************修改容器相关*******************************/
    // 插入操作 --- 不允许重复, 返回pair<iterator, bool>
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type &value) {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while (x) {     // 寻找插入点
            y = x;
            comp = key_compare(KeyOfValue()(value), key(x));  // value是否小于x的键值
            x = comp ? left(x) : right(x);
        }
        // 此时y必为待插入点的父节点 (也必为叶节点)
        iterator j(y);
        // 情况1: 插入左侧
        if (comp) {                                           // y键值大于value键值, 插于左侧
            if (j == begin())                                 // 待插入点之父为最左节点
                return std::pair<iterator, bool>(_insert(x, y, value), true);
            else
                --j;                                          // 调整j准备完成测试(可能与某键值重复)
        }
        // 情况2: 插入右侧
        if (key_compare(key(j.node), KeyOfValue()(value)))      // 新键值不与旧有键值重复, 放心插入
            return std::pair<iterator, bool>(_insert(x, y, value), true);
        // 情况3: 插入为已存在的情况
        return std::pair<iterator, bool>(j, false);           // 当前value为重复值
    }

    // 在pos节点处插入值value的新节点(其实就是将pos视为根节点) --- 不允许重复, 返回iterator
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator pos, const value_type &value) {
        // 情况1: pos为begin(即最小节点) --- 此时其就为叶子节点
        if (pos.node == header->left) { // begin()
            // 情况1.1: 新节点更小
            if (node_count > 0 && key_compare(KeyOfValue()(value), key(pos.node)))
                return _insert(pos.node, pos.node, value);
            else   // 情况1.2: 新节点大
                return insert_unique(value).first;
        } else if (pos.node == header) {     // 情况2: pos为end()
            if (key_compare(key(rightmost()), KeyOfValue()(value)))
                return _insert(nullptr, rightmost(), value);
            else
                return insert_unique(value).first;
        } else {   // 情况3: pos为"正常位置"
            iterator before = pos;
            --before;
            if (key_compare(key(before.node), KeyOfValue()(value)) &&
                key_compare(KeyOfValue()(value), key(pos.node))) {
                if (!right(before.node))
                    return _insert(nullptr, before.node, value);
                else
                    return _insert(pos.node, pos.node, value);
            } else
                return insert_unique(value).first;
        }
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    template<class _InputIterator>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(_InputIterator first, _InputIterator last) {
        for (; first != last; ++first)
            insert_unique(*first);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type &value) {
        link_type y = header;
        link_type x = root();
        while (x) {
            y = x;
            x = key_compare(KeyOfValue()(value), key(x)) ? left(x) : right(x);  // 大则向左
        }
        return _insert(x, y, value);   // x为新值插入点, y为其父
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator pos, const value_type &value) {
        if (pos.node == header->left) { // begin()
            if (size() > 0 && !key_compare(key(pos.node), KeyOfValue()(value)))
                return _insert(pos.node, pos.node, value);
            else
                return insert_equal(value);
        } else if (pos.node == header) { // end()
            if (!key_compare(KeyOfValue()(value)), key(rightmost()))
                return _insert(nullptr, rightmost(), value);
            else
                return insert_equal(value);
        } else {
            iterator before = pos;
            --before;
            if (!key_compare(KeyOfValue()(value), key(before.node)) &&
                !key_compare(key(pos.node), KeyOfValue()(value))) {
                if (!right(before.node))
                    return _insert(nullptr, before.node, value);
                else
                    return _insert(pos.node, pos.node, value);
            } else
                return insert_equal(value);
        }
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    template<class _InputIterator>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(_InputIterator first, _InputIterator last) {
        for (; first != last; ++first)
            insert_equal(*first);
    }

    // 删除操作
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator pos) {
        link_type y = reinterpret_cast<link_type>(
            rb_tree_rebalance_for_erase(pos.node, header->parent, header->left, header->right));
        destroy_node(y);
        --node_count;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type &k) {
        std::pair<iterator, iterator> p = equal_range(k);
        size_type n = distance(p.first, p.second);
        erase(p.first, p.second);
        return n;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
        if (first == begin() && last == end())
            clear();
        else
            while (first != last) erase(first++);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::swap(rb_tree &lhs) noexcept {
        // swap data members
        std::swap(header, lhs.header);
        std::swap(node_count, lhs.node_count);
        std::swap(key_compare, lhs.key_compare);
    }

}

#endif //MINISTL_STL_RB_TREE_IMP_H
