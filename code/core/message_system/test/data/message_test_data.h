/********************************************************************/
/*                      Copyright 2026                              */
/*           Created and Maintained by Thomas Preisner              */
/********************************************************************/
#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

struct SimpleData {
    int value = 0;
    std::string name = "";
};

struct ComplexData {
    enum class EnumType {
        VAL_ONE,
        VAL_TWO,
        VAL_THREE,

        ENUM_COUNT
    };
    std::string uuid = "";
    std::vector<SimpleData> simple_vector;

    std::map<EnumType, std::string> mapping;
};
