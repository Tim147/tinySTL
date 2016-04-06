#ifndef ST_CONSTRUCT_H
#define ST_CONSTRUCT_H

#include "st_typetrait.h"
#include "st_iterator.h"
#include <new>

namespace tinySTL {
    template <class T1, class T2>
    inline void construct(T1* ptr, const T2& value) {
        new (ptr) T1(value);
    }

    template <class T1>
    inline void construct(T1* ptr) {
        new (ptr) T1();
    }

    template <class T1>
    inline void destroy(T1* ptr) {
       ptr->~T1();
    }

    template <class ForwardIterator>
    inline void _destroy_aux(ForwardIterator start, ForwardIterator end, _true_type) { }

    template <class ForwardIterator>
    inline void _destroy_aux(ForwardIterator start, ForwardIterator end, _false_type) {
        for(; start != end; ++start)
            destroy(&*start);
    }

    template <class ForwardIterator, class T>
    inline void destroy(ForwardIterator start, ForwardIterator end, T*) {
        typedef typename _type_traits<T>::is_POD_type _is_POD_type;
        _destroy_aux(start, end, _is_POD_type());
    }

    template <class ForwardIterator>
    inline void destroy(ForwardIterator start, ForwardIterator end) {
        _destroy(start, end, value_type(start) );
    }
}

#endif // CONSTRUCT_H

