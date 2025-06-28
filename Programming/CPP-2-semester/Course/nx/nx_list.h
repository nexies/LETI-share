//
// Created by nexie on 05.06.2025.
//

#ifndef LIST_H
#define LIST_H
#include <initializer_list>
#include <memory>

namespace nx {
    namespace _detail
    {
        struct _list_node_base
        {
            using Self = _list_node_base;
            _list_node_base * _next;
            _list_node_base * _prev;

            _list_node_base () : _next {nullptr}, _prev {nullptr} {}
            _list_node_base (Self * p_next, Self * p_prev) : _next {p_next}, _prev {p_prev} {}
            void m_init()
            {
                _next = nullptr;
                _prev = nullptr;
            }
            void m_swap(_list_node_base & other)
            {
                std::swap(other._next, this->_next);
                std::swap(other._prev, this->_prev);
            }

            void m_place_after (Self * p_node_before)
            {
                Self * future_after = p_node_before->_next;
                this->_prev = p_node_before;
                this->_next = future_after;
                if (p_node_before)
                    p_node_before->_next = this;
                if (future_after)
                    future_after->_prev = this;
            }
            void m_place_before (Self * p_node_after)
            {
                Self * future_before = p_node_after->_prev;
                this->_prev = future_before;
                this->_next = p_node_after;
                if (future_before)
                    future_before->_next = this;
                if (p_node_after)
                    p_node_after->_prev = this;
            }
            void m_place_between (Self * p_node_before, Self * p_node_after)
            {
                this->_prev = p_node_before;
                this->_next = p_node_after;
                if (p_node_before)
                    p_node_before->_next = this;
                if (p_node_before)
                    p_node_after->_prev = this;
            }
            Self * m_take ()
            {
                if (_prev)
                    _prev->_next = this->_next;
                if (_next)
                    _next->_prev = this->_prev;
                return this;
            }
        };

        struct _list_header : public _list_node_base
        {
            using Base = _list_node_base;
            using Self = _list_header;
            _list_header () : _list_node_base(this, this) {}
            void m_init()
            {
                _next = this;
                _prev = this;
            }
            Base * m_base () { return static_cast<Base *>(this); }
        };

        template <typename _Tp>
        struct _list_node : public _list_node_base
        {
            using Self = _list_node;
            using Base = _list_node_base;
            using DataType = _Tp;
            _Tp _data;

            _list_node() : _list_node_base() {}
            explicit _list_node(const _Tp & p_data) : _list_node_base(), _data {p_data} {}
            explicit _list_node(_Tp && p_data) : _list_node_base(), _data {std::move(p_data)} {}
            template <class ...Args>
            explicit _list_node (Args&&... args) : _list_node_base(), _data {std::forward<Args>(args)...} {}

            DataType & m_data () const { return _data; }
            DataType & m_data () { return _data; }
            Base * m_base () { return static_cast<Base *> (this); }
        };

        struct _list_base
        {
            using Node = _list_node_base;

            _list_header _start;
            _list_header _end;
            size_t _size;
            _list_base () : _start {}, _end {}, _size {0}
            {
                m_init();
            }
            ~_list_base ()
            {
                m_clear();
            }
            _list_base (_list_base && other)
            {
                this->_start = other._start;
                this->_end = other._end;
                this->_size = other._size;
                other.m_init();
            }
            _list_base & operator = (_list_base && other)
            {
                this->_start = other._start;
                this->_end = other._end;
                this->_size = other._size;
                other.m_init();
                return *this;
            }
            void m_init ()
            {
                _size = 0;
                _start._prev = &_start;
                _start._next = &_end;

                _end._next = &_end;
                _end._prev = &_start;
            }
            void m_clear ()
            {
                Node * cur_ptr = _start._next;
                Node * next_ptr = cur_ptr->_next;
                while (cur_ptr != next_ptr)
                {
                    delete cur_ptr;
                    cur_ptr = next_ptr;
                    next_ptr = next_ptr->_next;
                }
                m_init();
            }
            size_t m_size () { return _size; }
            void m_insert_node (size_t p_pos, Node * p_node)
            {
                if (p_pos <= 0)
                    p_node->m_place_after(&_start);
                else if (p_pos >= _size)
                    p_node->m_place_before(&_end);
                else
                {
                    Node * ptr = _start._next;
                    for (size_t it = 0; it != p_pos; it++)
                        ptr = ptr->_next;
                    p_node->m_place_between(ptr->_prev, ptr);
                }
                _size += 1;
            }
            Node * m_take_node (size_t p_pos)
            {
                Node * out;
                if (_size <= 0) return nullptr;
                if (p_pos <= 0)
                    out = _start._next->m_take();
                else if (p_pos >= _size)
                    out = _end._prev->m_take();
                else
                {
                    Node * ptr = _start._next;
                    for (size_t it = 0; it != p_pos; it++)
                        ptr = ptr->_next;
                    out = ptr->m_take();
                }
                _size -= 1;
                return out;
            }
            void m_splice (size_t p_pos, _list_base & p_other)
            {
                if (p_other._size <= 0) return;

                auto other_first = p_other._start._next;
                auto other_last = p_other._end._prev;
                if (p_pos <= 0)
                {
                    other_first->_prev = &_start;
                    other_last->_next = _start._next;
                    other_first->_prev->_next = other_first;
                    other_last->_next->_prev = other_last;
                }
                else if (p_pos >= _size)
                {
                    other_first->_prev = _end._prev;
                    other_last->_next = &_end;
                    other_first->_prev->_next = other_first;
                    other_last->_next->_prev = other_last;
                }
                else
                {
                    Node * ptr = _start._next;
                    for (size_t it = 0; it != p_pos; it++)
                        ptr = ptr->_next;
                    other_last->_next = ptr->_next;
                    other_first->_prev = ptr;
                    ptr->_next->_prev = other_last;
                    ptr->_next = other_first;
                }
                _size += p_other._size;
                p_other.m_init();
            }
        };

        template <typename _Tp>
        class _list_iterator
        {
            using Self = _list_iterator<_Tp>;
            using Node = _list_node_base;
            using Reference = _Tp &;
            using Pointer   = _Tp *;
            Node * _cur;

        public:
            _list_iterator () : _cur {} {}
            explicit _list_iterator (Node * p_node) : _cur {p_node} {}

            Self m_const_cast () const
            {
                return *this;
            }
            Reference operator * () const
            {
                return static_cast<_list_node<_Tp> *> (_cur)->_data;
            }
            Pointer operator -> () const
            {
                return &(static_cast<_list_node<_Tp> *> (_cur)->_data);
            }
            Self& operator ++ ()
            {
                _cur = _cur->_next;
                return *this;
            }
            Self operator ++ (int)
            {
                auto tmp = *this;
                _cur = _cur->_next;
                return tmp;
            }
            Self& operator -- ()
            {
                _cur = _cur->_prev;
                return *this;
            }
            Self operator -- (int)
            {
                auto tmp = *this;
                _cur = _cur->_prev;
                return tmp;
            }
            bool operator == (const Self & p_other) const
            {
                return _cur == p_other._cur;
            }
            bool operator != (const Self & p_other) const
            {
                return _cur != p_other._cur;
            }
        };

        template <typename _Tp>
        class _list_const_iterator
        {
            using Iterator = _list_iterator<_Tp>;
            using Self = _list_const_iterator<_Tp>;
            using Node = _list_node_base;
            using Reference = const _Tp &;
            using Pointer = const _Tp *;

            const Node * _cur;

        public:
            _list_const_iterator() : _cur {} {}
            explicit _list_const_iterator(const Node * p_node) : _cur { p_node } {}
            _list_const_iterator (const Iterator & p_it) : _cur { p_it._cur } {}
            Reference operator * () const
            {
                return static_cast<const _list_node<_Tp> *> (_cur)->_data;
            }
            Pointer operator -> () const
            {
                return &(static_cast<const _list_node<_Tp> *> (_cur)->_data);
            }
            Self& operator ++ ()
            {
                _cur = _cur->_next;
                return *this;
            }
            Self operator ++ (int)
            {
                auto tmp = *this;
                _cur = _cur->_next;
                return tmp;
            }
            Self& operator -- ()
            {
                _cur = _cur->_prev;
                return *this;
            }
            Self operator -- (int)
            {
                auto tmp = *this;
                _cur = _cur->_prev;
                return tmp;
            }
            bool operator == (const Self & p_other) const
            {
                return _cur == p_other._cur;
            }
            bool operator != (const Self & p_other) const
            {
                return _cur != p_other._cur;
            }
        };

    } // _detail

    template <typename _Tp>
    class list
    {
    public:
        using Self = list<_Tp>;
        using Node = _detail::_list_node<_Tp>;
        using Base = _detail::_list_base;

        using value_type = _Tp;
        using reference = _Tp &;
        using const_reference = _Tp const &;
        using pointer = _Tp *;

        using iterator = _detail::_list_iterator<_Tp>;
        using const_iterator = _detail::_list_const_iterator<_Tp>;
#ifdef __aarch64__
        using size_type = uint64_t;
#else
        using size_type = size_t
#endif
    private:
        Base _base;
    public:
        list () : _base {} {}
        explicit list (size_t size, value_type default_value = value_type())
        {
            for (size_t it = 0; it <= size; it++)
                push_back(default_value);
        }
        list (const Self & other)
        {
            auto it = other.begin();
            while (it != other.end())
            {
                push_back(*it);
                ++it;
            }
        }
        Self & operator = (const Self & other)
        {
            _base.m_clear();
            auto it = other.begin();
            while (it != other.end())
            {
                push_back(*it);
                ++it;
            }
            return *this;
        }
        list (Self && other) : _base(std::move(other._base)) {}
        Self & operator = (Self && other)
        {
            _base.m_clear();
            _base = std::move(other._base);
            return *this;
        }
        list (std::initializer_list<value_type> p_ilist) : list {}
        {
            for (auto it: p_ilist)
                push_back(it);
        }
        Self & operator = (std::initializer_list<value_type> p_ilist)
        {
            clear();
            for (auto it: p_ilist)
                push_back(it);
            return *this;
        }

        bool empty () const { return size() == 0; }
        size_t size () const { return _base._size; }
        void clear () { _base.m_clear(); }
        void insert (size_t pos, const value_type & value)
        {
            _base.m_insert_node(pos, new Node(value));
        }
        void insert (size_t pos, value_type && p_value)
        {
            _base.m_insert_node(pos, new Node(std::forward<value_type>(p_value)));
        }

        template <class ...Args>
        void emplace (size_t pos, Args&&... args)
        {
            _base.m_insert_node(pos, new Node(std::forward<Args>(args)...));
        }
        template <class ...Args>
        void emplace_front(Args&&... args)
        {
            emplace(0, std::forward<Args>(args)...);
        }
        template <class ...Args>
        void emplace_back(Args&&... args)
        {
            emplace(size(), std::forward<Args>(args)...);
        }

        void push_back (const value_type & p_value)
        {
            insert(size(), p_value);
        }
        void push_back (value_type && p_value)
        {
            insert(size(), std::forward<value_type>(p_value));
        }

        void push_front (const value_type & p_value)
        {
            insert(0, p_value);
        }
        void push_front (value_type && p_value)
        {
            insert(0, std::forward<value_type>(p_value));
        }

        // void splice (size_t pos, Self && other);
        void splice (size_t pos, Self & other)
        {
            _base.m_splice(pos, other._base);
        }
        void splice_front (Self & other)
        {
            splice(0, other);
        }
        void splice_back (Self & other)
        {
            splice(size(), other);
        }

        void remove_at (size_t pos)
        {
            auto removed = _base.m_take_node(pos);
        }
        void pop_front ()
        {
            remove_at(0);
        }
        void pop_back ()
        {
            remove_at(size());
        }

        iterator begin ()
        {
            return iterator(_base._start._next);
        }
        const_iterator begin () const
        {
            return const_iterator(_base._start._next);
        }

        iterator end ()
        {
            return iterator(&_base._end);
        }
        const_iterator end () const
        {
            return const_iterator(&_base._end);
        }

        reference front ()
        {
            return *begin();
        }
        const_reference front () const
        {
            return *begin();
        }
        reference back ()
        {
            return *(--end());
        }
        const_reference back () const
        {
            return *(--end());
        }

        Self & operator << (const _Tp & value)
        {
            push_back(value);
            return *this;
        }

        template <typename compare_func = std::less<_Tp>>
        void sort (compare_func comp = std::less<_Tp>())
        {
            if (size() <= 1) return;

            value_type pivot = front();
            pop_front();

            Self less;
            Self greater;

            for (const auto& item : *this) {
                if (comp(item, pivot)) {
                    less.push_back(item);
                } else {
                    greater.push_back(item);
                }
            }
            clear();
            less.sort(comp);
            greater.sort(comp);

            splice_back(less);
            push_back(pivot);
            splice_back(greater);
        }
    };
} // nx
#endif //LIST_H
