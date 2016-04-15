#ifndef ST_LIST_H
#define ST_LIST_H

#include "st_iterator.h"
#include "st_allocator.h"
#include "st_algorithm.h"

namespace tinySTL {

template <class T>
    struct list_node {
        typedef void* void_ptr;
        void_ptr   next;
        void_ptr   prev;
        T   data;
    };

template <class T, class Ref, class Ptr>
    class list_iterator {
        public:
            typedef list_iterator<T, T&, T*> iterator;
            typedef list_iterator<T, Ref, Ptr>  self;

            typedef bidirectional_iterator_tag  iterator_category;
            typedef T   value_type;
            typedef Ptr  pointer;
            typedef Ref  reference;
            typedef list_node<T>*   link_type;
            typedef size_t  size_type;
            typedef ptrdiff_t   difference_type;

            link_type   node;

            list_iterator() {}
            list_iterator (link_type x) :node(x) {}
            list_iterator (const iterator& x) :node(x.node) {}

            bool operator==(const self& x) const {return x.node == node; }
            bool operator!=(const self& x) const { return node != x.node; }

            reference operator* () const { return node->data; }
            pointer operator-> () const {return &(operator*()); }
            self& operator++() {
                node = static_cast<link_type>(node->next);
                return *this;
            }

            self operator++(int) {
                self tmp = *this;
                node = static_cast<link_type>(node->next);
                return tmp;
            }

            self& operator--() {
                node = static_cast<link_type>(node->prev);
                return *this;
            }

            self operator--(int) {
                self tmp = *this;
                --*this;
                return tmp;
            }
};

template <class T, class Alloc = SimpleAlloc<T> >
class list {
protect:
    typedef _list_node<T>   list_node;
public:
    typedef list_node*      link_type;
    typedef T               value_type;
    typedef list_node*      iterator;
    typedef const list_node*    const_iterator;
    typedef size_t              size_type;
    typedef T&                 reference;
    typedef const T&           const_reference;

protect:
    link_type   node;

public:

    iterator begin() { return (link_type) (*node).next; }
    const_iterator begin() const { return (link_type) (*node).next; }
    const_iterator cbegin() const { return (link_type) (*node).next; }

    iterator end() { return node; }
    const_iterator end() const { return node; }
    const_iterator cend() const {return node; }

    reference back() { return &(node->prev->data); }
    const_reference back() const { return &(node->prev->data); }

    reference front() { return &(node->next->data); }
    const_reference front() const { return &(node->next->data); }

    bool empty() const { node == node->next; }



    size_type size() const { return distance(begin(), end()); }




};



}
#endif
