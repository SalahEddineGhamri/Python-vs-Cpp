// Header file of Sortfunctions - Task2App
// Author: Salah Eddine Ghamri
#ifndef MYFUNCTIONS_HPP
#define MYFUNCTIONS_HPP

// include dependecies =========================================================
#include <iostream>
#include <vector>
#include <string>
// typedef =====================================================================
using IntV = std::vector<int>;
using StrV = std::vector<std::string>;
//==============================================================================

//functions definitions
template<class V1, class V2, class T>
std::pair<V1, V2> SortFunctionOne(V1 Cont1, V2 Cont2, bool (*f)(T, T));
bool Smaller( int var1, int var2);

#endif // ifndef  MYFUNCTIONS_HPP
