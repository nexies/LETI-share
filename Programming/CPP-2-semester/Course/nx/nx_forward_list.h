//
// Created by nexie on 17.06.2025.
//

#ifndef NX_FORWARD_LIST_H
#define NX_FORWARD_LIST_H

#include <ranges>

namespace nx {

    namespace _detail
    {
        struct _forward_list_node_base {
            _forward_list_node_base * _next;
            explicit _forward_list_node_base (_forward_list_node_base * p_next = nullptr) : _next{p_next}  {};
        };
        template <typename _Tp>
        struct _forward_list_node : public _forward_list_node_base
        {
            using base = _forward_list_node_base;
            using value_type = _Tp;
            value_type _data;

            explicit _forward_list_node (const value_type & p_data) :
            _data {p_data},
            _forward_list_node_base{nullptr}
            {};
            explicit _forward_list_node (const _Tp & p_data, base * p_next) :
            _data {p_data},
            _forward_list_node_base{p_next}
            {};

            base * m_base () const { return static_cast<base *> (this); }
            value_type & m_data () { return _data; }
            value_type const & m_data () const { return _data; }
            value_type * m_data_ptr () { return &_data; }
            value_type * m_data_ptr () const { return &_data; }
        };

        template <typename _Tp>
        struct _forward_list_iterator {
            using value_type = _Tp;
            using base_type = _forward_list_node_base;
            using node_type = _forward_list_node<value_type>;
            using self_type = _forward_list_iterator<value_type>;
            base_type * _node;

            explicit _forward_list_iterator (base_type * p_node) : _node(p_node) {}
            const self_type & m_const_cast () const { return *this; }
            value_type * operator -> () { return static_cast<node_type *>(_node->_next)->m_data_ptr(); }
            value_type & operator * () { return static_cast<node_type *>(_node->_next)->m_data(); }
            self_type & operator ++ () { _node = _node->_next; return *this; }
            self_type operator ++ (int) { auto tmp = *this; _node = _node->_next; return tmp; }
            bool operator == (const self_type & p_other) { return _node == p_other._node || (at_end() && p_other.at_end()); }
            bool operator != (const self_type & other) { return !operator == (other); }
            bool at_end () const { return !valid() || (_node->_next == nullptr); } // || (_node->_next->_next == nullptr); }
            bool valid () const { return _node != nullptr; }
        };

        template <typename _Tp>
        struct _const_forward_list_iterator {
            using value_type = _Tp;
            using base_type = _forward_list_node_base;
            using node_type = _forward_list_node<value_type>;
            using self_type = _const_forward_list_iterator<value_type>;
            using iterator_type = _forward_list_iterator<value_type>;
            const base_type * _node;

            explicit _const_forward_list_iterator (const base_type * p_node) : _node(p_node) {}
            _const_forward_list_iterator (const iterator_type & p_other) : _node(p_other._node) {}
            value_type * operator -> () const { return static_cast<node_type *>(_node->_next)->m_data_ptr(); }
            value_type & operator * () const { return static_cast<node_type *>(_node->_next)->m_data(); }
            self_type & operator ++ () { _node = _node->_next; return *this; }
            self_type operator ++ (int) { auto tmp = *this; _node = _node->_next; return tmp; }
            bool operator == (const self_type & p_other) { return _node == p_other._node || (at_end() && p_other.at_end());; }
            bool operator != (const self_type & other) { return !operator == (other); }
            bool at_end () const { return !valid() || (_node->_next == nullptr); }// || (_node->_next->_next == nullptr); }
            bool valid () const { return _node != nullptr; }
        };
    };

    template <typename _Tp = void>
    class forward_list {
    public:
        using value_type = _Tp;
        using reference = value_type&;
        using pointer = value_type*;
        using const_reference = const value_type&;
        using const_pointer = const value_type*;
        using iterator = _detail::_forward_list_iterator<value_type>;
        using const_iterator = _detail::_const_forward_list_iterator<value_type>;
        using list_node = _detail::_forward_list_node<value_type>;
        using self_type = forward_list<value_type>;
#ifdef __aarch64__
        using size_type = uint64_t;
#else
        using size_type = size_t;
#endif
    private:
        _detail::_forward_list_node_base _root;
        size_type _size;
        void m_init() {
            _root._next = nullptr;
            _size = 0;
        }
        const_iterator m_last_node () {
            auto it = begin();
            for (; !it.at_end(); ++it) {}
            return it;
        }
    public:
        forward_list () { m_init(); }
        ~forward_list () { clear(); }
        forward_list (::std::initializer_list<value_type> p_list) {
            m_init();
            auto back_it = begin();
            for (auto it = p_list.begin(); it != p_list.end(); ++it, ++back_it)
                insert(back_it, *it);
        }
        forward_list (const self_type & p_other) {
            m_init();
            splice_back(p_other);
        }
        forward_list (self_type && p_other) {
            _root = p_other._root;
            p_other.m_init();
        }
        forward_list & operator = (const self_type & p_other) {
            clear();
            m_init();
            splice_back(p_other);
            return *this;
        }
        forward_list & operator = (self_type && p_other) {
            clear();
            m_init();
            _root = p_other._root;
            _size = p_other._size;
            p_other.m_init();
            return *this;
        }

        iterator begin () { return iterator(&_root); }
        const_iterator begin () const { return const_iterator(&_root); }
        iterator end () { return iterator(nullptr); }
        const_iterator end () const { return iterator(nullptr); }
        iterator true_end () {auto it = begin(); while (!it.at_end()) {++it;} return it; }
        const_iterator true_end () const {auto it = begin(); while (!it.at_end()) {++it;} return it; }

        void clear() {
            auto cur_ptr = &_root;
            auto next_ptr = _root._next;
            while (next_ptr && (cur_ptr != next_ptr)) {
                cur_ptr = next_ptr;
                next_ptr = next_ptr->_next;
                delete cur_ptr;
            }
            m_init();
        }
        size_type size () const {
            return _size;
        }
        bool empty () const { return _size == 0; }
        iterator insert (const iterator & pos, const value_type & p_data) {
            if (!pos.valid()) return iterator(nullptr);
            auto new_node = new list_node(p_data, pos._node->_next);
            pos._node->_next = new_node;
            _size += 1;
            return iterator(new_node);
        }
        iterator remove (const iterator & pos) {
            if (!pos.valid()) { return iterator(nullptr); }
            if (pos.at_end()) { pop_back(); return end(); }
            auto node = pos._node->_next;
            pos._node->_next = node->_next;
            delete node;
            return iterator(pos._node);
        }
        void push_front(const value_type & p_data) {
            auto new_node = new list_node(p_data, _root._next);
            _root._next = new_node;
            _size += 1;
        }
        void pop_front () {
            if (_root._next == nullptr) return;
            auto tmp = _root._next;
            _root._next = _root._next->_next;
            delete tmp;
            _size -= 1;
        }
        void push_back(const value_type & data) {
            auto it = begin();
            for (; !it.at_end(); ++it) {}
            insert(it, data);
        }
        void pop_back() {
            if (empty()) return;
            auto tmp = &_root;
            while (tmp->_next->_next != nullptr) { tmp = tmp->_next; }
            delete tmp->_next;
            tmp->_next = nullptr;
            _size -= 1;
        }
        void splice_back (const self_type & p_other) {
            auto back_it = iterator(&_root);
            for (; !back_it.at_end(); ++back_it) {}
            auto other_it = p_other.begin();
            while (other_it != p_other.end()) {
                insert(back_it, *other_it);
                ++back_it;
                ++other_it;
            }
        }

        reference at (size_type p_pos) {
            // static_assert(p_pos < _size && p_pos > 0);
            size_type pos { 0 };
            for (auto it = begin(); it != end(); ++it, pos++)
                if (pos == p_pos)
                    return *it;
        }
        const_reference at (size_type p_pos) const {
            // static_assert(p_pos < _size && p_pos > 0);
            size_type pos { 0 };
            for (auto it = begin(); it != end(); ++it, pos++)
                if (pos == p_pos)
                    return *it;
            return {};
        }

        template <typename compare_func = std::less<_Tp>>
        void sort (compare_func comp = std::less<_Tp>())
        {
            if (size() <= 1) return;
            value_type pivot = *begin();
            pop_front();

            self_type less;
            self_type greater;

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

        self_type reversed () const {
            forward_list out;
            for (auto it = begin(); it != end(); ++it)
                out.push_front(*it);
            return out;
        }

        self_type & reverse () {
            return *this = reversed();
        }
    };

} // nx

#endif //NX_FORWARD_LIST_H
