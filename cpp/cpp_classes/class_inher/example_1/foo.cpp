#include "foo.h"
#include <iostream>

template<typename T>
container<T>::container(){}

template<typename T>
container<T>::container(T a):a{a}{
    std::cout << "I am father" << std::endl;
}

template<typename T>
void container<T>::who(){
    std::cout << "This is " << this-> a << std::endl;
}

//child constructor that call father with type
template<typename T>
container_child<T>::container_child(T x):container<T>(x){}