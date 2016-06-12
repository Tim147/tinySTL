#ifndef ST_RB_TREE
#define ST_RB_TREE
#include "st_allocator.h"
#include "st_construct.h"
#include "st_pair.h"

namespace tinySTL {

typedef bool _color_type;
const _color_type   _red = false;
const _color_type   _black = true;

class rb_tree_base {
    public:
        typedef rb_tree_base*   base_ptr;

        _color_type color;
        base_ptr    head;
        base_ptr    parent;
        base_ptr    left;
        base_ptr    right;

        static  base_ptr  minimum (base_ptr p) {
            while (p->left != 0) p = p->left;
            return p;
        }

        static base_ptr maximum (base_ptr p) {
            while (p->right != 0) p = p->right;
            return p;
        }

};

template <class T>
class _rb_tree_node : public rb_tree_base {
public:
    typedef _rb_tree_node<T>*   link_type;
    typedef rb_tree_base::base_ptr base_ptr;
    typedef T                   value_type;
    value_type  value_field;
};

struct  _rb_tree_base_iterator {
    typedef rb_tree_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag  iterator_category;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;

    base_ptr   node;

    void increment () {
        if (node->right != 0) {
            node = node->right;
            while (node->left != 0) node = node->left;

        } else {
            base_ptr cur = node->parent;
            while (node == cur->right) {
                node = node->parent;
                cur = cur->parent;
            }
            if (node->right != cur )
                node = cur;
        }
    }

    void decrement () {
        if (node->color == _red && node->parent == node->parent->parent) 
            node = node->right;
        else if ( node->left != 0) {
            node = node->left;
            while (node->right != 0) node = node->right;
        } else {
            base_ptr cur = node->parent;
            while (node == cur->left) {
                cur = cur->parent;
                node = node->parent;
            }
            node = cur;
        }
    }
};

template <class T>
    struct rb_tree_iterator : public _rb_tree_base_iterator {
        typedef T                                           value_type;
        typedef T&                                          reference;
        typedef T*                                          pointer;
        typedef rb_tree_iterator<T>                         iterator;
        typedef const rb_tree_iterator<T>                   const_iterator;
        typedef rb_tree_iterator<T>*                        link_type;
        typedef typename _rb_tree_base_iterator::base_ptr   base_ptr;

        rb_tree_iterator() {}
        rb_tree_iterator(base_ptr x) { node = x; }
        rb_tree_iterator(const_iterator& x) { node = x.node; }
        

        reference operator* () const { return link_type(node)->value_field; }
        pointer operator-> () const { return &(operator*()); }

        iterator& operator++ () {
            increment ();
            return *this;
        }

        iterator operator++ (int ) {
            iterator tmp = *this;
            increment();
            return tmp;
        }

        iterator& operator-- () {
            decrement();
            return *this;
        }

        iterator operator-- (int) {
            iterator tmp = *this;
            decrement();
            return tmp;
        }
    };

template <class Key, class Value, class KeyofValue, class Compare, class Alloc = SimpleAlloc >
class rb_tree {
    public:
        typedef _rb_tree_node<Value>                        rb_tree_node;
        typedef simple_alloc<rb_tree_node, Alloc>           rb_tree_node_allocator;
        typedef _color_type                                 color_type;
        
        typedef Key                                         key_type;
        typedef Value                                       value_type;
        typedef value_type*                                 pointer;
        typedef const value_type*                           const_pointer;
        typedef value_type&                                 reference;
        typedef const value_type&                           const_reference;
        typedef rb_tree_node*                               link_type;
        typedef typename rb_tree_node::base_ptr             base_ptr;
        typedef size_t                                      size_type;
        typedef ptrdiff_t                                   difference_type;

    protected:
        
        link_type get_node () { return rb_tree_node_allocator::allocate(1); }
        link_type put_node (link_type p) { rb_tree_node_allocator::deallocate(p); }

        link_type create_node (const value_type& val) {
            link_type tmp = get_node();
            construct (&tmp->value_field, val);
            return tmp;
        }

        link_type clone_node (link_type p) {
            link_type tmp = create_node (p->value_field);
            tmp->color = p->color;
            tmp->left = 0;
            tmp->right = 0;
            return tmp;
        }

        void destroy_node (link_type p) {
            destroy (&p->value_field);
            put_node(p);
        }

        void free_node (link_type p) {
            if (p == 0 || p == header) return ;
            if (p->left != 0) free_node ((link_type)(p->left));
            if (p->right != 0) free_node ((link_type)(p->right));
            destroy_node (p);
            return ;
        }

    protected:
        size_type   node_num;
        link_type   header;
        Compare     key_compare;

        link_type& root ()  { return (link_type&) header->parent; }
        link_type& leftmost ()  { return (link_type&) header->left; }
        link_type& rightmost ()  { return (link_type&) header->right; }

        static link_type& left (link_type x) { return (link_type&) x->left; }
        static link_type& right (link_type x) { return (link_type&) x->right; }
        static link_type& parent (link_type x) { return (link_type&) x->parent; }

        static reference value (link_type x) { return x->value_field; }
        static const Key key (link_type X) { return KeyofValue() (value(X)); }
        static color_type& color (link_type x) { return (color_type&) x->color; }

        static link_type minimum (link_type x) {
            return (link_type) rb_tree_base::minimum (x); 
        }

        static link_type maximum (link_type x) {
            return (link_type) rb_tree_base::maximum (x);
        }

    public:
        typedef rb_tree_iterator<value_type> iterator;
    
    private:
        //x is the insert pos, x != 0 for equal case
        iterator _insert (link_type x, link_type y, const value_type& val) {
            link_type z;
            if (y == header || x != 0 || key_compare (KeyofValue()(val), key(y))) {
                z = create_node (val);
                left(y) = z;
                if ( y == header) {
                    root() = z;
                    rightmost () = z;
                }
                else if (y == leftmost())
                    leftmost() = z;
            } else {
                z = create_node(val);
                right(y) = z;
                if (y == rightmost () )
                    rightmost() = z;
            }
            parent (z) = y;
            left (z) = 0;
            right (z) = 0;

            rb_tree_rebalance (z, header->parent);
            ++node_num;
            return iterator (z);
        }


        link_type _copy (link_type x, link_type p);


        void init () {
            header = get_node ();
            color (header) = _red;

            root() = 0;
            leftmost () = 0;
            rightmost () = 0;
        }

    public:
        rb_tree (const Compare& comp = Compare() )
            : node_num (0), key_compare (comp) { init (); }

        ~rb_tree () {
            clear ();
            put_node(header);
        }

        void _erase (base_ptr z) {
            base_ptr y = z;
            _color_type original_color = y->color;
            base_ptr x;
            if (z->left == 0) {
                if (z == leftmost()) leftmost() =(link_type) z->right;
                x = z->right;
                rb_tree_transplant (z, z->right, header->parent);
            } else if (z->right == 0) {
                if (z == rightmost()) rightmost() = (link_type)z->left;
                x = z->left;
                rb_tree_transplant (z, z->left, header->parent);
            } else {
                iterator j = iterator (z->right);
                --j;
                y = j.node;
                original_color = y->color;
                x = y->right;
                if (y->parent == z) {
                    if (x != 0) x->parent = y;
                } else {
                    rb_tree_transplant (y, y->right, header->parent);
                    y->right = z->right;
                    y->right->parent = y;
                }
                rb_tree_transplant (z, y, header->parent);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }
            if (original_color == _black)
                rb_erase_rebalance (x, header->parent);

        }

        rb_tree<Key, Value, KeyofValue, Compare, Alloc>& operator= 
            ( const rb_tree<Key, Value, KeyofValue, Compare, Alloc>& x);

    public:
        Compare key_comp () const { return key_compare; }
        iterator begin () { return leftmost(); }
        iterator end() { return header; }
        bool empty () const { return node_num == 0; }
        size_type size () const { return node_num; }
        size_type max_size () const { return size_type(-1); }
        void clear () {
            free_node (root());
            leftmost () = 0;
            rightmost () = 0;
            root() = 0;
        }

    public:

        typename tinySTL::pair<iterator, bool> insert_unique (const value_type& x) {
            link_type pre = header;
            link_type cur = root();
            bool comp = true;
            while (cur != 0) {
                pre = cur;
                comp = key_compare(KeyofValue()(x), key(cur));
                cur = comp ? left (cur) : right (cur);
            }
            iterator j = iterator (pre);
            if (comp) {
                if (j == begin()) return pair<iterator, bool>(_insert (cur, pre, x), true);
                else --j;
            }

            if (key_compare (key(j.node), KeyofValue()(x)))
                return pair<iterator, bool>(_insert (cur, pre, x), true);
            return pair<iterator, bool>(j, false);
        }

        iterator insert_equal (const value_type& x) {
            link_type pre = header;
            link_type cur = root();
            bool comp = true;
            while (cur != 0) {
                pre = cur;
                comp = key_compare (KeyofValue ()(x), key(cur));
                cur = comp ? left (cur) : right (cur);
            }
            return _insert(cur, pre, x);
        }
        
        void erase (iterator x) {
            _erase (x.node);
            destroy_node ((link_type)x.node);
        }

};

//template <class Key, class Value, class KeyofValue, class Compare, class Alloc = SimpleAlloc >
//void rb_tree<Key, Value, KeyofValue, Compare>::erase (rb_tree<Key, Value, KeyofValue, Compare>::link_type z) {
//
//}

inline void rb_tree_rotate_right (rb_tree_base* x, rb_tree_base*& root) {
    rb_tree_base* y = x->left;
    x->left = y->right;
    if (y->right) 
        y->right->parent = x;
    y->parent = x->parent;
    if (x == root) 
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

inline void rb_tree_rotate_left (rb_tree_base* x, rb_tree_base*& root) {
    rb_tree_base* y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    if (x == root) 
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

inline void rb_tree_rebalance (rb_tree_base* x, rb_tree_base*& root) {
    x->color = _red;
    while (x != root && x->parent->color == _red ) {
        if (x->parent == x->parent->parent->left) {
            rb_tree_base* y = x->parent->parent->right;
            if (y && y->color == _red) {
                x->parent->color = _black;
                y->color = _black;
                x->parent->parent->color = _red;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    rb_tree_rotate_left (x, root);
                }
                x->parent->color = _black;
                x->parent->parent->color = _red;
                rb_tree_rotate_right (x->parent->parent, root);
            }
        } else {
            rb_tree_base* y = x->parent->parent->left;
            if (y && y->color == _red) {
                x->parent->color = _black;
                y->color = _black;
                x->parent->parent->color = _red;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rb_tree_rotate_right (x, root);
                }
                x->parent->color = _black;
                x->parent->parent->color = _red;
                rb_tree_rotate_left (x->parent->parent, root);
            }
        }
    }
    root->color = _black;
}

inline void rb_erase_rebalance (rb_tree_base* x, rb_tree_base*& root) {
    while (x != root && x->color == _black) {
        if (x == x->parent->left ) {
            rb_tree_base* w = x->parent->right;
            if ( w && w->color == _red) {
                w->color = _black;
                x->parent->color = _red;
                rb_tree_rotate_left (x->parent, root);
                w = x->parent->right;
            }
            if ( (w->left == 0 || w->left->color == _black)
                    && (w->right == 0 || w->right->color == _black)) {
                w->color = _red;
                x = x->parent;
            } else {
                if (w->right == 0 || w->right->color == _black) {
                    if (w->left != 0) w->left->color = _black; //w->left won't be null
                    w->color = _red;
                    rb_tree_rotate_right (w, root);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = _black;
                if (w->right != 0) w->right->color = _black;
                rb_tree_rotate_left (x->parent, root);
                x = root;
            }
        } else {
            rb_tree_base* w = x->parent->left;
            if ( w && w->color == _red) {
                w->color = _black;
                x->parent->color = _red;
                rb_tree_rotate_right (x->parent, root);
                w = x->parent->left;
            }
            if ( (w->left == 0 || w->left->color == _black) 
                    && (w->right == 0 || w->right->color == _black)) {
                w->color = _red;
                x = x->parent;
            } else {
                if (w->left == 0 ||w->left->color == _black) {
                    if (w->right != 0) w->right->color = _black;
                    w->color = _red;
                    rb_tree_rotate_left (w, root);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = _black;
                if (w->left != 0) w->left->color = _black;
                rb_tree_rotate_right (x->parent, root);
                x = root;
            }
        }
        x->color = _black;
    }
}

inline void rb_tree_transplant (rb_tree_base* x, rb_tree_base* y, rb_tree_base*& root)  {
    if (x == root) 
        root = y;
    else if (x == x->parent->left) 
        x->parent->left = y;
    else 
        x->parent->right = y;
    if (y != 0) y->parent = x->parent;
}



}
#endif 
