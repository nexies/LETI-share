//
// Created by nexie on 14.06.2025.
//

#ifndef NX_TYPE_TRAITS_H
#define NX_TYPE_TRAITS_H

#include <type_traits>

namespace nx {
    template <typename _Tp>
    struct is_char_type : std::false_type {};

    template <> struct is_char_type<char> : std::true_type {};
    template <> struct is_char_type<wchar_t> : std::true_type {};
    template <> struct is_char_type<char8_t> : std::true_type {};
    template <> struct is_char_type<char16_t> : std::true_type {};
    template <> struct is_char_type<char32_t> : std::true_type {};

    template <typename _Tp>
    struct is_char_type_t : is_char_type<_Tp> {};

}

#endif //NX_TYPE_TRAITS_H
