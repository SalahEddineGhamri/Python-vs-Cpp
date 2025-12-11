#include <iostream>
#include <vector>
#include <type_traits>
#include <typeinfo>
#include <cxxabi.h>
#include <memory>

// Helper to get human-readable type name
template <typename T>
std::string type_name() {
    int status;
    std::unique_ptr<char[], void(*)(void*)> res(
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status),
        std::free);
    return (status == 0) ? res.get() : typeid(T).name();
}

// Helper to print variable value and type
template <typename T>
void print_var(const std::string &name, T &&var) {
    std::cout << name << " = " << var
              << " | deduced type: " << type_name<decltype(var)>() << "\n";
}

int main() {
    std::cout << "--- Auto type deduction examples ---\n";

    // 1. Simple cases
    auto i = 42;
    auto d = 3.14;
    auto c = 'x';
    auto b = true;

    print_var("i", i);
    print_var("d", d);
    print_var("c", c);
    print_var("b", b);

    // 2. Auto with references
    int x = 10;
    int &rx = x;
    auto y = rx;           // y is int (reference ignored)
    auto &z = rx;          // z is int&

    print_var("y", y);
    print_var("z", z);
    z = 20; // modifies x
    print_var("x after modifying z", x);

    // 3. Auto with const
    const int ci = 100;
    auto a1 = ci;
    const auto a2 = ci;

    print_var("a1", a1);
    print_var("a2", a2);

    // 4. Auto with pointers
    int* p = &x;
    auto ap = p;
    auto* ap2 = p;

    std::cout << "ap points to = " << *ap
              << " | deduced type: " << type_name<decltype(ap)>() << "\n";

    // 5. Auto with expressions
    auto sum = i + d;      // deduced as double
    print_var("sum (i + d)", sum);

    // 6. Auto with containers and iterators
    std::vector<int> vec{1,2,3};
    for (auto it = vec.begin(); it != vec.end(); ++it)
        std::cout << "vec element = " << *it << " | iterator type: "
                  << type_name<decltype(it)>() << "\n";

    for (auto &elem : vec)
        elem *= 2;

    for (auto elem : vec)
        print_var("vec doubled", elem);

    // 7. Ambiguous/tricky cases
    int arr[3] = {1,2,3};
    auto arr_copy = arr;       // decays to int*
    auto &arr_ref = arr;       // preserves array type

    std::cout << "arr_copy points to first element = " << *arr_copy
              << " | type: " << type_name<decltype(arr_copy)>() << "\n";
    std::cout << "arr_ref[0] = " << arr_ref[0]
              << " | type: " << type_name<decltype(arr_ref)>() << "\n";

    // 8. decltype(auto) for exact type
    decltype(auto) r1 = x;
    decltype(auto) r2 = (x);

    print_var("r1", r1);
    print_var("r2", r2);

    return 0;
}
