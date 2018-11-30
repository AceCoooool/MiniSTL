#ifndef MINISTL_ITERATOR_ADAPTERS_H
#define MINISTL_ITERATOR_ADAPTERS_H

#include <iostream>
#include "stl_iterator.h"

namespace MiniSTL {
    /**
     * insert iterators: 对insert_iterators做赋值操作时(operator=),调用底层容器的push_back
     * push_front或者insert
     */
    // 第一种: 采用back_push
    template<class Container>
    class back_insert_iterator {
    protected:
        Container *container;             // 底层容器

    public:
        using iterator_category = output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;

    public:
        explicit back_insert_iterator(Container &value) : container(&value) {}
        back_insert_iterator &operator=(const typename Container::value_type &value) {
            container->push_back(value);  // 本质上是调用了push_back
            return *this;
        }

        // 以下三个接口对back_insert_iterator无用, 关闭功能
        // (为什么不直接设为private?---因为类似copy等函数需要用用到)
        back_insert_iterator &operator*() { return *this; }
        back_insert_iterator &operator++() { return *this; }
        back_insert_iterator &operator++(int) { return *this; }
    };

    // 以下是一个辅助函数, 便于快速使用back_insert_iterator
    template<class Container>
    inline back_insert_iterator<Container> back_inserter(Container &x) {
        return back_insert_iterator<Container>(x);
    }

    // 第二种: 采用front_push
    template<class Container>
    class front_insert_iterator {
    protected:
        Container *container;           // 底层容器

    public:
        using iterator_category = output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;

    public:
        explicit front_insert_iterator(Container &value) : container(&value) {}
        front_insert_iterator &operator=(const typename Container::value_type &value) {
            container->push_front(value);
            return *this;
        }

        // 以下三个接口对back_insert_iterator无用, 关闭功能
        front_insert_iterator &operator*() { return *this; }
        front_insert_iterator &operator++() { return *this; }
        front_insert_iterator &operator++(int) { return *this; }
    };

    template<class Container>
    inline front_insert_iterator<Container> front_inserter(Container &x) {
        return front_insert_iterator<Container>(x);
    }

    // 第三种: 在指定位置(迭代器)插入
    template<class Container>
    class insert_iterator {
    protected:
        Container *container;                // 底层容器
        typename Container::iterator iter;

    public:
        using iterator_category = output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;

    public:
        insert_iterator(Container &value, typename Container::iterator i) : container(&value), iter(i) {}
        insert_iterator<Container> &operator=(const typename Container::value_type &value) {
            iter = container->insert(iter, value); // 调用insert
            ++iter;                         // 保证insert_iterator永远与目标贴合
            return *this;
        }

        // 以下三个接口对binsert_iterator不起作用, 关闭功能. 返回均为自己
        insert_iterator &operator*() { return *this; }
        insert_iterator &operator++() { return *this; }
        insert_iterator &operator++(int) { return *this; }
    };

    template<class Container, class Iterator>
    inline insert_iterator<Container> inserter(Container &x, Iterator i) {
        return insert_iterator<Container>(x, i);
    }

    /**
     * reverse iterator
     */
    template<class Iterator>
    class reverse_iterator {
        template<class Iterator1>
        friend bool operator==(const reverse_iterator<Iterator1> &, const reverse_iterator<Iterator1> &);
        template<class Iterator1>
        friend bool operator!=(const reverse_iterator<Iterator1> &, const reverse_iterator<Iterator1> &);

    protected:
        Iterator current;                 // 与之对应的正向迭代器

    public:
        using iterator_category = iterator_category_t<Iterator>;
        using value_type = value_type_t<Iterator>;
        using difference_type = difference_type_t<Iterator>;
        using pointer = pointer_t<Iterator>;
        using reference = reference_t<Iterator>;

        using iterator_type = Iterator;   // 正向迭代器
        using self = reverse_iterator;    // 反向迭代器

    public:
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type value) : current(value) {}
        reverse_iterator(const self &value) : current(value.current) {}

        iterator_type base() const { return current; }

        reference operator*() const {
            Iterator temp = current;
            return *--temp;               // 对逆向迭代器的取值等价于对应的正向迭代器后退一个单位取值
        }

        pointer operator->() const { return &(operator*()); }

        self &operator++() {
            --current;
            return *this;
        }

        self operator++(int) {
            self temp = *this;
            --current;
            return temp;
        }

        self &operator--() {
            ++current;
            return *this;
        }

        self operator--(int) {
            self temp = *this;
            ++current;
            return temp;
        }

        self operator+(difference_type n) const {
            return self(current - n);
        }

        self &operator+=(difference_type n) const {
            current -= n;
            return *this;
        }

        self operator-(difference_type n) const {
            return self(current + n);
        }

        self &operator-=(difference_type n) const {
            current += n;
            return *this;
        }

        reference operator[](difference_type n) const { return *(*this + n); }

        bool operator==(const self &rhs) const { return current == rhs.current; }
        bool operator!=(const self &rhs) const { return !((*this) == rhs); }
    };

    /**
     * istream_iterator
     */
    template<class T, class Distance = ptrdiff_t>
    class istream_iterator {
    protected:
        std::istream *stream;
        T value;
        bool end_marker;
        void read() {
            end_marker = (*stream) ? true : false;
            if (end_marker) *stream >> value;
            // 完成输入后, stream状态可能发生了改变，再次判定
            end_marker = (*stream) ? true : false;
        }

    public:
        using iterator_category = input_iterator_tag;
        using value_tyep = T;
        using difference_type = Distance;
        using pointer = const T *;    // 由于身为input_iterator, 采用const较为保险
        using reference = const T &;

        istream_iterator() : stream(&std::cin), end_marker(false) {}
        istream_iterator(std::istream &s) : stream(&s) { read(); }

        reference operator*() const { return value; }
        pointer operator->() const { return &(operator*()); }

        // 迭代器前进一个位置, 就代表要读取一笔资料
        istream_iterator &operator++() {
            read();
            return *this;
        }

        istream_iterator operator++(int) {
            istream_iterator temp = *this;
            read();
            return temp;
        }
    };

    /**
     * ostream_iterator
     */
    template<class T>
    class ostream_iterator {
    protected:
        std::ostream *stream;
        const char *interval;     // 输出间隔符

    public:
        using iterator_category = output_iterator_tag;
        using value_tyep = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;

        ostream_iterator(std::ostream &s) : stream(&s), interval(nullptr) {}
        ostream_iterator(std::ostream &s, const char *c) : stream(&s), interval(c) {}

        ostream_iterator &operator=(const T &value) {
            *stream << value;
            if (interval) *stream << interval;
            return *this;
        }

        ostream_iterator &operator*() { return *this; }
        ostream_iterator &operator++() { return *this; }
        ostream_iterator &operator++(int) { return *this; }
    };


}

#endif //MINISTL_ITERATOR_ADAPTERS_H
