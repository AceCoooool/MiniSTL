#ifndef MINISTL_TEST_FUNCTION_H
#define MINISTL_TEST_FUNCTION_H

#include "stl_function.h"
#include "function_adapters.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

void test_functor() {
    cout << "---------------------------算数类仿函数------------------------" << endl;
    MiniSTL::plus<int> plusobj;
    MiniSTL::minus<int> minusobj;
    MiniSTL::multiplies<int> multipliesobj;
    MiniSTL::divides<int> dividesobj;
    MiniSTL::modulus<int> modulusobj;
    MiniSTL::negate<int> negateobj;

    cout << "plusobj(3, 5)=8?: " << plusobj(3, 5) << endl;
    cout << "minusobj(3, 5)=-2?: " << minusobj(3, 5) << endl;
    cout << "multipliesobj(3, 5)=15?: " << multipliesobj(3, 5) << endl;
    cout << "dividesobj(3, 5)=0?: " << dividesobj(3, 5) << endl;
    cout << "modulusobj(3, 5)=3?: " << modulusobj(3, 5) << endl;
    cout << "negateobj(3)=-3?: " << negateobj(3) << endl;

    cout << "-------------------------运算关系类仿函数-----------------------" << endl;
    cout << "MiniSTL::equal_to<int>()(3, 5)=0?: " << MiniSTL::equal_to<int>()(3, 5) << endl;
    cout << "MiniSTL::not_equal_to<int>()(3, 5)=1?: " << MiniSTL::not_equal_to<int>()(3, 5) << endl;
    cout << "MiniSTL::greater<int>()(3, 5)=0?: " << MiniSTL::greater<int>()(3, 5) << endl;
    cout << "MiniSTL::greater_equal<int>()(3, 5)=0?: " << MiniSTL::greater_equal<int>()(3, 5) << endl;
    cout << "MiniSTL::less<int>()(3, 5)=1?: " << MiniSTL::less<int>()(3, 5) << endl;
    cout << "MiniSTL::less_equal<int>()(3, 5) =1?: " << MiniSTL::less_equal<int>()(3, 5) << endl;

    cout << "---------------------------逻辑运算类仿函数------------------------" << endl;
    cout << "MiniSTL::logical_and<int>()(true, true)=1?: " << MiniSTL::logical_and<int>()(true, true) << endl;
    cout << "MiniSTL::logical_or<int>()(true, false)=1?: " << MiniSTL::logical_or<int>()(true, false);
    cout << "MiniSTL::logical_not<int>()(true)=0?: " << MiniSTL::logical_not<int>()(true) << endl;

    cout << "------------------------------其他仿函数---------------------------" << endl;
    cout << "MiniSTL::identity<int>()(10)=10?: " << MiniSTL::identity<int>()(10) << endl;
    cout << "MiniSTL::select1st<std::pair<int, int>>()({2, 3})=2?: "
         << MiniSTL::select1st<std::pair<int, int>>()({2, 3}) << endl;
    cout << "MiniSTL::select2nd<std::pair<int, int>>()({2, 3})=3?: "
         << MiniSTL::select2nd<std::pair<int, int>>()({2, 3}) << endl;
    cout << "MiniSTL::project1st<int, double>()(2, 3.0)=2?: "
         << MiniSTL::project1st<int, double>()(2, 3.0) << endl;
    cout << "MiniSTL::project2nd<int, double>()(2, 3.0)=3.0?: "
         << MiniSTL::project2nd<int, double>()(2, 3.0) << endl;
}

struct LessThan3 : std::unary_function<int, bool> {
    bool operator()(int i) const { return i < 3; }
};

struct Equal : std::binary_function<int, int, bool> {
    bool operator()(int i, int j) const { return i == j; }
};

struct f : std::binary_function<int, int, int> {
    int operator()(int i, int j) const { return i + j; }
};

struct f1 : std::unary_function<int, int> {
    int operator()(int i) const { return i + 2; }
};

struct f2 : std::unary_function<int, int> {
    int operator()(int i) const { return i * 2; }
};

int func1(int x) {
    return x + 2;
}

int func(int x, int y) {
    return x + y;
}

class Shape {
public:
    virtual void display() = 0;   // 纯虚函数
};

class Rect : public Shape {
public:
    virtual void display() {
        cout << "Rect ";
    }
};

class Circle : public Shape {
public:
    virtual void display() {
        cout << "Circle ";
    }
};

class Square : public Shape {
public:
    virtual void display() {
        cout << "Square ";
    }
};

void test_function_adapters() {
    cout << "-----------------not--------------------" << endl;
    vector<int> a = {1, 2, 3, 4, 5, 6};
    cout << "vector<int> a = {1, 2, 3, 4, 5, 6}" << endl;
    auto n1 = std::count_if(a.begin(), a.end(), MiniSTL::not1(LessThan3()));
    cout << "count_if(a.begin(), a.end(), MiniSTL::not1(LessThan3())=4?: " << n1 << endl;
    vector<int> b = {1, 2, 4, 3, 5, 5};
    cout << "vector<int> b = {1, 2, 4, 3, 5, 5}" << endl;
    vector<int> c(6);
    std::transform(a.begin(), a.end(), b.begin(), c.begin(), MiniSTL::not2(Equal()));
    cout << "c(after transform(a.begin(), a.end(), b.begin(), c.begin(), MiniSTL::not2(Equal())))=0 0 1 1 0 1?: ";
    for (auto t: c) cout << t << " ";
    cout << endl;

    cout << "-----------------bind--------------------" << endl;
    vector<double> v1 = {0, 30, 45, 60, 90, 180};
    cout << "vector<double> v1 = {0, 30, 45, 60, 90, 180}" << endl;
    vector<double> r1(v1.size());
    double pi = std::acos(-1);
    std::transform(v1.begin(), v1.end(), r1.begin(), MiniSTL::bind1st(multiplies<double>(), pi / 180));
    cout << "r1(after transform(v1.begin(), v1.end(), r1.begin(), MiniSTL::bind1st(multiplies<double>(), pi / 180))="
            "0 0.523599 0.785398 1.0472 1.5708 3.14159?: ";
    for (auto r: r1) cout << r << " ";
    cout << endl;
    vector<int> v2 = {1, 2, 3, 4, 5};
    cout << "vector<int> v2 = {1, 2, 3, 4, 5}" << endl;
    vector<int> r2(v2.size());
    std::transform(v2.begin(), v2.end(), r2.begin(), MiniSTL::bind2nd(multiplies<int>(), 2));
    cout << "r2(after std::transform(v2.begin(), v2.end(), r2.begin(), MiniSTL::bind2nd(multiplies<int>(), 2)))="
            "2 4 6 8 10?: ";
    for (auto v: r2) cout << v << " ";
    cout << endl;

    cout << "-----------------compose--------------------" << endl;
    vector<int> v3 = {1, 2, 3, 4, 5};
    cout << "vector<int> v3 = {1, 2, 3, 4, 5}" << endl;
    cout << "f = (int x, int y) { return x + y; }" << endl;
    cout << "f1 = (int x) { return x + 2; }" << endl;
    cout << "f2 = (int x) { return x * 2; }" << endl;
    vector<int> r3(v3.size());
    std::transform(v3.begin(), v3.end(), r3.begin(), MiniSTL::compose1(f1(), f2()));
    cout << "r3(after transform(v3.begin(), v3.end(), r3.begin(), MiniSTL::compose1(f1(), f2())))="
            "4 6 8 10 12?: ";
    for (auto r: r3) cout << r << " ";
    cout << endl;
    vector<int> r4(v3.size());
    std::transform(v3.begin(), v3.end(), r4.begin(), MiniSTL::compose2(f(), f1(), f2()));
    cout << "r4(after transform(v3.begin(), v3.end(), r3.begin(), MiniSTL::compose2(f(), f1(), f2())))="
            "5 8 11 14 17?: ";
    for (auto r: r4) cout << r << " ";
    cout << endl;

    cout << "-----------------ptr_fun--------------------" << endl;
    std::transform(v3.begin(), v3.end(), r3.begin(), MiniSTL::pointer_to_unary_function(func1));
    cout << "r3(after transform(v3.begin(), v3.end(), r3.begin(), MiniSTL::pointer_to_unary_function(func1)))="
            "3 4 5 6 7?: ";
    for (auto v: r3) cout << v << " ";
    cout << endl;
    std::transform(v3.begin(), v3.end(), r3.begin(), MiniSTL::compose2(MiniSTL::pointer_to_binary_function(func),
                                                                       f1(), f2()));
    cout << "r3(after transform(v3.begin(), v3.end(), r4.begin(), MiniSTL::compose2("
            "MiniSTL::pointer_to_binary_function(func), f1(), f2())))=5 8 11 14 17??: ";
    for (auto v: r3) cout << v << " ";
    cout << endl;

    cout << "-----------------mem_fun--------------------" << endl;
    vector<Shape *> V;
    V.push_back(new Rect);
    V.push_back(new Circle);
    V.push_back(new Square);
    V.push_back(new Circle);
    V.push_back(new Rect);
    std::for_each(V.begin(), V.end(), MiniSTL::mem_fun(&Shape::display));
    cout << endl;

};


#endif //MINISTL_TEST_FUNCTION_H
