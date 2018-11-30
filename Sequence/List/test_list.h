#ifndef MINISTL_TEST_LIST_H
#define MINISTL_TEST_LIST_H

#include "stl_list.imp.h"
#include <iostream>

using namespace std;

void test_list() {
    cout << "---------------构造函数---------------" << endl;
    MiniSTL::list<int> a({1, 2, 3});
    cout << "a({1, 2, 3})=1 2 3?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;

    MiniSTL::list<int> b(a);
    cout << "b(a)=1 2 3?: ";
    for (auto t: b)
        cout << t << " ";
    cout << endl;

    MiniSTL::list<int> c = b;
    cout << "(c=b)=1 2 3?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;

    cout << "---------------插入操作---------------" << endl;
    c.insert(c.begin(), 6);
    cout << "(c.insert(c.begin(), 6))=6 1 2 3?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;

    c.insert(c.begin(), b.begin(), b.end());
    cout << "(c.insert(c.begin(), b.begin(), b.end()))=1 2 3 6 1 2 3?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;

    cout << "---------------删除操作---------------" << endl;
    c.erase(c.begin());
    cout << "(c.erase(c.begin()))=2 3 6 1 2 3?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;

    cout << "---------------push&pop操作---------------" << endl;
    c.pop_back();
    cout << "(c.pop_back())=2 3 6 1 2?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    c.pop_fornt();
    cout << "(c.pop_front())=3 6 1 2?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    c.push_back(7);
    cout << "(c.push_back(7))=3 6 1 2 7?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    c.push_front(4);
    cout << "(c.push_front(4))=4 3 6 1 2 7?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;

    cout << "---------------others操作---------------" << endl;
    c.sort();
    cout << "(c.sort())=1 2 3 4 6 7?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    c.push_front(1);
    c.unique();
    cout << "(c.push_front(1)+c.unique())=1 2 3 4 6 7?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    c.remove(4);
    cout << "(c.remove(4))=1 2 3 6 7?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    MiniSTL::list<int> d{1, 4, 6, 8};
    c.merge(d);
    cout << "(c.merge({1, 4, 6, 8}))=1 1 2 3 4 6 6 7 8?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    c.reverse();
    cout << "(c.reverse)=8 7 6 6 4 3 2 1 1?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;

    cout << "---------------容量操作---------------" << endl;
    cout << "c.size() = 9?:  " << c.size() << endl;
};

#endif //MINISTL_TEST_LIST_H
