#ifndef ST_ALGORITHM_H
#define ST_ALGORITHM_H

#include "st_iterator.h"

namespace tinySTL {

    //*******************less**************************************
    template <class T>
    struct less {
        bool operator()(const T& x, const T& y) {
            return x<y;
        }
    };

    template <class T>
    const T max(const T& a, const T& b) {
        return (a > b ? a : b );
    }
    template <class T>
    const T min(const T& a, const T& b) {
        return (a < b ? a : b );
    }

    template <class T>
    void swap(T& a, T& b) {
        T tmp;
        tmp = a;
        a = b;
        b = tmp;
    }

    template <class InputIterator1, class InputIterator2>
        bool equal (InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
            while (first1 != last1) {
                if( *first1 != *first2) return false;
                ++first1; ++first2;
            }
            return true;
        }

    template <class InputIterator1, class InputIterator2, class BinaryPredicate>
        bool equal (InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, BinaryPredicate pred) {
            while (first1 != last1) {
                if(!pred(*first1, *first2)) return false;
                ++first1; ++first2;
            }
            return true;
        }

    //*******************iterartor distance**************************************
    template <class InputIterator>
        inline typename iterator_traits<InputIterator>::difference_type
        _distance(InputIterator first, InputIterator last, input_iterator_tag) {
            typename iterator_traits<InputIterator>::difference_type res = 0 ;
            while (first != last) {
                ++first; ++res;
            }
            return res;
        }
    template <class InputIterator>
        inline typename iterator_traits<InputIterator>::difference_type
        _distance (InputIterator first, InputIterator last, random_access_iterator_tag ) {
            return last - first;
        }

    
    template <class InputIterator>
        inline typename iterator_traits<InputIterator>::difference_type
        distance (InputIterator first, InputIterator last) {
            typedef typename iterator_traits<InputIterator>::iterator_category   category;
            return _distance(first, last, category());
        }

    // ************* lexicographical compare***************************************
    template <class InputIterator1, class InputIterator2>
        bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                     InputIterator2 first2, InputIterator2 last2) {
            while (first1 != last1) {
                if( first2 == last2 || *first1 > *first2) return false;
                else if(*first1 < *first2) return true;
                ++first1; ++first2;
            }
            return (first2 != last2);
        }



    template <class ForwardIterator, class Size, class T>
    ForwardIterator fill_n(ForwardIterator start, Size size, const T& x) {
        for(Size i=0; i<size; ++i)
            *(start+i) = x;
        return (start+size);
    }

    template <class ForwardIterator, class InputIterator>
    ForwardIterator copy(InputIterator start, InputIterator end, ForwardIterator result){
        for(int i=0;start + i != end; ++i, ++result) {
            *result = *(start+i);
        }
        return result;
    }

    template <class ForwardIterator, class T>
    ForwardIterator fill(ForwardIterator start, ForwardIterator end, const T& x) {
        for(;start != end; ++start)
            *start = x;
        return start;
    }

    template <class BiDirectionIterator>
    BiDirectionIterator 
    backward_copy(BiDirectionIterator first, BiDirectionIterator last, BiDirectionIterator result) {
        --last;
        for(;last != first - 1; --last, --result)
            *result = *last;
        return result;
    }




}








#endif
