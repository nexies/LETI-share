//
// Created by nexie on 12.06.2025.
//

#ifndef NX_IOSTREAM_H
#define NX_IOSTREAM_H

// #include "nx_vector.h"
// #include "nx_string.h"
// #include "nx_list.h"
#include "nx_string_extension.h"

#include <iostream>

namespace nx {
    // std::basic_istream<char> & getline (std::basic_istream<char> & stream, string & str, char sep);
}

// template <typename _Tp>
// inline std::basic_ostream<char> & operator << (std::basic_ostream<char> & stream, const nx::list<_Tp> & list)
//     {
//         stream << "{";
//         auto pre_last = --list.end();
//         for (auto it = list.begin(); it != pre_last; ++it) {
//             stream << *it << ", ";
//         }
//         stream << list.back() << "}";
//         return stream;
//     }

// inline std::basic_ostream<char> & operator << (std::basic_ostream<char> & stream, const nx::string & str)
// {
//     return stream << str.c_str();
// }
//
// inline std::basic_ostream<wchar_t> & operator << (std::basic_ostream<wchar_t> & stream, const nx::wstring & str)
// {
//     return stream << str.c_str();
// }
//
// inline std::basic_ostream<char> & operator << (std::basic_ostream<char> & stream, const nx::string_list & list)
// {
//     stream << "{";
//     auto pre_last = --list.end();
//     for (auto it = list.begin(); it != pre_last; ++it) {
//         stream << "\"" << *it << "\", ";
//     }
//     stream << "\"" <<list.back() << "\"" << "}";
//     return stream;
// }

// inline std::basic_ostream<wchar_t> & operator << (std::basic_ostream<wchar_t> & stream, const nx::wstring_list & list)
// {
//     stream << L"{";
//     auto pre_last = --list.end();
//     for (auto it = list.begin(); it != pre_last; ++it) {
//         stream << L"\"" << *it << L"\", ";
//     }
//     stream << L"\"" <<list.back() << L"\"" << L"}";
//     return stream;
// }

// inline std::basic_istream<char> & operator >> (std::basic_istream<char> & stream, nx::string & str)
// {
//     str.clear();
//     char ch;
     // while (stream.get(ch) && std::isspace(static_cast<unsigned char>(ch))) {}
//     if (!stream) return stream;
//     str.push_back(ch);
//     while (stream.get(ch) && !std::isspace(static_cast<unsigned char>(ch)))
//         str.push_back(ch);
//     if (stream && std::isspace(static_cast<unsigned char>(ch)))
//         stream.unget();
//     return stream;
// }

template <typename _Tcontainer,
std::enable_if_t<nx::is_char_type<typename _Tcontainer::value_type>::value, bool> = true>
inline std::basic_ostream<typename _Tcontainer::value_type> & operator <<
    (std::basic_ostream<typename _Tcontainer::value_type> & stream,
        const nx::basic_string_extension<_Tcontainer> & string) {
    return stream << string.c_str();
}

template <typename _Tcontainer,
std::enable_if_t<nx::is_char_type<typename _Tcontainer::value_type>::value, bool> = true>
inline std::basic_istream<typename _Tcontainer::value_type> & operator >>
(std::basic_istream<typename _Tcontainer::value_type> & stream, nx::basic_string_extension<_Tcontainer> & str)
{
    str.clear();
    char ch;
    while (stream.get(ch) && std::isspace(static_cast<unsigned char>(ch))) {}
    if (!stream) return stream;
    str.push_back(ch);
    while (stream.get(ch) && !std::isspace(static_cast<unsigned char>(ch)))
        str.push_back(ch);
    if (stream && std::isspace(static_cast<unsigned char>(ch)))
        stream.unget();
    return stream;
}

// template <typename _Tp>
// inline std::basic_ostream<char> & operator << (std::basic_ostream<char> & stream, const nx::vector<_Tp> & vec)
// {
//     stream << "{";
//     auto prelast = vec.size() - 1;
//     for (auto i = 0; i < prelast; ++i)
//         stream << vec.at(i) << ", ";
//     stream << vec.back() << "}";
//     return stream;
// }
#endif //NX_IOSTREAM_H
