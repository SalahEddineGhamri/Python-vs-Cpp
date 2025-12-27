// new_delete_demo.cpp
// Complete C++17 tutorial and controlled demonstration about raw 'new' / 'delete'
// and 'new[]' / 'delete[]'.
// Shows basic usage, correct pairing, common pitfalls, and why modern C++ strongly prefers
// to avoid them altogether.
//
// Compile: g++ -std=c++17 -Wall -Wextra -pedantic -O2 new_delete_demo.cpp -o new_delete_demo
//
// Key takeaway up front:
// Raw new/delete are NOT recommended in modern C++ (C++11 and later).
// They are error-prone and almost always replaced by safer alternatives
// (smart pointers, containers, return-by-value).

#include <iostream>
#include <memory>   // std::unique_ptr, std::shared_ptr, std::make_unique, std::make_shared
#include <vector>

/*
    Why do we still learn new/delete?

    - They are the foundation of dynamic memory allocation in C++.
    - Understanding them helps you read legacy code and grasp how smart pointers work internally.
    - Some very rare low-level scenarios (custom allocators, placement new) still need them.

    Basic rules you MUST follow if you ever use them:

    1. Every 'new' must be matched with exactly one 'delete'.
    2. Every 'new[]' must be matched with exactly one 'delete[]'.
    3. Never mix them: new + delete[] or new[] + delete → undefined behavior!
    4. After delete or delete[], set the pointer to nullptr (helps catch double-delete bugs).
    5. Never delete a pointer that was not obtained from new/new[].

    Common pitfalls:
    - Memory leaks (forgetting delete)
    - Double delete (corruption or crash)
    - Mismatched new/delete[] → undefined behavior (often heap corruption)
    - Exceptions between new and delete → leak

    Interesting fact:
    Mixing new/delete[] is one of the most common causes of mysterious crashes in older codebases.
    The reason is that arrays allocated with new[] store extra hidden metadata (size) that delete[]
    needs to properly call the correct number of destructors.
*/

struct Widget {
    int id;
    Widget(int i = 0) : id(i) { std::cout << "Widget " << id << " constructed\n"; }
    ~Widget() { std::cout << "Widget " << id << " destroyed\n"; }
};

// ---------------------------------------------------------------
// 1. Basic single-object new / delete
// ---------------------------------------------------------------
void demo_single_object()
{
    std::cout << "\n=== Demo 1: Single object new/delete ===\n";

    Widget* p = new Widget(1);     // dynamic allocation
    std::cout << "Using widget: id = " << p->id << "\n";

    delete p;                      // correct deallocation
    p = nullptr;                   // good practice
}

// ---------------------------------------------------------------
// 2. Array new[] / delete[] — correct pairing
// ---------------------------------------------------------------
void demo_array_correct()
{
    std::cout << "\n=== Demo 2: Array allocation (correct) ===\n";

    const size_t N = 3;
    Widget* arr = new Widget[N]{10, 20, 30};  // calls constructor for each element

    for (size_t i = 0; i < N; ++i) {
        std::cout << "arr[" << i << "].id = " << arr[i].id << "\n";
    }

    delete[] arr;   // MUST use delete[] for arrays
    arr = nullptr;
}

// ---------------------------------------------------------------
// 3. Dangerous mismatch — new + delete[] (UB!)
// ---------------------------------------------------------------
void demo_mismatch_bad()
{
    std::cout << "\n=== Demo 3: Mismatched new + delete[] (UNDEFINED BEHAVIOR) ===\n";
    std::cout << "This often 'works' in debug builds but crashes in release!\n";

    Widget* p = new Widget(999);   // single object

    // delete[] p;   // ← VERY BAD! We comment it out to keep the program running safely
    std::cout << "We intentionally avoided executing mismatched delete[]\n";

    delete p;      // correct way
    p = nullptr;
}

// ---------------------------------------------------------------
// 4. Modern C++ alternatives — why we avoid raw new/delete
// ---------------------------------------------------------------
void demo_modern_alternatives()
{
    std::cout << "\n=== Demo 4: Modern safe alternatives ===\n";

    // 4.1 unique_ptr for single objects (exclusive ownership)
    {
      auto p1 = std::make_unique<Widget>(100);
      std::cout << "unique_ptr widget id: " << p1->id << "\n";
    } // ← automatically deleted when p1 goes out of scope — no leak possible

    {
      // 4.2 vector for dynamic arrays (resizable, safe, exception-safe)
      std::vector<Widget> vec;
      vec.emplace_back(200);
      vec.emplace_back(201);
      vec.emplace_back(202);
      std::cout << "vector size: " << vec.size() << "\n";
    } // ← automatically destroys all elements when vec is destroyed

    {
      // 4.3 shared_ptr when shared ownership is truly needed
      auto sp = std::make_shared<Widget>(300);
      {
          auto sp2 = sp;  // reference count increases
          std::cout << "shared widget id (inner scope): " << sp2->id << "\n";
      }  // reference count decreases, memory still alive
      std::cout << "shared widget id (after inner scope): " << sp->id << "\n";
    } // ← destroyed automatically when last shared_ptr is gone
}

// ---------------------------------------------------------------
// Main
// ---------------------------------------------------------------
int main()
{
    std::cout << "Raw new/delete Tutorial & Demo\n";
    std::cout << "==============================\n";

    demo_single_object();
    demo_array_correct();
    demo_mismatch_bad();
    demo_modern_alternatives();

    std::cout << "\n=== Summary ===\n";
    std::cout << "- Always pair new ↔ delete and new[] ↔ delete[]\n";
    std::cout << "- Set pointers to nullptr after delete/delete[]\n";
    std::cout << "- In modern C++ (C++11+), raw new/delete should be avoided.\n";
    std::cout << "- Use std::unique_ptr / std::make_unique for single objects\n";
    std::cout << "- Use std::vector / std::string / other containers for arrays\n";
    std::cout << "- Use std::shared_ptr only when real shared ownership is required\n";
    std::cout << "- These alternatives are exception-safe and prevent leaks automatically.\n";

    std::cout << "\nTrick: Many code bases enforce 'no raw new/delete' with static analysis tools.\n";
    std::cout << "Google C++ Style Guide and C++ Core Guidelines explicitly recommend banning them.\n";

    return 0;
}
