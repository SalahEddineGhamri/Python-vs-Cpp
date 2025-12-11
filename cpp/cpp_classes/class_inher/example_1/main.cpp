#include <iostream>
#include "foo.h"
#include "foo.cpp"

int main(){

    container_child<int> a(5);

    a.who();

    return 0;
}