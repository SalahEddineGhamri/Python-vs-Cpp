#include <iostream>
#include <array>
#include <cstddef>   // std::size_t, std::ptrdiff_t
#include <iomanip>   // std::hex

using std::cout;

/*
  RULES:
  - Pointers are typed addresses.
  - Arithmetic is scaled by sizeof(T).
  - Dereference only valid, aligned, live objects.
*/

int main() {
    cout << "=== Address printing ===\n";
    int x = 42;
    int* px = &x;

    cout << "x value        = " << x << "\n";
    cout << "&x (address)   = " << &x << "\n";   // prints as pointer
    cout << "px (same addr) = " << px << "\n";
    cout << "*px (deref)   = " << *px << "\n";

    // Best practice: print addresses explicitly as void*
    cout << "addr(void*)   = " << static_cast<void*>(px) << "\n\n";

    cout << "=== Dereferencing ===\n";
    *px = 100;              // write through pointer
    cout << "x after write = " << x << "\n";

    // Pitfall: null dereference (UB)
    int* pnull = nullptr;
    // *pnull = 1;           // ❌ UB — never do this

    // Best practice: guard
    if (pnull) { /* safe */ }

    cout << "\n=== Pointer arithmetic ===\n";
    std::array<int, 4> a{10, 20, 30, 40};
    int* p = a.data();      // points to a[0]

    cout << "p        = " << static_cast<void*>(p) << "\n";
    cout << "p + 1    = " << static_cast<void*>(p + 1)
         << "  (+" << sizeof(int) << " bytes)\n";

    cout << "*p       = " << *p << "\n";
    cout << "*(p+2)   = " << *(p + 2) << "\n";   // a[2]

    // Pointer difference (elements, not bytes)
    std::ptrdiff_t diff = (p + 3) - p;
    cout << "diff elems = " << diff << "\n";

    // Pitfall: out-of-bounds arithmetic (even without deref is UB in many cases)
    // int* bad = p + 4;     // ❌ past-the-end only allowed for comparison, not deref

    cout << "\n=== Const correctness (Very Important) ===\n";
    const int cx = 7;
    const int* pc = &cx;    // pointer to const
    // *pc = 8;             // ❌ cannot modify

    int y = 9;
    int* const cp = &y;     // const pointer
    *cp = 10;               // OK
    // cp = &x;             // ❌ cannot reseat

    cout << "\n=== Idiomatic alternatives ===\n";
    // Prefer references when null is not meaningful
    int& rx = x;
    rx = 123;

    // Prefer containers/iterators to raw arithmetic
    for (auto it = a.begin(); it != a.end(); ++it)
        cout << *it << " ";
    cout << "\n";

    // Prefer span (C++20) / gsl::span for bounds-safe views (hint)
}
