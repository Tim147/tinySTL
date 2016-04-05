#ifndef CONSTRUCT_H
#define CONSTRUCT_H

#include "typetrait.h"
#include "iterator.h"
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
    inline void _destroy(ForwardIterator start, ForwardIterator end, _true_type) { }

    template <class ForwardIterator>
    inline void _destroy(ForwardIterator start, ForwardIterator end, _false_type) {
        for(; start != end; ++start)
            destroy(&*start);
    }

    template <class ForwardIterator, class T>
    inline void destroy(ForwardIterator start, ForwardIterator end, const T&) {
        // typedef typename value_type(start) T;
        //  typedef static_cast<typename iterator<start>::value_type :>
        typedef typename _type_traits<T>::is_POD_type _is_POD_type;
        _destroy(start, end, _is_POD_type());
    }
}

#endif // CONSTRUCT_H

