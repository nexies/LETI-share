//
// Created by nexie on 09.06.2025.
//

#include "nx_vector.h"
#include <cstring>
#include <cstdint>

namespace nx {
    template <>
    uint32_t _detail::_helpers::nlpo2(uint32_t x)
    {
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        return x + 1;
    }
    template <>
    int32_t _detail::_helpers::nlpo2(int32_t x)
    {
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        return x + 1;
    }
    template <>
    uint64_t _detail::_helpers::nlpo2 (uint64_t x)
    {
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        x |= (x >> 32);
        return x + 1;
    }
    template <>
    int64_t _detail::_helpers::nlpo2 (int64_t x)
    {
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
        x |= (x >> 32);
        return x + 1;
    }

    void _detail::_allocation_block::m_init()
    {
        _data = nullptr;
        _size = 0;
    }

    void _detail::_allocation_block::m_alloc(size_type p_bytes)
    {
#ifdef NX_VECTOR_DEBUG
        std::cerr << "_allocation_block -- Performing alloc: " << p_bytes << " bytes" << std::endl;
#endif
        _data = new byte_type [p_bytes];
        _size = p_bytes;
    }

    void _detail::_allocation_block::m_dealloc()
    {
        if (_data)
        {
#ifdef NX_VECTOR_DEBUG
        std::cerr << "_allocation_block -- Performing dealloc: " << _size << " bytes" << std::endl;
#endif
            delete [] _data;
            _data = nullptr;
            _size = 0;
        }
    }

    void _detail::_allocation_block::m_realloc(size_type p_bytes)
    {
#ifdef NX_VECTOR_DEBUG
        std::cerr << "_allocation_block -- Performing realloc: " << p_bytes << " bytes" << std::endl;
#endif
        byte_type * new_data = new byte_type[p_bytes];
        ::std::memcpy(new_data, _data, _size > p_bytes ? p_bytes : _size);
        _size = p_bytes;
        delete [] _data;
        _data = new_data;
    }

    _detail::_allocation_block::size_type _detail::_allocation_block::S_suggest_size(size_type p_size)
    {
        static constexpr size_type treshhold = 0x10000; // after which we are not doubling the size;
        if (p_size >= treshhold)
            return ((p_size / treshhold) + 1) * treshhold;
        if (p_size >= 16)
        {
            auto next_pow_2 = _helpers::nlpo2(p_size + 16);
            return next_pow_2 - 16;
        }
        return _helpers::nlpo2(p_size);
    }

    _detail::_allocation_block::_allocation_block() :
        _data { nullptr },
        _size { 0 }
    {
    }

    _detail::_allocation_block::_allocation_block(_allocation_block&& p_other) :
        _data { p_other._data },
        _size { p_other._size }
    {
        p_other.m_init();
    }

    _detail::_allocation_block& _detail::_allocation_block::operator=(_allocation_block&& p_other)
    {
        m_dealloc();
        _data = p_other._data;
        _size = p_other._size;
        p_other.m_init();
        return *this;
    }
} // nx