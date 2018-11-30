#ifndef MINISTL_TEST_STACK_H
#define MINISTL_TEST_STACK_H

#include "stl_stack.h"
#include <iostream>

using namespace std;

void test_stack() {
    MiniSTL::stack<int> a;
    a.push(10);
    cout << "(a.top() {after a.push(10)} = 10? " << a.top() << endl;
    a.push(20);
    cout << "(a.top() {after a.push(20)} = 20? " << a.top() << endl;
    a.pop();
    cout << "(a.top() {after a.pop()} = 10? " << a.top() << endl;
    cout << "(a.size() = 1? " << a.size() << endl;
    MiniSTL::stack<int> b;
    b.push(10);
    cout << "(a(a=10)==b(b=10))=1? " << (a == b) << endl;
}

#endif //MINISTL_TEST_STACK_H
