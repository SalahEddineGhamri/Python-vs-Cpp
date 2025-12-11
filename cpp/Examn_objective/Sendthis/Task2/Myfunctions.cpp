//Implementation file of sortfunctions
//Author: Salah Eddine Ghamri
#include "Myfunctions.hpp"
//==============================================================================

// function implementation of SortFunctionOne
template<class V1, class V2, class T>
std::pair<V1, V2> SortFunctionOne(V1 Cont1, V2 Cont2, bool (*f)(T, T)){
    // A function to perform sorting based on a comparison relationship
    // given as argument.
    // Containers must be of same size - both subscribable( operand[])
    // returns a pair of sorted containers.
    if (Cont1.size() != Cont2.size()){
        printf("Data containers are not of the same size <!>.\n");
        throw "Size mismatch error";
    } else {

    V1 SortedCont1 = Cont1;
    V2 SortedCont2 = Cont2;
    auto temp1 = Cont1[0];
    auto temp2 = Cont2[0];
    std::pair<V1, V2> result;

    // Sorting loop
    for (int i=0; i < Cont2.size(); ++i){
        int piv = i;
        for (int j = i + 1; j < Cont2.size(); ++j){
            if ( (*f)(SortedCont2[j], SortedCont2[piv])){
                piv = j;
            }
        }
        //Sort Container 1======================================================
        temp1 = SortedCont1[i];
        SortedCont1[i] = SortedCont1[piv];
        SortedCont1[piv] = temp1;
        //======================================================================
        //Sort Container 2======================================================
        temp2 = SortedCont2[i];
        SortedCont2[i] = SortedCont2[piv];
        SortedCont2[piv] = temp2;
    }
    return std::pair<V1, V2>(SortedCont1, SortedCont2);
    }
}

// Comparator function
bool Smaller( int var1, int var2){
    // This is an implementation of " < " operand as example
    // We can implement any comparison relationship following the same logic.
    return (var1 < var2) ? true:false;
}

// To avoid linker errors related to template functions.
// Add whatever variante enteries here.
// for large projects we need to create template file instead of this.
template std::pair<StrV, IntV> SortFunctionOne<StrV, IntV, int>(StrV Cont1, IntV Cont2, bool (*f)(int, int));
