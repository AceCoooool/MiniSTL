#ifndef MINISTL_TEST_VECTOR_H
#define MINISTL_TEST_VECTOR_H

#include "stl_vector.imp.h"
#include <iostream>

using namespace std;

void test_vector() {
    cout << "---------------构造函数---------------" << endl;
    MiniSTL::vector<int> a(3);
    cout << "a(3)=0 0 0?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    MiniSTL::vector<int> b(3, 1);
    cout << "b(3, 1)=1 1 1?: ";
    for (auto t: b)
        cout << t << " ";
    cout << endl;
    MiniSTL::vector<int> c(b.begin(), b.end());
    cout << "c(b.begin(), b.end())=1 1 1?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    MiniSTL::vector<int> d{1, 2, 3};
    cout << "d{1, 2, 3}=1 2 3?: ";
    for (auto t: d)
        cout << t << " ";
    cout << endl;
    MiniSTL::vector<int> e(d);
    cout << "e(d)=1 2 3?: ";
    for (auto t: e)
        cout << t << " ";
    cout << endl;

    MiniSTL::vector<int> me(move(d));
    cout << "me(move(d))=1 2 3?: ";
    for (auto t: me)
        cout << t << " ";
    cout << endl;

    cout << "---------------复制操作---------------" << endl;
    MiniSTL::vector<int> f = e;
    cout << "(f=e)=1 2 3?: ";
    for (auto t: f)
        cout << t << " ";
    cout << endl;

    cout << "---------------修改容器操作---------------" << endl;
    f.insert(f.begin(), 4);
    cout << "(f.insert(f.begin(), 4))=4 1 2 3?: ";
    for (auto t: f)
        cout << t << " ";
    cout << endl;
    f.insert(f.begin(), c.begin(), c.end());
    cout << "(f.insert(f.begin(), c.begin(), c.end()))=1 1 1 4 1 2 3?: ";
    for (auto t: f)
        cout << t << " ";
    cout << endl;
    cout << "f.size()=7? : " << f.size() << endl;
    f.erase(f.begin() + 2, f.begin() + 4);
    cout << "(f.erase(f.begin()+2, f.begin()+4)=1 1 1 2 3?: ";
    for (auto t: f)
        cout << t << " ";
    cout << endl;
    f.push_back(8);
    cout << "(f.push_back(8))=1 1 1 2 3 8?: ";
    for (auto t: f)
        cout << t << " ";
    cout << endl;
    f.pop_back();
    cout << "(f.pop_back(8))=1 1 1 2 3?: ";
    for (auto t: f)
        cout << t << " ";
    cout << endl;
}

#endif //MINISTL_TEST_VECTOR_H
