//
// Created by nexie on 09.06.2025.
//

#ifndef NX_VECTOR_H
#define NX_VECTOR_H

// #define NX_VECTOR_DEBUG // define to output debug info

#include <stdexcept>
#include <iostream>
#include <initializer_list>

namespace nx
{
    namespace _detail
    {
        namespace _helpers
        {
            template <typename int_type>
            int_type nlpo2(int_type x);
        }

        struct _allocation_block
        {
#ifdef __aarch64__
            using size_type = uint64_t;
#else
            using size_type = size_t;
#endif
            using byte_type = unsigned char;

            byte_type* _data{nullptr};
            size_type _size{0};

            void m_init();
            void m_alloc(size_type p_bytes);
            void m_dealloc();
            void m_realloc(size_type p_bytes);

            static size_type S_suggest_size(size_type p_size);

            _allocation_block();
            _allocation_block(_allocation_block && p_other);
            _allocation_block & operator = (_allocation_block && p_other);
        };

        template <typename _Tp>
        struct _allocator
        {
            using size_type = _allocation_block::size_type;
            using byte_type = _allocation_block::byte_type;
            using value_type = _Tp;
            using reference = _Tp&;
            using const_reference = const _Tp&;
            using pointer = _Tp*;
            using const_pointer = const _Tp*;
            static constexpr size_type value_type_size = sizeof(_Tp);

            _allocation_block _block;

            _allocator() : _block{} {}
            _allocator(_allocator<_Tp> && p_other) : _block {std::move(p_other._block)} {}
            _allocator<_Tp> & operator = (_allocator<_Tp> && p_other)
            {
                m_deallocate();
                _block = std::move(p_other._block);
                return *this;
            }
            explicit _allocator(size_type p_size) : _block{}
            {
                try
                {
                    _block.m_alloc(sizeof(value_type) * p_size);
                }
                catch (std::runtime_error& e)
                {
                    std::cerr << "Runtime exception in _allocation_block: " << e.what();
                    throw;
                }catch (std::bad_alloc& e)
                {
                    std::cerr << "Bad alloc exception in _alloction_block: " << e.what();
                    throw;
                }catch (...)
                {
                    std::cerr << "Exception in _allocation_block!";
                    throw;
                }
            }
            void m_allocate_at_least(size_type p_size)
            {
                auto suggested_size = _block.S_suggest_size(_allocator<_Tp>::value_type_size * p_size);
                _block.m_alloc(suggested_size);
            }
            void m_allocate(size_type p_size)
            {
                _block.m_alloc(_allocator<_Tp>::value_type_size * p_size);
            }
            void m_deallocate()
            {
                _block.m_dealloc();
            }
            void m_reallocate_at_least(size_type p_size)
            {
                auto suggested_size = _block.S_suggest_size(_allocator<_Tp>::value_type_size * p_size);
                _block.m_realloc(suggested_size);
            }
            void m_reallocate(size_type p_size)
            {
                _block.m_realloc(_allocator<_Tp>::value_type_size * p_size);
            }
            pointer m_data()
            {
                return reinterpret_cast<pointer>(_block._data);
            }
            size_type m_capacity() const
            {
                return _block._size / _allocator<_Tp>::value_type_size;
            }
            void m_construct(pointer p, const_reference val)
            {
                ::new((void*)p) value_type(val);
            }
            template <typename U, class... Args>
            void m_construct(U* p, Args&&... args)
            {
                ::new((void*)p) U(std::forward<Args>(args)...);
            }
            void m_destroy(pointer p)
            {
                p->~value_type();
            }
            template <typename U>
            void m_destroy(U* p)
            {
                p->~U();
            }
        };

        template <typename _Tp>
        class _vector_iterator
        {
            using iterator = _vector_iterator<_Tp>;
            using value_type = _Tp;
            using reference = _Tp &;
            using pointer = _Tp *;

            pointer _cur_pos;
        public:
            explicit _vector_iterator(pointer p_pos) : _cur_pos(p_pos) {}
            const iterator & m_const_cast () const { return const_cast<iterator>(*this); }
            reference operator * () { return *_cur_pos; }
            pointer operator -> () { return _cur_pos; }
            iterator & operator ++ ()
            {
                _cur_pos += 1;
                return *this;
            }
            iterator & operator -- ()
            {
                _cur_pos -= 1;
                return *this;
            }
            iterator operator ++ (int)
            {
                auto tmp = *this;
                _cur_pos += 1;
                return tmp;
            }
            iterator operator -- (int)
            {
                auto tmp = *this;
                _cur_pos -= 1;
                return *tmp;
            }
            iterator & operator += (long p_val)
            {
                _cur_pos += p_val;
                return *this;
            }
            iterator & operator -= (long p_val)
            {
                _cur_pos -= p_val;
                return *this;
            }
            bool operator == (const iterator & rhs) const { return _cur_pos == rhs._cur_pos; }
            bool operator != (const iterator & rhs) const { return _cur_pos != rhs._cur_pos; }
        };

        template <typename _Tp>
        class _const_vector_iterator
        {
            using noconst_interator = _vector_iterator<_Tp>;
            using iterator = _const_vector_iterator<_Tp>;
            using value_type = _Tp;
            using reference = const _Tp &;
            using pointer = _Tp const *;

            pointer _cur_pos;
        public:
            explicit _const_vector_iterator(pointer p_pos) : _cur_pos(p_pos) {}
            explicit _const_vector_iterator(const noconst_interator & p_it) : _cur_pos(p_it._cur_pos) {}

            reference operator * () { return *_cur_pos; }
            pointer operator -> () { return _cur_pos; }
            iterator & operator ++ ()
            {
                _cur_pos += 1;
                return *this;
            }
            iterator & operator -- ()
            {
                _cur_pos -= 1;
                return *this;
            }
            iterator operator ++ (int)
            {
                auto tmp = *this;
                _cur_pos += 1;
                return tmp;
            }
            iterator operator -- (int)
            {
                auto tmp = *this;
                _cur_pos -= 1;
                return *tmp;
            }
            iterator & operator += (long p_val)
            {
                _cur_pos += p_val;
                return *this;
            }
            iterator & operator -= (long p_val)
            {
                _cur_pos -= p_val;
                return *this;
            }
            bool operator == (const iterator & rhs) const { return _cur_pos == rhs._cur_pos; }
            bool operator != (const iterator & rhs) const { return _cur_pos != rhs._cur_pos; }
        };
    }


    template <typename _Tp>
    class vector
    {
    public:
        using allocator_type = _detail::_allocator<_Tp>;
        using value_type = allocator_type::value_type;
        using size_type = allocator_type::size_type;
        using reference = allocator_type::reference;
        using const_reference = allocator_type::const_reference;
        using pointer = allocator_type::pointer;
        using const_pointer = allocator_type::const_pointer;
        using iterator = _detail::_vector_iterator<_Tp>;
        using const_iterator = _detail::_const_vector_iterator<_Tp>;

    protected:
        allocator_type _allocator;
        pointer _start{nullptr};
        pointer _finish{nullptr};
        pointer _cur_pos{nullptr};

        void m_init()
        {
            _allocator.m_allocate_at_least(0);
            _start = _allocator.m_data();
            _finish = _start + _allocator.m_capacity();
            _cur_pos = _start;
        }
        void m_bounds_check(pointer p) const
        {
            if ((p < _start) || (p > _cur_pos))
                throw std::range_error("nx::vector range_error - Index out of range");
        }
        void m_rebase()
        {
            auto ptr_diff = _cur_pos - _start;
            _start = _allocator.m_data();
            _cur_pos = _start + ptr_diff;
            _finish = _start + _allocator.m_capacity();
        }
        void m_rebase_expand ()
        {
            _allocator.m_reallocate_at_least(size() + 1);
            m_rebase();
        }
        void m_rebase_shrink()
        {
            _allocator.m_reallocate(size());
            m_rebase();
        }
        void m_construct_contents (pointer p_start, pointer p_finish, const value_type & p_val)
        {
            for (auto p = p_start; p < p_finish; p += 1)
                _allocator.m_construct(p, p_val);
        }
        template <class ...Args>
        void m_construct_contents (pointer p_start, pointer p_finish, Args&&... args)
        {
            for (auto p = p_start; p < p_finish; p += 1)
                _allocator.m_construct(p, std::forward<Args>(args)...);
        }
        void m_destroy_contents (pointer p_start, pointer p_finish)
        {
            for (auto p = p_start; p < p_finish; p += 1)
                _allocator.m_destroy(p);
        }

    public:
        vector() : _start{nullptr}, _finish{nullptr}, _cur_pos{nullptr} { m_init(); }
        explicit vector(size_type size, const value_type & p_val = value_type()) : vector()
        {
            resize(size, p_val);
        }
        ~vector()
        {
            clear();
        }
        vector(const vector<_Tp>& p_other)
        {
            _allocator.m_allocate(p_other.size());
            for (auto s = 0; s < p_other.size(); s++)
                push_back(p_other.at(s));
        }
        vector(vector<_Tp> && p_other) : _allocator(std::move(p_other._allocator))
        {
            m_rebase();
            p_other.m_init();
        }
        vector<_Tp> & operator= (const vector<_Tp> & p_other)
        {
            clear();
            _allocator.m_allocate(p_other.size());
            for (auto s = 0; s < p_other.size(); s++)
                push_back(p_other.at(s));
        }
        vector<_Tp> & operator= (vector<_Tp> && p_other)
        {
            clear();
            _allocator = std::move(p_other._allocator);
            m_rebase();
            p_other.m_init();
        }
        vector (std::initializer_list<_Tp> p_ilist) : vector {}
        {
            reserve(p_ilist.size());
            for (auto it: p_ilist)
                push_back(it);
        }
        vector<_Tp> & operator= (std::initializer_list<_Tp> p_ilist)
        {
            clear();
            reserve(p_ilist.size());
            for (auto it: p_ilist)
                push_back(it);
        }

        bool operator == (const vector<_Tp> & other) const
        {
            if (this->size() != other.size())
                return false;
            size_type size = this->size();
            for (size_type s = 0; s < size; ++s)
                if (this->at(s) != other.at(s)) return false;
            return true;
        }
        bool operator != (const vector& p_other) const
        {
            return !operator==(p_other);
        }

        size_type size () const { return _cur_pos - _start; }
        size_type capacity () const { return _allocator.m_capacity(); }
        bool empty () const { return _start == _cur_pos; }

        void shrink ()
        {
            m_rebase_shrink();
        }
        void reserve (size_type p_size)
        {
            if (p_size <= capacity())
                return;
            _allocator.m_reallocate(p_size);
            m_rebase();
        }
        void reserve_at_least (size_type p_size)
        {
            if (p_size <= capacity())
                return;
            _allocator.m_reallocate_at_least(p_size);
            m_rebase();
        }
        void resize (size_type p_size)
        {
            if (p_size > capacity())
                reserve (p_size);
            if (p_size > size())
                m_construct_contents(_start + size(), _start + p_size);
            else
                m_destroy_contents(_start + p_size, _start + size());
                _cur_pos = _start + p_size;
        }
        void clear ()
        {
            m_destroy_contents(_start, _cur_pos);
            _allocator.m_deallocate();
            m_init();
        }
        reference at (size_type p_pos)
        {
            m_bounds_check(_start + p_pos);
            return *(_start + p_pos);
        }
        const_reference at (size_type p_pos) const
        {
            m_bounds_check(_start + p_pos);
            return *(_start + p_pos);
        }
        reference front ()
        {
            return _start[0];
        }
        const_reference front () const
        {
            return _start[0];
        }
        reference back ()
        {
            return _start[size() - 1];
        }
        const_reference back () const
        {
            return _start[size() - 1];
        }
        pointer data()
        {
            return _start;
        }
        const_pointer data() const
        {
            return _start;
        }
        iterator begin ()
        {
            return iterator(_start);
        }
        const_iterator begin () const
        {
            return const_iterator(_start);
        }
        iterator end ()
        {
            return iterator(_cur_pos);
        }
        const_iterator end () const
        {
            return const_iterator(_cur_pos);
        }

        void push_back (const value_type & p_val)
        {
            if (size() >= capacity())
                m_rebase_expand();
            _allocator.m_construct(_cur_pos, p_val);
            _cur_pos += 1;
        }
        void pop_back ()
        {
            if (size () == 0)
            {
                throw std::runtime_error("nx::vector runtime error -- pop_back(): vector is empty");
                return;
            }
            _allocator.m_destroy(_cur_pos);
            _cur_pos -= 1;
        }

        reference operator [] (size_type p_pos)
        {
            return _start[p_pos];
        }
        const_reference operator [] (size_type p_pos) const
        {
            return _start[p_pos];
        }
    };
} // nx



#endif //VECTOR_H
