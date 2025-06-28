//
// Created by nexie on 10.06.2025.
//

#ifndef STRING_H
#define STRING_H

#include <cstring>

#include "nx_vector.h"
#include "nx_list.h"
#include "nx_type_traits.h"
#include "nx_string.h"

namespace nx {
    template <typename _Tp,
            std::enable_if_t<is_char_type_t<_Tp>::value, bool>>
    class basic_string_list;

    template <typename _Tp,
            std::enable_if_t<is_char_type<_Tp>::value, bool> = true>
    class basic_string : public vector<_Tp>
    {
    public:
        using char_type = vector<_Tp>::value_type;
        using size_type = vector<_Tp>::size_type;
        using pointer   = vector<_Tp>::pointer;
        using list_type = basic_string_list<char_type, true>;
    private:
        void m_from_cstr (const char_type * p_cstr, size_type p_len = -1)
        {
            if (!this->is_empty())
                this->clear();
            size_type len;
            if (p_len == -1) len = strlen(p_cstr);
            else len = p_len;
            this->_allocator.m_allocate(len);
            this->m_rebase();
            this->_cur_pos = this->_start + len;
            std::memcpy(this->data(), p_cstr, sizeof(char_type) * len);
        }
    public:
        basic_string () : vector<char_type> () {}
        basic_string(const char_type* p_cstr, int p_len = -1)
        {
            this->m_from_cstr(p_cstr, p_len);
        }
        basic_string(const basic_string & p_other)
        {
            this->_allocator.m_allocate(p_other.size());
            this->m_from_cstr(p_other.c_str(), p_other.size());
        }
        basic_string(basic_string && p_other)
        {
            this->_allocator = std::move(p_other._allocator);
            this->m_rebase();
        }
        basic_string & operator = (const basic_string & p_other)
        {
            this->clear();
            this->_allocator.m_allocate(p_other.size());
            m_copy_to(this->data(), p_other.data(), p_other.size());
            return *this;
        }
        basic_string & operator = (basic_string && p_other)
        {
            this->clear();
            this->_allocator = std::move(p_other._allocator);
            this->m_rebase();
            this->_cur_pos = this->_start + p_other.size();
            p_other.m_init();
            return *this;
        }
        const char_type * c_str() const
        {
            return const_cast<const char_type *>(this->data());
        }
        void append(const basic_string & p_other)
        {
            if (this->capacity() < this->size() + p_other.size())
                this->reserve_at_least(this->size() + p_other.size());
            std::memcpy(this->_cur_pos, p_other.data(), sizeof(char_type) * p_other.size());
            this->_cur_pos += p_other.size();
        }
        basic_string & operator += (const basic_string & p_other)
        {
            this->append(p_other);
            return *this;
        }
        basic_string operator + (const basic_string & p_other) const
        {
            basic_string out = *this;
            return out += p_other;
        }
        bool starts_with (const basic_string & p_other) const
        {
            return std::memcmp(this->data(), p_other.data(), p_other.size()) == 0;
        }
        bool ends_with (const basic_string & p_other) const
        {
            return std::memcmp(this->data() + this->size() - p_other.size(), p_other.data(), p_other.size()) == 0;
        }
        int first_index_of(char_type p_ch) const
        {
            for (size_type s = 0; s < this->size(); ++s)
                if (this->at(s) == p_ch)  return s;
            return -1;
        }
        int last_index_of(char_type p_ch) const
        {
            for (size_type s = this->size() - 1; s >= 0; --s)
                if (this->at(s) == p_ch) return s;
            return -1;
        }
        basic_string mid (size_type pos, int len = -1) const
        {
            if (pos >= this->size())
                return basic_string();
            if (len == -1)
                len = this->size() - pos;
            return basic_string(this->data() + pos, len);
        }
        list_type split(char_type p_ch, bool keep_empty_parts = false) const
        {
            list_type out;
            basic_string tmp = *this;
            int idx = 0;
            while (-1 != (idx = tmp.first_index_of(p_ch)))
            {
                auto sub = tmp.mid(0, idx);
                if (!sub.is_empty() || keep_empty_parts)
                    out.emplace_back(sub);
                tmp = tmp.mid(idx + 1, -1);
            }
            out.emplace_back(tmp);
            return out;
        }
        bool operator == (const basic_string & p_other) {
            if (p_other.size() != this->size())
                return false;
            return strncmp(this->data(), p_other.data(), p_other.size()) == 0;
        }
        bool operator != (const basic_string & p_other) {
            return !this->operator == (p_other);
        }
    };

    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;

    template <typename _Tp,
            std::enable_if_t<is_char_type_t<_Tp>::value, bool> = true>
    class basic_string_list : public list<basic_string<_Tp>> {
    public:
        using char_type = _Tp;
        using string_type = basic_string<char_type>;

        basic_string_list() : list<string_type>() {}
        basic_string_list(const basic_string_list & p_other) : list<string_type>(p_other) {}
        basic_string_list & operator = (const basic_string_list & p_other) {
            return list<string_type>::operator=(p_other);
        }
        basic_string_list (basic_string_list && p_other) : list<string_type>(p_other) {}
        basic_string_list & operator = (basic_string_list && p_other) {
            return list<string_type>::operator=(std::forward<basic_string_list>(p_other));
        }

        string_type join () {
            string_type out;
            for (auto it = this->begin(); it != this->end(); ++it)
                out += *it;
            return out;
        }
        string_type join (char_type p_sep) {
            string_type out;
            auto pre_last = --(this->end());
            for (auto it = this->begin(); it != pre_last; ++it) {
                out += *it;
                out.push_back(p_sep);
            }
            out += this->back();
            return out;
        }
        string_type join (const string_type & p_sep)
        {
            string_type out;
            auto pre_last = --(this->end());
            for (auto it = this->begin(); it != pre_last; ++it)
                out += *it + p_sep;
            return out + this->back();
        }
    };

    typedef basic_string_list<char> string_list;
    typedef basic_string_list<wchar_t> wstring_list;

    template <typename ...Args>
    string format(const string::char_type * c_str, Args... args) {
        string::char_type buf [2048];
        int size = ::snprintf(buf, 2048, c_str, args...);
        return buf;
    }
} // nx
#endif //STRING_H
