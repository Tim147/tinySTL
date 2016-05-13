#ifndef ST_LIST_H
#define ST_LIST_H

#include "st_iterator.h"
#include "st_allocator.h"
#include "st_algorithm.h"
#include "st_construct.h"
#include <assert.h>

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
                node = (link_type)(node->next);
                return *this;
            }

            self operator++(int) {
                self tmp = *this;
                node = (link_type) (node->next);
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

template <class T, class Alloc = SimpleAlloc >
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
    simple_alloc<_list_node<T> , Alloc> node_allocator;

private:
    //allocate a single node
    link_type get_node () { return node_allocator.allocate(1); }
    //deallocate a node
    void put_node(link_type ptr) {
        node_allocator.deallocate(ptr);
    }
    //construct a node and return the link_type position
    link_type create_node(const T& data) {
        link_type p = get_node();
        construct(&(p->data), data);
        return p;
    }
    //destroy a node
     void destroy_node(link_type ptr) {
        destroy(&ptr->data);
        put_node(ptr);
     }
    //initialize a empty list
    void empty_initialize() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
    //insert base function
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
    //transfer the elements from [first,last) to list before the position
    void transfer (const_iterator position, iterator first, iterator last) {
        if (position != last && first != last) {
            ((link_type) last.node->prev)->next = position.node;
            ((link_type) first.node->prev)->next = last.node;
            ((link_type) position.node->prev)->next = first.node;
            link_type tmp = (link_type)position.node->prev;
            position.node->prev = last.node->prev;
            last.node->prev = first.node->prev;
            first.node->prev = tmp;
        }
    }

public:
    //Return iterator to beginning 
    iterator begin() { return (link_type) (*node).next; }
    const_iterator begin() const { return (link_type) (*node).next; }
    const_iterator cbegin() const { return (link_type) (*node).next; }
    //Return iterator to end
    iterator end() { return node; }
    const_iterator end() const { return node; }
    const_iterator cend() const {return node; }
    //Returns a reference to the last element in the list container
    reference back() { return &(node->prev->data); }
    const_reference back() const { return &(node->prev->data); }
    //Returns a reference to the first element in the list container.
    reference front() { return &(node->next->data); }
    const_reference front() const { return &(node->next->data); }
    //Return reverse iterator to reverse beginning
    iterator rbegin() { return  (link_type) (*node).prev; }
    const_iterator rbegin() const { return (link_type) (*node).prev; }
    const_iterator crbegin() const { return (link_type) (*node).prev; }
    //Return reverse iterator to reverse end
    iterator rend() { return (link_type) node; }
    const_iterator rend() const { return (link_type) node; }
    const_iterator crend() const {return (link_type) node; }
    //Test whether container is empty
    bool empty() const {  return node == node->next; }
    //Returns the number of elements in the list container
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
    //Constructs a list container object
    list (const list& x) {
        list(x.begin(), x.end());
    }
    //Destroys the container object.
    ~list() {
        clear();
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
    //The container is extended by inserting new elements before the element at the specified position
    iterator insert (const_iterator position, size_type n, const value_type& val) {
       iterator cur = position;
       iterator start = cur;
       for (size_type i = 0; i < n; ++i) {
            cur = insert_aux (cur, val); //return the position after inserted element
       }
       return cur;
    }
    //The container is extended by inserting new elements before the element at the specified position
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
    //Assign new content to container
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
    //Removes from the list container either a single element (position) or a range of elements ([first,last)).
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
        merge (x, less<T>());
    }
    template <class Compare>
        void merge (list& x, Compare comp) {
            iterator first1 = begin();
            iterator last1 = end();
            iterator first2 = x.begin();
            iterator last2 = x.end();

            while ( first1 != last1 && first2 != last2) {
                if (!comp(*first1, *first2)) {
                    iterator _next = first2;
                    transfer(first1, first2, ++_next);
                    first2 = _next;
                } else 
                    ++first1;
            }
            if (first2 != last2) transfer(last1, first2, last2);
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
    //Remove elements fulfilling condition
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
    //removes all but the first element from every consecutive group of equal
    void unique () {
        link_type cur = (link_type) node->next;
        link_type _prev = cur;
        cur = (link_type) cur->next;
        while (cur != node) {
            if (cur->data == _prev->data) {
                link_type _next = (link_type) cur->next;
                _prev->next = _next;
                _next->prev = _prev;
                destroy_node (cur);
                cur = _next;
            } else {
                _prev = cur;
                cur = (link_type)cur->next;
            }
        }
    }
    //Exchanges the content of the container by the content of x, which is another list of the same type. Sizes may differ
    //O(n) complexity, in fact, we can exchange the node of the lists, in constants complexity
    void swap_badway (list& x) {
        link_type cur = (link_type) node->next;
        link_type xcur = (link_type) x.node->next;
        while (cur != node) {
            if (xcur == x.node) {
                link_type _next = (link_type) cur->next;
                ((link_type) cur->prev)->next = cur->next;
                ((link_type) cur->next)->prev = cur->prev;
                destroy_node (cur);
                cur = _next;
            } else {
                value_type tmp = cur->data;
                cur->data = xcur->data;
                xcur->data = tmp;
                cur = (link_type)cur->next;
                xcur = (link_type)xcur->next;
            }
        }
        while (xcur != x.node) {
            link_type _next = (link_type) xcur->next;
            ((link_type) xcur->prev)->next = xcur->next;
            ((link_type) xcur->next)->prev = xcur->prev;
            x.destroy_node (xcur);
            xcur = _next;
        }
    }
    //Constants complexity
    void swap (list& x) {
        link_type tmp = node;
        node = x.node;
        x.node = tmp;
    }
    //Transfers elements from x into the container, inserting them at position
    void splice (const_iterator position, list& x) {
        assert (node != x.node);
        if (!x.empty())
            transfer (position, x.begin(), x.end());
    }
    // i and position can be the same list
    void splice (const_iterator position, list& x, const_iterator i) {
        iterator j = i;
        ++j;
        if (i != position && j != position ) 
            transfer (position, i, j);
    }
    // position can't be in [first,last), can be the same list
    void splice (const_iterator position, list& x, iterator first, iterator last) {
        if (first != last) 
            transfer (position, first, last);
    }
    //Sorts the elements in the list, altering their position within the container
    void sort() {
        sort(tinySTL::less<T>());
    }
    //merge sort
    template <class Compare>
        void sort (Compare comp) {
            if (node->next == node || ((link_type)node->next)->next == node)
                return ;
            list<T> carry;
            list<T> counter[64];
            int fill = 0;
            while (!empty()) {
                int i = 0;
                carry.splice(carry.begin(), *this, begin());
                while (i<fill && !counter[i].empty()) {
                    counter[i].merge(carry, comp);
                    carry.swap(counter[i++]);
                }
                carry.swap(counter[i]);
                if (i==fill) ++fill;
            }

            for (int i=1; i<fill; ++i) 
                counter[i].merge(counter[i-1], comp);
            swap(counter[fill-1]);
        }
};
    //Performs the appropriate comparison operation between the list containers lhs and rhs.
    template <class T>
        inline bool operator== (const list<T>& lhs, const list<T>& rhs) {
            typename list<T>::iterator first1 = lhs.begin();
            typename list<T>::iterator first2 = rhs.begin();
            while (first1 != lhs.end()) {
                if ( first2 == rhs.end() ) return false;
                if ( *first1 != *first2 ) return false;
                ++first1; ++first2;
            }
            return (first2 == rhs.end());
        }
    template <class T>
        inline bool operator!= (const list<T>& lhs, const list<T>& rhs) {
            return !(lhs == rhs);
        }
    
    template <class T>
        inline bool operator< (const list<T>& lhs, const list<T>& rhs) {
            return lexicographical_compare (lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
        }
    template <class T>
        inline bool operator<= (const list<T>& lhs, const list<T>& rhs) {
            return (lhs < rhs || lhs == rhs);
        }
    template <class T> 
        inline bool operator> (const list<T>& lhs, const list<T>& rhs) {
            return lexicographical_compare (rhs.begin(), rhs.end(), lhs.begin(), lhs.end() );
        }
    template <class T>
        inline bool operator>= (const list<T>& lhs, const list<T>& rhs) {
            return (lhs >rhs || lhs == rhs);
        }
    //Exchanges the contents of two lists
    template <class T>
        void swap (list<T>& x, list<T>& y) {
            x.swap(y);
        }
}
#endif
