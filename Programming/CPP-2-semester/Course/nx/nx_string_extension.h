//
// Created by nexie on 18.06.2025.
//

#ifndef NX_STRING_EXTENSION_H
#define NX_STRING_EXTENSION_H

#include "nx_type_traits.h"
#include "nx_forward_list.h"

namespace nx {

    template <typename _Tunderlying_container_type,
    std::enable_if_t<nx::is_char_type<typename _Tunderlying_container_type::value_type>::value, bool> = true>
    class basic_string_extension : public _Tunderlying_container_type{
    public:
        using underlying_container_type = _Tunderlying_container_type;
        using char_type = underlying_container_type::value_type;
        using size_type = typename underlying_container_type::size_type;
        using iterator = typename underlying_container_type::iterator;
        using const_iterator = typename underlying_container_type::const_iterator;

        basic_string_extension() = default;
        basic_string_extension(const char_type * c_str) {
            char_type * ptr = const_cast<char_type*>(c_str);
            while (*ptr != char_type(0))
                this->push_back(*ptr++);
        }
        basic_string_extension (const basic_string_extension&) = default;
        basic_string_extension (basic_string_extension&&) = default;
        basic_string_extension& operator= (const char_type * c_str) {};
        basic_string_extension& operator= (const basic_string_extension&) = default;
        basic_string_extension& operator= (basic_string_extension&&) = default;

        char_type & operator[] (size_type pos) const { return at(pos); }
        char_type & operator[] (size_type pos) { return at(pos); }

        template <typename _Tcontainer,
        std::enable_if_t<std::is_same<char_type, typename _Tcontainer::value_type>::value, bool> = true>
        void append (const basic_string_extension<_Tcontainer> & other) {
            for (auto it = other.begin(); it != other.end(); ++it)
                this->push_back(*it);
        }
        const char_type * c_str () const {
            char_type * buf;
            buf = new char_type[this->size() + 1]();
            size_t i = 0;
            auto it = this->begin();
            for (auto it = this->begin(); it != this->end(); ++it)
                buf[i++] = *it;
            return const_cast<const char_type *>(buf);
        }

        basic_string_extension operator + (const basic_string_extension & other) {
            auto out = *this;
            out.splice_back(other);
            return out;
        }

        basic_string_extension& operator += (const basic_string_extension & other) {
            this->splice_back(other);
            return *this;
        }
    };

    typedef basic_string_extension<nx::forward_list<char>> forward_string;
    typedef basic_string_extension<nx::forward_list<wchar_t>> forward_wstring;

    // typedef basic_string_extension<nx::list<char>> linked_string;
    // typedef basic_string_extension<nx::list<wchar_t>> linked_wstring;

    template <typename ...Args>
    forward_string format(const forward_string::char_type * c_str, Args... args) {
        forward_string::char_type buf [2048];
        int size = ::snprintf(buf, 2048, c_str, args...);
        return buf;
    }


} // nx

#endif //NX_STRING_EXTENSION_H
