/*==============================================================================
# Title           : main.cpp of Task2App
# Description     : This is a c++ app to implement "SortFunction":
#                   A function that sorts a Data container following the sorting
#                   pattern of another container.
# Author          : Salah Eddine Ghamri
# Date            : 18-11-2018
# Version         : 1.0
# Usage           : Compile using Cmake.
# Notes           : We will admit STL vector as a default container, since "or"
//                  is used in the task resume.
//                  The function will not modify the original data. So it will
//                  return a zipped form of data that needs more processing
//                  to diplay the requested results.
# C++_version     : C++14
# //TODO          : ...
# ==============================================================================
*/
#include "Myfunctions.hpp"

//Iterate vectors
IntV IntegerVector = {0, 4, 2, 8, 4};
StrV StringVector = {"A", "B", "C", "D", "E"};

int main(){
    // We assume using STL Vectors
    // STL lists do not support random access, mixing Vector - list enteries
    // demands profound reflexion - or at least overloading operator[]
    std::pair<StrV, IntV> Result;
    try{
        // Smaller is a binary function
        Result = SortFunctionOne(StringVector, IntegerVector, Smaller);
    } catch (char const* e) {
        std::cout << e << std::endl;
        return EXIT_FAILURE;
    }
    for (auto &element:Result.first)
        std::cout << element << " ";
    printf("\n");
    for (auto &element:Result.second)
        std::cout << element << " ";
    printf("\n");
    return EXIT_SUCCESS;
}

/*
We can take a "python zip" approach. merge the enteries in a single container
and we use std::sort function from algorithms.
*/
