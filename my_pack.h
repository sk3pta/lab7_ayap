#ifndef LAB2_7_MY_PACK_H
#define LAB2_7_MY_PACK_H
#include <iostream>



template<class Type>
Type array_print(Type *array, size_t from, size_t to){
    for (size_t x = from; x < to; ++x) {
        std::cout << array[x] << " ";
    }
    std::cout << std::endl;
}


#endif //LAB2_7_MY_PACK_H
