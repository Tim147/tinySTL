#ifndef ST_BK_TREE
#define ST_BR_TREE

namespace tinySTL {

typedef bool _color_type;
const _color_type   _red = false;
const _color_type   _black = true;

class rb_tree_base {
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
struct _rb_tree_node : public _rb_tree_base {
    typedef _rb_tree_node<T>*   link_type;
    typedef T                   value_type;
    value_type  value_field;
};

struct  _rb_tree_base_iterator {
    typedef _rb_tree_base::base_ptr base_ptr;
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
        typedef T   value_type;
        typedef T&  reference;
        typedef T*  pointer;
        typedef rb_tree_iterator<T> iterator;
        typedef const rb_tree_iterator<T>   const_iterator;
        typedef rb_tree_iterator<T>* link_type;

        rb_tree_iterator() {}
        rb_tree_iterator(iterator x) { node = x.node; }
        rb_tree_iterator(const_iterator x) { node = x.node; }
        

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

template <class T, class key_of_value, class compare, class Alloc = simple_alloc >
class rb_tree {
    
}





}
#endif 
