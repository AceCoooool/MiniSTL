#ifndef MINISTL_TEST_PRIORITY_QUEUE_H
#define MINISTL_TEST_PRIORITY_QUEUE_H

#include "stl_priority_queue.h"
#include <iostream>

using namespace std;

namespace MiniSTL {
    void test_priority_queue() {
        int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
        cout << "ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5}";
        MiniSTL::priority_queue<int> ipq(ia, ia + 9);
        cout << "ipq.size()(after priority_queue<int> ipq(ia, ia + 9))=9 ?: " << ipq.size() << endl;
        cout << "ipq.top()=9?: " << ipq.top() << endl;
        cout << "pop all the ipq elements = 9 8 5 4 3 3 2 1 0?: ";
        while (!ipq.empty()) {
            cout << ipq.top() << " ";
            ipq.pop();
        }
        cout << endl;
    }
}

#endif //MINISTL_TEST_PRIORITY_QUEUE_H
