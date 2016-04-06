#ifndef UNINITIALLED_H
#define UNINITIALLED_H
#include "construct.h"
#include "iterator.h"
#include "typetrait.h"
#include "st_algorithm.h"

namespace tinySTL {

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator _uninitialed_fill_n_aux(ForwardIterator first, Size n, const T& x, _true_type){
        return fill_n(first, n, x); //TODO
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator _uninitialed_fill_n_aux(ForwardIterator first, Size n, const T& x, _false_type){
        ForwardIterator cur = first;
        for(; n > 0; --n, ++cur) {
            construct(&*cur, x);
        }
        return cur;
    }

    template <class ForwardIterator, class Size, class T>
    inline ForwardIterator _uninitialed_fill_n(ForwardIterator first, Size n, const T& x) {
        typedef typename _type_traits<T>::is_POD_type  _is_POD_type;
        return _uninitialed_fill_n_aux(first, n, x, _is_POD_type());
    }

    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator
        _uninitialed_copy_aux(InputIterator first, InputIterator end, ForwardIterator result, _true_type) {
        return copy(first, end, result); //TODO
    }

    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator
        _uninitialed_copy_aux(InputIterator first, InputIterator end, ForwardIterator result, _false_type) {
        ForwardIterator cur = result;
        for(; first != end; ++cur, ++first)
            construct(&*cur, *first);
        return cur;
    }


    template <class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator
        _uninitialed_copy(InputIterator first, InputIterator end, ForwardIterator result, T*) {
        typedef typename _type_traits<T>::is_POD_type _is_POD_type;
        return _uninitialed_copy_aux(first, end, result, _is_POD_type());
    }


    template <class InputIterator, class ForwardIterator>
    inline ForwardIterator
        uninitialed_copy(InputIterator first, InputIterator end, ForwardIterator result) {
        return _uninitialed_copy(first, end, result, value_type(first));

    }

    template <class ForwardIterator, class T>
    inline ForwardIterator
        _uninitialled_fill_aux(ForwardIterator first, ForwardIterator end, const T& x, _true_type) {
        return fill(first, end, x); //TODO
    }

    template <class ForwardIterator, class T>
    inline ForwardIterator
        _uninitialled_fill_aux(ForwardIterator first, ForwardIterator end, const T& x, _false_type) {
        for(; first != end; ++first)
            construct(&*first, x);
        return first;

    }

    template <class ForwardIterator, class T, class T1>
    inline ForwardIterator
        _uninitialled_fill(ForwardIterator first, ForwardIterator end, const T& x, T1 ) {
        typedef typename _type_traits<T1>::is_POD_type _is_POD_type;
        return _uninitialled_fill_aux(first, end, x, _is_POD_type());
    }

    template <class ForwardIterator, class T>
    inline ForwardIterator
        uninitialled_fill(ForwardIterator first, ForwardIterator end, const T& x) {
        return _uninitialled_fill(first, end, x, value_type(first));
    }

}
#endif // UNINITIALLED_H

