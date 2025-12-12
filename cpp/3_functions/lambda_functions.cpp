#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <array>
#include <memory>       // for make_unique
#include <cctype>       // for tolower
#include <iterator>   // for std::begin, std::end

using namespace std;

int main() {
    cout << "=== C++17 LAMBDA MASTERCLASS ===\n\n";

    /* ============================================================
       1. BASIC SYNTAX – What every lambda really is
       ============================================================*/
    auto lamb = [] (int x, int y) { return x + y; };
    cout << "1. Basic: 5+7 = " << lamb(5,7) << "\n\n";

    // Full general form (C++17)
    // [ capture-list ] ( parameters ) mutable noexcept -> return_type { body }

    /* ============================================================
       2. CAPTURE MODES – The heart of lambdas
       ============================================================*/
    int a = 10, b = 20;
    string msg = "Hello";

    auto by_value = [=]() {
        cout << "2.1 By value: a=" << a << " b=" << b << " msg=" << msg << "\n";
    };
    a = 999; b = 888;
    by_value();

    auto by_ref = [&]() {
        cout << "2.2 By ref: a=" << a << " b=" << b << " msg=" << msg << "\n";
        a = 111;
    };
    by_ref();
    cout << "After by_ref call, a is now " << a << "\n\n";

    int counter = 0;
    auto mixed = [counter, &msg, &b]() mutable {
        cout << "2.3 Mixed + mutable: counter=" << ++counter
             << " b=" << ++b << " msg=" << (msg += "!") << "\n";
    };
    mixed();
    mixed();
    cout << "Original counter still " << counter << " (not changed)\n";
    cout << "Original msg became '" << msg << "'\n\n";

    /* ============================================================
       3. MUTABLE LAMBDA – Allow modification of value-captured vars
       ============================================================*/
    int x = 100;
    auto mut = [x]() mutable { x = 50; cout << "3. mutable: x became " << x << "\n"; };
    mut();
    cout << "Original x still " << x << "\n\n";

    /* ============================================================
       4. CAPTURE WITH INITIALIZER (C++14+)
       ============================================================*/
    auto lambda_with_own_var = [owned = 0, ptr = make_unique<int>(42)] () mutable {
        cout << "4. Init capture: owned=" << ++owned
             << " pointed value=" << *ptr << "\n";
    };
    lambda_with_own_var();
    lambda_with_own_var();

    /* ============================================================
       5. *this, this, & capture object itself
       ============================================================*/
    struct Widget {
        int value = 999;
        void demo() const {
            auto copy_this = [*this] () {
                cout << "5.1 *this capture: value = " << value << "\n";
            };
            copy_this();

            auto ref_this = [this] () {
                cout << "5.2 this capture: value = " << value << "\n";
            };
            ref_this();
        }
    };
    Widget{}.demo();

    /* ============================================================
       6. GENERIC (templated) LAMBDAS
       ============================================================*/
    // This is c++20 templated lambda
    //auto generic = []<typename T>(T&& container) {
    //        for (auto&& elem : container)
    //            cout << elem << " ";
    //        cout << "\n";
    //    };


    auto generic = [](auto&& container) {  // Generic lambda (C++14)
        for (auto&& elem : container)      // range like this is not fit for array a c array style
            cout << elem << " ";
        cout << "\n";
    };

    auto generic_c_array = [](auto& container) {
        for (auto it = std::begin(container); it != std::end(container); ++it)
            cout << *it << " ";
        cout << "\n";
    };

    vector v{1,2,3};
    array  aa{4,5,6};
    int    c_array[] = {7,8,9};

    cout << "6. Generic lambda works on any container:\n   ";
    generic(v);
    cout << "   ";
    generic_c_array(aa);
    cout << "   ";
    generic_c_array(c_array);

    // generic
    auto print_container = [](auto&& container) {
        for (auto it = std::begin(container); it != std::end(container); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    };

    std::cout << "Single generic lambda for all containers:\n   ";
    print_container(v);
    std::cout << "   ";
    print_container(aa);
    std::cout << "   ";
    print_container(c_array);

    /* ============================================================
       7. IMMEDIATE INVOCATION (IIFE)
       ============================================================*/
    int result = [] (int x, int y) -> int { return x * y; } (6, 7);
    cout << "7. IIFE: 6*7 = " << result << "\n";

    string prefix = "[LOG]";
    ([] (string p, string m) { cout << p << " " << m << "\n"; })(prefix, "Hello from IIFE");

    /* ============================================================
       8. REAL-WORLD PATTERNS
       ============================================================*/
    vector<string> names = {"alice", "Zoe", "Bob", "charlie", "Dave"};

    // 8.1 Case-insensitive sort
    sort(names.begin(), names.end(),
         [](const string& a, const string& b) {
             return std::lexicographical_compare(
                 a.begin(), a.end(),
                 b.begin(), b.end(),
                 [](char c1, char c2) { return tolower(c1) < tolower(c2); }
             );
         });

    // 8.2 Factory returning comparator
    auto make_comparator = [](bool case_sensitive = false) -> function<bool(const string&, const string&)> {
        if (case_sensitive)
            return [](const string& a, const string& b){ return a < b; };
        else
            return [](const string& a, const string& b){
                return std::lexicographical_compare(
                    a.begin(), a.end(),
                    b.begin(), b.end(),
                    [](char c1, char c2){ return tolower(c1) < tolower(c2); }
                );
            };
    };

    cout << "\n8. Real-world examples:\n";
    for (auto& s : names) cout << s << " ";
    cout << "\n";

    /* ============================================================
       9. RECURSIVE LAMBDAS
       ============================================================*/
    function<int(int)> factorial;
    factorial = [&factorial] (int n) -> int {
        return n <= 1 ? 1 : n * factorial(n-1);
    };
    cout << "9. Recursive lambda: 7! = " << factorial(7) << "\n";

    /* ============================================================
       10. BEST PRACTICES CHEAT SHEET
       ============================================================*/
    cout << R"(
10. BEST PRACTICES (memorize this!)

Capture rules:
    [=]      → safest for short-lived lambdas
    [&]      → only when you really need to modify outside
    [this]   → avoid if the object can die
    [*this]  → preferred when copying the object is cheap/safe
    [var]    → capture only what you need
    [var=expr] → C++14 init-capture for move-only types

Prefer generic lambdas (auto parameters) – they beat function objects

Use mutable only when you really need to change a copy

Mark lambdas noexcept when possible (especially in algorithms)

Use std::function only when storing/erasing type is required
    → direct lambda is faster and has no heap allocation

Immediate invocation for one-off complex expressions

)";

    return 0;
}
