#ifndef MINISTL_FUNCTION_ADAPTERS_H
#define MINISTL_FUNCTION_ADAPTERS_H

#include "stl_function.h"

namespace MiniSTL {
    /**
     * not1: 对返回值进行逻辑否定(一元)
     */
    template<class Predicate>
    class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
    protected:
        Predicate pred;

    public:
        explicit unary_negate(const Predicate &x) : pred(x) {}
        bool operator()(const typename Predicate::argument_type &x) const {
            return !pred(x);
        }
    };

    template<class Predicate>
    inline unary_negate<Predicate> not1(const Predicate &pred) {
        return unary_negate<Predicate>(pred);
    }

    /**
     * not2: 对返回值进行逻辑否定(二元)
     */
    template<class Predicate>
    class binary_negate : public binary_function<typename Predicate::first_argument_type,
            typename Predicate::second_argument_type, bool> {
    protected:
        Predicate pred;

    public:
        explicit binary_negate(const Predicate &x) : pred(x) {}
        bool operator()(const typename Predicate::first_argument_type &x,
                        const typename Predicate::second_argument_type &y) const {
            return !pred(x, y);
        }
    };

    template<class _Predicate>
    inline binary_negate<_Predicate> not2(const _Predicate &pred) {
        return binary_negate<_Predicate>(pred);
    }

    /**
     * bind1st: 对参数进行绑定(对第一个参数, 即第一个参数初始化后不变)
     */
    template<class Operation>
    class binder1st : public unary_function<typename Operation::second_argument_type,
            typename Operation::result_type> {
    protected:
        Operation op;
        typename Operation::first_argument_type value;

    public:
        binder1st(const Operation &x, const typename Operation::first_argument_type &y) : op(x), value(y) {}
        typename Operation::result_type operator()(const typename Operation::second_argument_type &x) const {
            return op(value, x);      // 调用表达式, 将value作为第一参数
        }
    };

    template<class Operation, class T>
    inline binder1st<Operation> bind1st(const Operation &op, const T &x) {
        using Arg1_type = typename Operation::first_argument_type;
        return binder1st<Operation>(op, static_cast<Arg1_type>(x));
    }

    /**
     * bind2nd: 对参数进行绑定(对第二个参数, 即第二个参数初始化后不变)
     */
    template<class Operation>
    class binder2nd : public unary_function<typename Operation::first_argument_type,
            typename Operation::result_type> {
    protected:
        Operation op;
        typename Operation::second_argument_type value;

    public:
        binder2nd(const Operation &x, const typename Operation::second_argument_type &y) : op(x), value(y) {}
        typename Operation::result_type operator()(const typename Operation::first_argument_type &x) const {
            return op(x, value);
        }
    };

    template<class Operation, class T>
    inline binder2nd<Operation> bind2nd(const Operation &op, const T &x) {
        using Arg2_type = typename Operation::second_argument_type;
        return binder2nd<Operation>(op, Arg2_type(x));
    }

    /**
     * compose1: 函数合成("合并两个函数---即op1(op2(x)")
     */
    template<class Operation1, class Operation2>
    class unary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type> {
    protected:
        Operation1 op1;
        Operation2 op2;

    public:
        unary_compose(const Operation1 &x, const Operation2 &y) : op1(x), op2(y) {}
        typename Operation1::result_type operator()(const typename Operation2::argument_type &x) const {
            return op1(op2(x));
        }
    };

    template<class Operation1, class Operation2>
    inline unary_compose<Operation1, Operation2> compose1(const Operation1 &op1, const Operation2 &op2) {
        return unary_compose<Operation1, Operation2>(op1, op2);
    }

    /**
     * compose2: 函数合成("合并三个函数---即op1(op2(x), op3(x))")
     */
    template<class Operation1, class Operation2, class Operation3>
    class binary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type> {
    protected:
        Operation1 op1;
        Operation2 op2;
        Operation3 op3;

    public:
        binary_compose(const Operation1 &x, const Operation2 &y, const Operation3 &z) : op1(x), op2(y), op3(z) {}
        typename Operation1::result_type operator()(const typename Operation2::argument_type &x) const {
            return op1(op2(x), op3(x));
        }
    };

    template<class Operation1, class Operation2, class Operation3>
    inline binary_compose<Operation1, Operation2, Operation3>
    compose2(const Operation1 &op1, const Operation2 &op2, const Operation3 &op3) {
        return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
    }

    /**
     * ptr_fun: 将一个一元指针包起来
     */
    template<class Arg, class Result>
    class pointer_to_unary_function : public unary_function<Arg, Result> {
    protected:
        Result (*ptr)(Arg);    // 内部成员, 一个函数指针
    public:
        pointer_to_unary_function() {}
        explicit pointer_to_unary_function(Result(*x)(Arg)) : ptr(x) {}
        Result operator()(Arg x) const { return ptr(x); }
    };

    template<class Arg, class Result>
    inline pointer_to_unary_function<Arg, Result> ptr_fun(Result(*x)(Arg)) {
        return pointer_to_unary_function<Arg, Result>(x);
    }

    /**
     * ptr_fun: 将一个二元指针包起来
     */
    template<class Arg1, class Arg2, class Result>
    class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result> {
    protected:
        Result (*ptr)(Arg1, Arg2);
    public:
        pointer_to_binary_function() {}
        explicit pointer_to_binary_function(Result(*x)(Arg1, Arg2)) : ptr(x) {}
        Result operator()(Arg1 x, Arg2 y) const {
            return ptr(x, y);
        }
    };

    template<class Arg1, class Arg2, class Result>
    inline pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(Result(*x)(Arg1, Arg2)) {
        return pointer_to_binary_function<Arg1, Arg2, Result>(x);
    }

    /**
     * mem_fun & mem_fun_ref: 成员函数指针(无参情况)
     */
    // 无参情况, 通过pointer调用, non-const成员函数
    template<class S, class T>
    class mem_fun_t : public unary_function<T *, S> {
    private:
        S (T::*f)();    // 内部成员指针, 指向成员函数

    public:
        explicit mem_fun_t(S(T::*pf)()) : f(pf) {}
        S operator()(T *p) const { return (p->*f)(); }
    };

    template<class S, class T>
    inline mem_fun_t<S, T> mem_fun(S (T::*f)()) {
        return mem_fun_t<S, T>(f);
    }

    // 无参情况, 通过reference调用, non-const成员函数
    template<class S, class T>
    class mem_fun_ref_t : public unary_function<T, S> {
    public:
        explicit mem_fun_ref_t(S(T::*pf)()) : f(pf) {}
        S operator()(T &r) const { return (r.*f)(); }
    private:
        S (T::*f)();
    };

    template<class S, class T>
    inline mem_fun_t<S, T> mem_fun_ref(S (T::*f)()) {
        return mem_fun_ref_t<S, T>(f);
    }

    // 无参情况, 通过pointer调用, const成员函数
    template<class S, class T>
    class const_mem_fun_t : public unary_function<const T *, S> {
    private:
        S (T::*f)() const;

    public:
        explicit const_mem_fun_t(S(T::*pf)() const) : f(pf) {}
        S operator()(const T *p) const { return (p->*f)(); }
    };

    template<class S, class T>
    inline const_mem_fun_t<S, T> mem_fun(S (T::*f)() const) {
        return const_mem_fun_t<S, T>(f);
    }

    // 无参情况, 通过reference调用, const成员函数
    template<class S, class T>
    class const_mem_fun_ref_t : public unary_function<const T *, S> {
    private:
        S (T::*f)() const;

    public:
        explicit const_mem_fun_ref_t(S(T::*pf)() const) : f(pf) {}
        S operator()(const T &r) const { return (r.*f)(); }
    };

    template<class S, class T>
    inline const_mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)() const) {
        return const_mem_fun_ref_t<S, T>(f);
    }

    /**
     * mem_fun1 & mem_fun1_ref: 有参情况
     */
    // 有参情况, 通过pointer调用, non-const成员函数
    template<class S, class T, class Arg>
    class mem_fun1_t : public binary_function<T *, Arg, S> {
    public:
        explicit mem_fun1_t(S(T::*pf)(Arg)) : f(pf) {}
        S operator()(T *p, Arg x) const { return (p->*f)(x); }
    private:
        S (T::*f)(Arg);
    };

    template<class S, class T, class A>
    inline mem_fun1_t<S, T, A> mem_fun1(S (T::*f)(A)) {
        return mem_fun1_t<S, T, A>(f);
    }

    // 有参情况, 通过reference调用, non-const成员函数
    template<class S, class T, class Arg>
    class mem_fun1_ref_t : public binary_function<T *, Arg, S> {
    public:
        explicit mem_fun1_ref_t(S(T::*pf)(Arg)) : f(pf) {}
        S operator()(T &r, Arg x) const { return (r.*f)(x); }
    private:
        S (T::*f)(Arg);
    };

    template<class S, class T, class A>
    inline mem_fun1_ref_t<S, T, A> mem_fun1_ref(S (T::*f)(A)) {
        return mem_fun1_t<S, T, A>(f);
    }

    // 有参情况, 通过pointer调用, const成员函数
    template<class S, class T, class Arg>
    class const_mem_fun1_t : public binary_function<T *, Arg, S> {
    public:
        explicit const_mem_fun1_t(S(T::*pf)(Arg) const) : f(pf) {}
        S operator()(const T *p, Arg x) const { return (p->*f)(x); }
    private:
        S (T::*f)(Arg) const;
    };

    template<class S, class T, class A>
    inline const_mem_fun1_t<S, T, A> mem_fun1(S (T::*f)(A) const) {
        return const_mem_fun1_t<S, T, A>(f);
    }

    // 有参情况, 通过reference调用, const成员函数
    template<class S, class T, class Arg>
    class const_mem_fun1_ref_t : public binary_function<T *, Arg, S> {
    public:
        explicit const_mem_fun1_ref_t(S(T::*pf)(Arg) const) : f(pf) {}
        S operator()(const T &r, Arg x) const { return (r.*f)(x); }
    private:
        S (T::*f)(Arg) const;
    };

    template<class S, class T, class A>
    inline const_mem_fun1_ref_t<S, T, A> mem_fun1_ref(S (T::*f)(A) const) {
        return const_mem_fun1_t<S, T, A>(f);
    }

}

#endif //MINISTL_FUNCTION_ADAPTERS_H
