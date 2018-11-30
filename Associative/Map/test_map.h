#ifndef MINISTL_TEST_MAP_H
#define MINISTL_TEST_MAP_H

#include "stl_map.imp.h"
#include <iostream>

using namespace std;

namespace MiniSTL {
    void test_map() {
        MiniSTL::map<string, int> simap;
        simap["jjhou"] = 1;
        simap["jerry"] = 2;
        simap["jason"] = 3;
        simap["jimmy"] = 4;
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
        int number = simap["jjhou"];
        cout << "number of (simap[\"jjhou\"])=1?: " << number << endl;

        auto ite1 = simap.find("mchen");
        if (ite1 == simap.end())
            cout << "mchen not found" << endl;

        ite1 = simap.find("jerry");
        if (ite1 != simap.end())
            cout << "jerry found" << endl;
        ite1->second = 9;
        cout << "value of jerry(after change)=9?: " << simap["jerry"] << endl;

    }
}

#endif //MINISTL_TEST_MAP_H
