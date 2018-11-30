#ifndef MINISTL_TEST_DEQUE_H
#define MINISTL_TEST_DEQUE_H

#include "stl_deque.imp.h"
#include <iostream>

using namespace std;

void test_deque() {
    cout << "---------------构造函数---------------" << endl;
    MiniSTL::deque<int> a(3, 2);
    cout << "a(3, 2)=2 2 2?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    MiniSTL::deque<int> b(a.begin(), a.end());
    cout << "b(a.begin(), a.end())=2 2 2?: ";
    for (auto t: b)
        cout << t << " ";
    cout << endl;
    MiniSTL::deque<int> c({1, 2, 3});
    cout << "c({1, 2, 3})=1 2 3?: ";
    for (auto t: c)
        cout << t << " ";
    cout << endl;
    MiniSTL::deque<int> d(move(c));
    cout << "d(move(c))=1 2 3?: ";
    for (auto t: d)
        cout << t << " ";
    cout << endl;
    cout << "---------------复制函数---------------" << endl;
    MiniSTL::deque<int> e = d;
    cout << "(e=d) = 1 2 3?: ";
    for (auto t: e)
        cout << t << " ";
    cout << endl;
    MiniSTL::deque<int> f = {2, 3, 4};
    cout << "(f = {2, 3, 4}) = 2 3 4?: ";
    for (auto t: f)
        cout << t << " ";
    cout << endl;
    MiniSTL::deque<int> g;
    g = move(f);
    cout << "(g = move(f)) = 2 3 4?: ";
    for (auto t: g)
        cout << t << " ";
    cout << endl;
    cout << "---------------修改容器---------------" << endl;
    a.push_back(6);
    cout << "a.push_back(6)=2 2 2 6?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    a.push_front(3);
    cout << "a.push_front(3)=3 2 2 2 6?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    a.push_front(4);
    cout << "a.push_front(4)=4 3 2 2 2 6?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    a.pop_front();
    cout << "a.pop_front()=3 2 2 2 6?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    a.push_back(7);
    cout << "a.push_back(7)=3 2 2 2 6 7?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    a.pop_back();
    cout << "a.pop_back()=3 2 2 2 6?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    cout << "---------------访问元素---------------" << endl;
    cout << "(a[2])=2?: " << a[2] << endl;
    cout << "a.front()=3?: " << a.front() << endl;
    cout << "a.back()=6?: " << a.back() << endl;
    cout << "---------------插入和删除元素---------------" << endl;
    a.insert(a.begin() + 2, 8);
    cout << "a.insert(a.begin()+2, 8)=3 2 8 2 2 6?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    a.erase(a.begin() + 1);
    cout << "a.erase(a.begin()+1)=3 8 2 2 6?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
    a.erase(a.begin() + 1, a.begin() + 3);
    cout << "a.erase(a.begin()+1, a.begin()+3)=3 2 6?: ";
    for (auto t: a)
        cout << t << " ";
    cout << endl;
}

#endif //MINISTL_TEST_DEQUE_H
