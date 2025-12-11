#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;


template <typename T>
struct Comparator : public std::binary_function<T,T,bool>
{
    bool operator()(const T& o1, const T& o2)
    {
        return o1.second < o2.second;
    }
};

/*
vector<string> A = {"a", "b", "c", "d"};
list<int> B = {4, 2, 1, 3};

*/
template <class T>
bool myway(const T& var1, const T& var2){
    if (var1.second < var2.second)
    return true;
    else
    return false;
}
/*
template <class T1, class T2 >
std::pair<T1, T2> MakePair(T1 element1, T2 element2){
    return std::pair<T1, T2>(element1, element2);
}
*/
//Example vector<string> = T1
//        vector<int> = T2
//        string = Var1
//        int = Var2
//        pair(sting, int) = C
//        vector<pair(sting, int)> = T
template <class T1, class T2>
std::vector<std::pair<T1, T2>>  SortFunctionTwo( T1 Cont1, T2 Cont2){
    std::vector<std::pair<T1, T2>> result;
    auto PtCont1 = Cont1.begin();
    auto PtCont2 = Cont2.begin();
    for(; PtCont1 != Cont1.end(), PtCont2 != Cont2.end(); ++PtCont1, ++PtCont2){
        result.emplace_back((*PtCont1, *PtCont2));
    }
    //std::sort(result.begin(), result.end(), *f);
    return result;
}

int main(){
    vector<string> a = {"A", "B"};
    vector<int> b = {1, 2};
    //auto d = Comparator<pair<string, int> >();
    auto result = SortFunctionTwo<vector<string>, vector<int> >(a, b);
}
