#ifndef MINISTL_STL_FUNCTION_H
#define MINISTL_STL_FUNCTION_H

namespace MiniSTL {
    // 一元运算符
    template<class Arg, class Result>
    struct unary_function {
        using argument_type = Arg;
        using result_type = Result;
    };

    // 二元操作结构定义
    template<class Arg1, class Arg2, class Result>
    struct binary_function {
        using first_argument_type = Arg1;
        using second_argument_type = Arg2;
        using result_type = Result;
    };

    /**---------------------------算数类仿函数-------------------------**/
    // 加法
    template<class T>
    struct plus : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const { return x + y; }
    };

    // 减法
    template<class T>
    struct minus : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const { return x - y; }
    };

    // 乘法
    template<class T>
    struct multiplies : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const { return x * y; }
    };

    // 除法
    template<class T>
    struct divides : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const { return x / y; }
    };

    // 取余
    template<class T>
    struct modulus : public binary_function<T, T, T> {
        T operator()(const T &x, const T &y) const { return x % y; }
    };

    // 取负
    template<class T>
    struct negate : public unary_function<T, T> {
        T operator()(const T &x) const { return -x; }
    };


    /**---------------------------运算关系类仿函数------------------------**/
    // 等于
    template<class T>
    struct equal_to : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const { return x == y; }
    };

    // 不等于
    template<class T>
    struct not_equal_to : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const { return x != y; }
    };

    // 大于
    template<class T>
    struct greater : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const { return x > y; }
    };

    // 小于
    template<class T>
    struct less : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const { return x < y; }
    };

    // 大于等于
    template<class T>
    struct greater_equal : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const { return x >= y; }
    };

    // 小于等于
    template<class T>
    struct less_equal : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const { return x <= y; }
    };

    /**---------------------------逻辑运算类仿函数------------------------**/
    // AND
    template<class T>
    struct logical_and : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const { return x && y; }
    };

    // OR
    template<class T>
    struct logical_or : public binary_function<T, T, bool> {
        bool operator()(const T &x, const T &y) const { return x || y; }
    };

    // NOT
    template<class T>
    struct logical_not : public unary_function<T, bool> {
        bool operator()(const T &x) const { return !x; }
    };

    /**------------------------------其他仿函数---------------------------**/
    // 证同: 任何数值通过此函数后, 不会有任何改变
    template<class T>
    struct identity : public unary_function<T, T> {
        const T &operator()(const T &x) const { return x; }
    };

    // 选择: 接受一个pair, 返回其第一个元素
    template<class Pair>
    struct select1st : public unary_function<Pair, typename Pair::first_type> {
        const typename Pair::first_type &operator()(const Pair &x) const {
            return x.first;
        }
    };

    // 选择: 接受一个pair, 返回其第二个元素
    template<class Pair>
    struct select2nd : public unary_function<Pair, typename Pair::second_type> {
        const typename Pair::second_type &operator()(const Pair &x) const {
            return x.second;
        }
    };

    // 投射: 输入两个参数, 传回第一个参数, 忽略第二个参数
    template<class Arg1, class Arg2>
    struct project1st : public binary_function<Arg1, Arg2, Arg1> {
        Arg1 operator()(const Arg1 &x, const Arg2 &) const { return x; }
    };

    // 投射: 输入两个参数, 传回第二个参数, 忽略第一个参数
    template<class Arg1, class Arg2>
    struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
        Arg2 operator()(const Arg1 &, const Arg2 &y) const { return y; }
    };

}

#endif //MINISTL_STL_FUNCTION_H
