//
// Created by qiyu on 4/24/18.
//
#include <iostream>
#include "feather.h"

struct person{
    std::string name;
    int age;
    std::string address;
};

REFLECTION(person, name, age, address);

void test_struct_to_json(){
    person p{"aa", 20, "zhu"};
    auto o = feather::struct_to_json(p);
    for (auto it = o.begin(); it != o.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << "\n";
    }
}

int main(){
    test_struct_to_json();

    return 0;
}