#ifndef ST_LIST_H
#define ST_LIST_H

#include "st_iterator.h"
#include "st_allocator.h"
#include "st_algorithm.h"
#include "st_construct.h"

namespace tinySTL {

template <class T>
    struct _list_node {
        typedef void* void_ptr;
        void_ptr   next;
        void_ptr   prev;
        T   data;
    };

template <class T, class Ref, class Ptr>
    class list_iterator {
        public:
            typedef list_iterator<T, T&, T*> iterator;
            typedef list_iterator<T, Ref , Ptr>  self;

            typedef bidirectional_iterator_tag  iterator_category;
            typedef T   value_type;
            typedef Ptr  pointer;
            typedef Ref  reference;
            typedef _list_node<T>*   link_type;
            typedef size_t  size_type;
            typedef ptrdiff_t   difference_type;

            link_type   node;

            list_iterator() {}
            list_iterator (link_type x) :node(x) {}
            list_iterator (const iterator& x) :node(x.node) {}

            bool operator==(const self& x) const {return x.node == node; }
            bool operator!=(const self& x) const { return node != x.node; }

            reference operator* () const { return (*node).data; }
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

template <class T, class Alloc = SimpleAlloc<_list_node<T> > >
class list {
protected:
    typedef _list_node<T>   list_node;
    
public:
    typedef list_node*      link_type;
    typedef T               value_type;
    typedef list_iterator<T,T&,T*>      iterator;
    typedef const iterator    const_iterator;
    typedef size_t              size_type;
    typedef T&                 reference;
    typedef const T&           const_reference;

protected:
    link_type   node;

private:
    Alloc node_allocator;

private:
    link_type get_node () { return node_allocator.allocate(1); }

    void put_node(link_type ptr) {
        node_allocator.deallocate(ptr);
    }

    link_type create_node(const T& data) {
        link_type p = get_node();
        construct(&(p->data), data);
        return p;
    }
     void destroy_node(link_type ptr) {
        destroy(&ptr->data);
        put_node(ptr);
     }

    void empty_initialize() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }

    iterator insert_aux (const_iterator position, const value_type& val) {
        iterator cur = position;
        link_type prev = (link_type) cur.node->prev;
        link_type result = create_node(val);
        prev->next = result;
        result->prev = prev;
        result->next = cur.node;
        cur.node->prev = result;
        return result;
    }

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

    iterator rbegin() { return  (link_type) (*node).prev; }
    const_iterator rbegin() const { return (link_type) (*node).prev; }
    const_iterator crbegin() const { return (link_type) (*node).prev; }

    iterator rend() { return (link_type) node; }
    const_iterator rend() const { return (link_type) node; }
    const_iterator crend() const {return (link_type) node; }

    bool empty() const { node == node->next; }

    size_type size() const { return distance(begin(), end()); }
    //Constructs a list container object, initializing its contents depending on the constructor version used
    list() { empty_initialize(); }
    list (iterator first, iterator last) {
        link_type cur = node;
        for(; first != last; ++first, ++cur) {
            link_type result = create_node(first.node->data);
            result->next = cur->next;
            cur->next = result;
        }
    }

    list (const list& x) {
        list(x.begin(), x.end());
    }

    ~list() {
        while(node->next != node) {
            link_type tmp = (link_type) node->next;
            node->next = tmp->next;
            destroy_node(tmp);
        }
        put_node(node);
    }
    //Assigns new contents to the container, replacing its current contents
    list& operator= (const list& x) {
        node = x.node;
        return *this;
    }

    //inserting new elements before the element at the specified position
    iterator insert (const_iterator position, const value_type& val) {
        return insert_aux (position, val);
    }

    iterator insert (const_iterator position, size_type n, const value_type& val) {
       iterator cur = position;
       iterator start = cur;
       for (size_type i = 0; i < n; ++i) {
            cur = insert_aux (cur, val); //return the position after inserted element
       }
       return cur;
    }
    template <class InputIterator>
    iterator insert (const_iterator position, InputIterator first, InputIterator last) {
        iterator cur = position;
        iterator start = cur;
        for (; first != last; ++first, ++cur) {
            insert_aux (cur, first->data);
        }
        return start;
    }
    //Adds a new element at the end of the list container,
    void push_back (const value_type& val) {
        insert_aux (node, val);
    }
    //Inserts a new element at the beginning of the list
    void push_front (const value_type& val) {
        insert_aux ((link_type)node->next, val);
    }
    //Removes the last element in the list container, effectively reducing the container size by one
    void pop_back () {
        if (node->next != node)
            erase ((link_type)node->prev);
    }
    //Removes the first element in the list container, effectively reducing its size by one
    void pop_front () {
        if (node->next != node)
            erase ((link_type)node->next);
    }

    //Assigns new contents to the list container, replacing its current contents, and modifying its size accordingly
    template <class InputIterator>
        void assign (InputIterator first, InputIterator last) {
            size_type size = distance(begin(), end());
            size_type n = distance(first, last);
            iterator it = (link_type) node->next;
            for (size_type i = 0; i < n; ++i, ++first) {
                if (i < size)
                    (it++).node->data = first.node->data;
                else push_back(first.node->data);
            }
            if (n < size)
                erase(it, node);
        }
    void assign (size_type n, const value_type& val) {
        size_type size = distance(begin(), end());
        iterator it = begin();
        for (size_type i = 0; i < n; ++i, ++it) {
            if (i <size )
                it.node->data = val;
            else insert_aux (node, val);
        }
        if ( n < size )
            erase(it, node);
    }
    //Removes from the list container either a single element (position) or a range of elements
    iterator erase (const_iterator position) {
       if(position != node) {
         link_type cur = (link_type) position.node->prev; 
         link_type result = (link_type) position.node->next;
         cur->next = result;
         result->prev = cur;
         destroy_node(position.node);
         return result;
       }
    }
        
    iterator erase (const_iterator first, const_iterator last ) {
        if (last != (link_type)node->next) {
            link_type cur = (link_type)first.node->prev;
            link_type result = (link_type) last.node;
            cur->next = result;
            result->prev = cur;
            link_type ptr = (link_type) first.node;
            while (ptr != last.node) { 
                link_type tmp = (link_type)ptr->next;
                destroy_node(ptr);
                ptr = tmp;
            }
            return result;
        }
    }
    //Returns a copy of the allocator object associated with the list container
    Alloc get_allocator () const { return node_allocator; }
    //Removes all elements from the list container  
    void clear () { erase(begin(), end()); }
    //Returns the maximum number of elements that the list container can hold.
    size_type max_size() const { return size_type(-1); }
    //Merges x into the list by transferring all of its elements at their respective ordered positions into the container 
    //This function requires that the list containers have their elements already ordered by value (or by comp) before the cal
    void merge (list& x) {
        link_type cur = (link_type) node->next;
        link_type xcur = (link_type) x.node->next;
        while (cur != node) {
            if (xcur == x.node) break; 
            if ( cur->data > xcur->data ) {
                link_type tmp = (link_type) xcur->next;
                link_type prevtmp = (link_type)cur->prev;
                xcur->next = cur;
                prevtmp->next = xcur;
                xcur->prev = cur->prev;
                cur->prev = xcur;
                xcur = tmp;
            } else cur = (link_type)cur->next;
        }
        if (xcur != x.node) {
            link_type prevt = (link_type)cur->prev;
            prevt->next = xcur;
            xcur->prev = cur->prev;
            link_type tmp = (link_type)x.node->prev;
            tmp->next = cur;
            cur->prev = x.node->prev;
            x.node->next = x.node;
            x.node->prev = x.node;
        }

    }
    template <class Compare>
        void merge (list& x, Compare comp) {
            link_type cur = (link_type) node->next;
            link_type xcur = (link_type) x.node->next;
            while (cur != node) {
                if (xcur == x.node) break; 
                if ( comp(cur->data, xcur->data) ) {
                    link_type tmp = (link_type) xcur->next;
                    link_type prevtmp = (link_type)cur->prev;
                    xcur->next = cur;
                    prevtmp->next = xcur;
                    xcur->prev = cur->prev;
                    cur->prev = xcur;
                    xcur = tmp;
                } else cur = (link_type)cur->next;
            }
            if (xcur != x.node) {
                link_type prevt = (link_type)cur->prev;
                prevt->next = xcur;
                xcur->prev = cur->prev;
                link_type tmp = (link_type)x.node->prev;
                tmp->next = cur;
                cur->prev = x.node->prev;
                x.node->next = x.node;
                x.node->prev = x.node;
            }

        }
    //Removes from the container all the elements that compare equal to val
    void remove (const value_type& val) {
        link_type cur = (link_type) node->next;
        while (cur != node) {
            if (cur->data == val) {
                link_type _prev = (link_type) cur->prev;
                link_type _next = (link_type) cur->next; 
                _prev->next = _next;
                _next->prev = _prev;
                destroy_node(cur);
                cur = _next;
            }
            else cur = (link_type)cur->next;
        }
    }
    template <class Predicate>
        void remove_if (Predicate pred) {
            link_type cur = (link_type) node->next;
            while (cur != node) {
                if (pred(cur->data)) {
                    link_type _prev = (link_type) cur->prev;
                    link_type _next = (link_type) cur->next; 
                    _prev->next = _next;
                    _next->prev = _prev;
                    destroy_node(cur);
                    cur = _next;
                }
                else cur = (link_type)cur->next;
            }
        }
    //Resizes the container so that it contains n elements
    void resize (size_type n, const value_type& val) {
        iterator cur = begin();
        size_type i = 0;
        while (cur != end()) {
            ++i;
            if (i > n)
                cur = erase(cur, end());
            else ++cur;
        }
        if ( i < n )
            insert (cur, n-i, val);
    }
    //Resizes the container so that it contains n elements
    void resize (size_type n) {
        resize (n, value_type());
    }
    //Reverses the order of the elements in the list container
    void reverse () {
       link_type cur = (link_type)node->next;
       while (cur != node) {
           link_type _next = (link_type)cur->next;
           cur->next = cur->prev;
           cur->prev = _next;
           cur = _next;
       }
       link_type tmp = (link_type) node->next;
       node->next = node->prev;
       node->prev = tmp;
    }
    //Sorts the elements in the list, altering their position within the container
    void sort() {

    }
    template <class Compare>
        void sort (Compare comp) {
        }
};

}
#endif
