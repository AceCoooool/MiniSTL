#ifndef MINISTL_TEST_HASH_MAP_H
#define MINISTL_TEST_HASH_MAP_H

#include "stl_hash_map.h"
#include "stl_hash_multimap.h"
#include <iostream>
#include <cstring>

using namespace std;

namespace MiniSTL {
    struct eqstr2 {
        bool operator()(const char *s1, const char *s2) const {
            return strcmp(s1, s2) == 0;
        }
    };

    void test_hash_map() {
        MiniSTL::hash_map<const char *, int, hash<const char *>, eqstr2> days;
        days["january"] = 31;
        days["february"] = 28;
        days["march"] = 31;
        days["april"] = 30;
        days["may"] = 31;
        days["june"] = 30;
        days["july"] = 31;
        days["august"] = 31;
        days["september"] = 30;
        days["october"] = 31;
        days["november"] = 30;
        days["december"] = 31;

        cout << "september->" << days["september"] << endl;
        cout << "june->" << days["june"] << endl;
        cout << "february->" << days["february"] << endl;
        cout << "december->" << days["december"] << endl;

        auto ite1 = days.begin();
        auto ite2 = days.end();
        for (; ite1 != ite2; ++ite1)
            cout << ite1->first << " ";
    }

    void test_hash_multimap() {
        MiniSTL::hash_multimap<const char *, int, hash<const char *>, eqstr2> days;
        days.insert({"january", 31});
        days.insert({"february", 28});
        days.insert({"march", 31});
        days.insert({"april", 30});
        days.insert({"may", 31});
        days.insert({"june", 30});
        days.insert({"july", 31});
        days.insert({"august", 31});
        days.insert({"september", 30});
        days.insert({"october", 31});
        days.insert({"november", 30});
        days.insert({"december", 31});

        cout << "september->" << (days.find("september"))->second << endl;
        cout << "june->" << (days.find("june"))->second << endl;
        cout << "february->" << (days.find("february"))->second << endl;
        cout << "december->" << (days.find("december"))->second << endl;

        auto ite1 = days.begin();
        auto ite2 = days.end();
        for (; ite1 != ite2; ++ite1)
            cout << ite1->first << " ";
    }
}

#endif //MINISTL_TEST_HASH_MAP_H
