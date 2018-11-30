#ifndef MINISTL_TEST_QUEUE_H
#define MINISTL_TEST_QUEUE_H

#include "stl_queue.h"
#include <iostream>

using namespace std;

void test_queue() {
    MiniSTL::queue<int> a;
    a.push(10);
    cout << "(a.front() {after a.push(10)} = 10? " << a.front() << endl;
    a.push(20);
    cout << "(a.front() {after a.push(20)} = 10? " << a.front() << endl;
    a.pop();
    cout << "(a.front() {after a.pop()} = 20? " << a.front() << endl;
    cout << "(a.size() = 1? " << a.size() << endl;
    MiniSTL::queue<int> b;
    b.push(10);
    cout << "(a(a=20)==b(b=10))=0? " << (a == b) << endl;
}

#endif //MINISTL_TEST_QUEUE_H
