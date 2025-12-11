//Make pairs
template <class T1, class T2 >
std::pair<T1, T2> MakePair(T1 element1, T2 element2){
    return std::pair<T1, T2>(element1, element2);
}

auto p = MakePair(a, b);
