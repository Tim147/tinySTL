#ifndef ST_PAIR_H
#define ST_PAIR_H

namespace tinySTL {
template <class T1, class T2>
    class pair {
        public :
            typedef T1  first_type;
            typedef T2  second_type;
        private:
            first_type first;
            second_type second;
        public:
            pair (const T1& a, const T2& b) {
                first = a;
                second = b;
            }
            ~pair () { }
    };
}


#endif
