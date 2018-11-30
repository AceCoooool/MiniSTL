#ifndef MINISTL_TYPE_TRAITS_H
#define MINISTL_TYPE_TRAITS_H

namespace MiniSTL {

    // integral_constant
    template<typename _Tp, _Tp __v>
    struct _integral_constant {
        static constexpr _Tp value = __v;
    };

    using _true_type = _integral_constant<bool, true>;
    using _false_type = _integral_constant<bool, false>;


    template<typename, typename>
    struct _is_same : public _false_type {};

    template<typename T>
    struct _is_same<T, T> : public _true_type {};


    // 萃取传入的T类型的类型特性
    template<class T>
    struct _type_traits {
        using has_trivial_default_constructor = _false_type;
        using has_trivial_copy_constructor = _false_type;
        using has_trivial_assignment_operator = _false_type;
        using has_trivial_destructor = _false_type;
        using is_POD_type = _false_type;
    };

    // 特化
    template<>
    struct _type_traits<bool> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<char> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };
    template<>
    struct _type_traits<unsigned char> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<signed char> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };
    template<>
    struct _type_traits<wchar_t> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<short> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<unsigned short> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<int> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<unsigned int> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<long> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<unsigned long> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<long long> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<unsigned long long> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<float> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<double> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<long double> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<class T>
    struct _type_traits<T *> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<class T>
    struct _type_traits<const T *> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<char *> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<unsigned char *> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<signed char *> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<const char *> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<const unsigned char *> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    template<>
    struct _type_traits<const signed char *> {
        using has_trivial_default_constructor = _true_type;
        using has_trivial_copy_constructor = _true_type;
        using has_trivial_assignment_operator = _true_type;
        using has_trivial_destructor = _true_type;
        using is_POD_type = _true_type;
    };

    // C++14: easy alias typename
    template<typename T>
    using has_trivial_default_constructor_t = typename _type_traits<T>::has_trivial_default_constructor;

    template<typename T>
    using has_trivial_copy_constructor_t = typename _type_traits<T>::has_trivial_copy_constructor;

    template<typename T>
    using has_trivial_assignment_operator_t = typename _type_traits<T>::has_trivial_assignment_operator;

    template<typename T>
    using has_trivial_destructor_t = typename _type_traits<T>::has_trivial_destructor;

    template<typename T>
    using is_POD_type_t = typename _type_traits<T>::is_POD_type;
}

#endif //MINISTL_TYPE_TRAITS_H
