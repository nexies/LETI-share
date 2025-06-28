//
// Created by nexie on 24.06.2025.
//

#ifndef MARKER_STRING_H
#define MARKER_STRING_H

#include <string>
#include <iostream>
#include "nx_forward_list.h"

class marker_string {
    char * _data;
    char _marker;

    void m_init() {
        _data = nullptr;
        _marker = '$';
    }

public:
    marker_string() : _data{ nullptr }, _marker{ '$' } {}
    marker_string(const char * cstr, int l = -1) : marker_string() {
        auto len = (l == -1) ? strlen(cstr) : l;
        _data = new char [len + 1];
        memcpy(_data, cstr, len);
        _data[len] = _marker;
    }
    marker_string(const std::string & str) : marker_string() {
        auto len = str.size();
        _data = new char [len + 1];
        memcpy(_data, str.c_str(), len);
        _data[len] = _marker;
    }
    marker_string(const marker_string & other) :
    _marker{other._marker}
    {
        int len = other.size();
        _data = new char [len + 1];
        _data[len] = _marker;
        for (int i = 0; i < len; i++)
            _data[i] = other._data[i];
    }
    ~marker_string() {
        clear();
    }
    marker_string & operator=(const marker_string & other) {
        clear();
        _marker = other._marker;
        auto len = other.size();
        _data = new char [len + 1];
        memcpy(_data, other.data(), len);
        _data[len] = _marker;
        return *this;

    }

    int size () const {
        if (_data == nullptr) return 0;
        int out = 0;
        auto ch = data();
        while (*ch++ != _marker) out += 1;
        return out;
    }
    void clear () { if (_data) delete [] _data; }
    char * data () const { return _data; }
    char marker () const { return _marker; }
    bool empty () const { return (_data == nullptr) || (_data[0] == _marker); }
    const char * c_str() const {
        static char * buf = (char *) malloc(2048);
        auto s = size();
        memcpy((void *)buf, data(), s);
        *(buf + s) = 0;
        return const_cast<const char *>(buf);
    }

    nx::forward_list<marker_string> split (char sep, bool keep_empty_parts = false) const {
        nx::forward_list<marker_string> out;
        char * p = _data;
        char * q = _data;
        while (*p != _marker) {
            if (*p == sep) {
                marker_string ms {q, (int)(p - q)};
                q = p + 1;
                if (!ms.empty() || keep_empty_parts) out.push_front(ms);
            }
            p++;
        }
        marker_string ms {q, (int)(p - q)};
        if (!ms.empty() || keep_empty_parts) out.push_front(ms);
        out.reverse();
        return out;
    }

    bool operator == (const marker_string & other) const {
        int this_size = size();
        int other_size = other.size();
        if (this_size != other_size) return false;

        char * this_p = data();
        char * other_p = other.data();
        while (*this_p != _marker) {
            if (*this_p != *other_p) return false;
            this_p++;
            other_p++;
        }
        return true;
    }
};

inline std::basic_ostream<char> & operator << (std::basic_ostream<char> & stream, const marker_string & str) {
    auto ch = str.data();
    while (*ch != str.marker())
        stream << *ch++;
    return stream;
}

marker_string remove_special_symbols_from_word_n(const marker_string & in, int n);

namespace nx {
    template <typename ...Args>
    marker_string format(const char * c_str, Args... args) {
        char buf [2048];
        int size = ::snprintf(buf, 2048, c_str, args...);
        return buf;
    }
}

#endif //MARKER_STRING_H
