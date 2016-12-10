// Macros for declaring enums in C++ that include to_string conversion and looping through all enum values
// https://github.com/therocode/smartenum

// File modified to add count and remove non class enum and add conversion functions to avoid writing cast

/* EXAMPLE

smart_enum(Day, Mon, Tue, Wed, Thu, Fri, Sat, Sun);

int main()
{
    std::cout << to_string(Day::Mon) << " " << to_string(Day::Tue) << std::endl;
    std::cout << Day::Wed << " " << Day::Thu << std::endl;
    std::cout << "The dishes we have are :\n";
    for (auto d : Day_list)
    {
        std::cout << d << " ";
    }
    std::cout << std::endl;
    std::cout << "Number of days : " << Day_count << std::endl;
    return 0;
}

*/

/*
The MIT License (MIT)

Copyright (c) 2015 Tobias Widlund

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SMARTENUM_HPP
#define SMARTENUM_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <iostream>

namespace smart_enum
{
    inline std::string trimWhitespace(std::string str)
    {
        // trim trailing whitespace
        size_t endPos = str.find_last_not_of(" \t");
        if(std::string::npos != endPos)
        {
            str = str.substr( 0, endPos + 1);
        }

        // trim leading spaces
        size_t startPos = str.find_first_not_of(" \t");
        if(std::string::npos != startPos)
        {
            str = str.substr(startPos);
        }

        return str;
    }

    inline std::string extractEntry(std::string& valuesString)
    {
        std::string result;
        size_t nextCommaPos = valuesString.find(',');

        if(nextCommaPos != std::string::npos)
        {
            std::string segment = valuesString.substr(0, nextCommaPos);
            valuesString.erase(0, nextCommaPos + 1);
            result = trimWhitespace(segment);
        }
        else
        {
            result = trimWhitespace(valuesString);
            valuesString = "";
        };
        return result;
    };

    inline std::unordered_map<int32_t, std::string> makeEnumNameMap(std::string enumValuesString)
    {
        std::unordered_map<int32_t, std::string> nameMap;

        int32_t currentEnumValue = 0;
        while(enumValuesString != "")
        {
            std::string currentEnumEntry = extractEntry(enumValuesString);

            size_t equalSignPos = currentEnumEntry.find('=');
            if(equalSignPos != std::string::npos)
            {
                std::string rightHandSide = currentEnumEntry.substr(equalSignPos + 1);
                currentEnumValue = std::stoi(rightHandSide);
                currentEnumEntry.erase(equalSignPos);
            }

            currentEnumEntry = trimWhitespace(currentEnumEntry);

            nameMap[currentEnumValue] = currentEnumEntry;

            currentEnumValue++;
        }

        return nameMap;
    }

    template<typename Type>
    std::vector<Type> makeEnumList(std::string enumValuesString)
    {
        std::vector<Type> enumList;

        int32_t currentEnumValue = 0;
        while(enumValuesString != "")
        {
            std::string currentEnumEntry = extractEntry(enumValuesString);

            size_t equalSignPos = currentEnumEntry.find('=');
            if(equalSignPos != std::string::npos)
            {
                std::string rightHandSide = currentEnumEntry.substr(equalSignPos + 1);
                currentEnumValue = std::stoi(rightHandSide);
                currentEnumEntry.erase(equalSignPos);
            }

            currentEnumEntry = trimWhitespace(currentEnumEntry);

            enumList.push_back(static_cast<Type>(currentEnumValue));

            currentEnumValue++;
        }

        return enumList;
    }
}

#define smart_enum(Type, ...) enum class Type { __VA_ARGS__}; \
    static std::unordered_map<int32_t, std::string> Type##_enum_names = smart_enum::makeEnumNameMap(#__VA_ARGS__);\
    static std::vector<Type> Type##_list = smart_enum::makeEnumList<Type>(#__VA_ARGS__);\
    static std::size_t Type##_count = smart_enum::makeEnumList<Type>(#__VA_ARGS__).size();\
    \
    inline const int32_t to_int(Type value) \
    { \
        return (int32_t)value; \
    } \
    \
    inline const std::string& to_string(Type value) \
    { \
        return Type##_enum_names.at(to_int(value));\
    } \
    \
    template <typename T> \
    inline const Type to_enum(T value) \
    { \
        return (Type)value; \
    } \
    \
    inline std::ostream& operator<<(std::ostream& outStream, Type value)\
    {\
        outStream << to_string(value);\
        return outStream;\
    }


#endif
