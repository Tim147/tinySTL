#ifndef ITERATOR_H
#define ITERATOR_H
namespace tinySTL {
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag {};
    struct bidirectional_iterator_tag {};
    struct random_access_iterator_tag {};

    template <class T, class Distance>
    struct input_iterator {
        typedef input_iterator_tag  iterator_category;
        typedef T                   value_type;
        typedef Distance            difference_type;
        typedef T*                  pointer;
        typedef T&                  reference;
    };

    template <>
    struct output_iterator {
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;
    };

    template <class T, class Distance>
    struct forward_iterator {
        typedef forward_iterator_tag iterator_category;
        typedef T                    value_type;
        typedef T*                   pointer;
        typedef T&                  reference;
        typedef Distance            difference_type;

    };

    template <class T, class Distance>
    struct bidirectional_iterator {
        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef Distance                    difference_type;
    };

    template <class T, class Distance>
    struct random_access_iterator {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef Distance                    difference_type;
    };

    template <class Category, class T, class Distance = ptrdiff_t,
              class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Pointer     pointer;
        typedef Distance    difference_type;
        typedef Reference   reference;

    };

    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
        typedef typename Iterator::difference_type      difference_type;
    };

    template <class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef ptrdiff_t                   difference_type;

    };

    template <class T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef const T*                    pointer;
        typedef const T&                    reference;
        typedef ptrdiff_t                   difference_type;
    };

    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
        iterator_category(const Iterator& ) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
        value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
        difference_type(const Iterator& ) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);

    }




}
#endif // ITERATOR_H

