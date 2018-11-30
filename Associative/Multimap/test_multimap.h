
#ifndef MINISTL_TEST_MULTIMAP_H
#define MINISTL_TEST_MULTIMAP_H

#include "stl_multimap.imp.h"
#include <iostream>

using namespace std;

namespace MiniSTL {
    void test_multimap() {
        MiniSTL::multimap<string, int> simap = {{"jjhou", 1},
                                                {"jerry", 2},
                                                {"jason", 3},
                                                {"jimmy", 4}};
        cout << "simap = ";
        for (auto t: simap)
            cout << "[" << t.first << " " << t.second << "]  ";
        cout << endl;

        std::pair<string, int> value("david", 5);
        simap.insert(value);
        cout << "simap(after insert (david, 5)) = ";
        for (auto t: simap)
            cout << "[" << t.first << " " << t.second << "]  ";
        cout << endl;

        auto ite1 = simap.find("mchen");
        if (ite1 == simap.end())
            cout << "mchen not found" << endl;

        ite1 = simap.find("jerry");
        if (ite1 != simap.end())
            cout << "jerry found" << endl;
    }
}

#endif //MINISTL_TEST_MULTIMAP_H
