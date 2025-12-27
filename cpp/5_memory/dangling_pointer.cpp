// dangling_pointer_demo.cpp
// A complete, self-contained C++17 tutorial and demonstration of dangling pointers.
// This file compiles and runs safely, showing controlled examples of how dangling pointers
// can arise, why they are dangerous, and modern ways to avoid them.
//
// Compile with: g++ -std=c++17 -Wall -Wextra -pedantic -O2 dangling_pointer_demo.cpp -o dangling_pointer_demo
//
// Key concepts covered:
// - What is a dangling pointer?
// - Classic causes: returning address of local variable, deleting raw pointer, misplaced references
// - Undefined behavior consequences
// - Safe alternatives: smart pointers, references with proper lifetime, return by value
// - Interesting fact: Use-after-free is one of the most common security vulnerabilities (CVE sources).

#include <iostream>
#include <memory>   // for std::unique_ptr, std::shared_ptr
#include <vector>

/*
    A dangling pointer is a pointer that points to memory that is no longer valid.
    Common causes:
    1. Object goes out of scope (stack variable)
    2. Memory is explicitly deallocated (delete/delete[])
    3. Reference/pointer outlives the object it refers to

    Accessing a dangling pointer leads to undefined behavior — crashes, silent corruption, or
    seemingly correct but wrong results are all possible.
*/

int* classic_dangling_example_bad()
{
    int local = 42;
    return &local;  // Dangling! local is destroyed when function returns
}

int* safe_alternative_return_by_value()
{
    // Modern C++ prefers returning by value — compiler applies RVO/NRVO
    return new int(42);  // Still manual management, but caller now owns it
}

/*
    Why does the first function create a dangling pointer?

    int* classic_dangling_example_bad()
    {
        int local = 42;
        return &local;  // <-- Problem here
    }

    - 'local' is an automatic (stack) variable.
    - Its lifetime ends exactly when the function returns.
    - The memory it occupies is immediately reclaimed by the stack frame cleanup.
    - Returning its address gives the caller a pointer to memory that no longer belongs
      to a valid object → dangling pointer.
    - Any use of the returned pointer is undefined behavior (crash, garbage value, or
      seemingly correct but fragile output).

    Interesting fact: Most compilers warn about this with -Wall:
      "address of local variable ‘local’ returned"

    What makes the second function safe?

    int* safe_alternative_return_by_value()
    {
        return new int(42);  // <-- Different allocation
    }

    - 'new int(42)' allocates memory on the free store (heap).
    - Heap allocations live until explicitly deleted (or until program end in leaks).
    - The lifetime of this object is completely independent of the function's stack frame.
    - The returned pointer points to valid, live memory → no dangling pointer here.
    - The caller now owns the object and is responsible for deleting it.

    Trick: This version is safe from dangling, but it introduces manual memory management
    (potential leaks if caller forgets 'delete'). Modern C++ almost always prefers:

        std::unique_ptr<int> make_int() { return std::make_unique<int>(42); }

    or simply returning by value:

        int make_int() { return 42; }  // RVO/copy elision makes this efficient

    Returning by value is usually the best and safest choice in modern C++.
*/

// ---------------------------------------------------------------
// 1. Dangling pointer from returning address of local variable
// ---------------------------------------------------------------
void demo_return_local_address()
{
    std::cout << "\n=== Demo 1: Returning address of local variable ===\n";

    int* ptr = classic_dangling_example_bad();
    std::cout << "Dangling pointer value (raw address): " << ptr << "\n";
    // The following line invokes undefined behavior!
    // It may print 42, garbage, or crash — depends on compiler and luck.
    std::cout << "Dereferencing dangling pointer: " << *ptr << "  (UNDEFINED BEHAVIOR!)\n";
}

// ---------------------------------------------------------------
// 2. Dangling pointer after delete
// ---------------------------------------------------------------
void demo_use_after_delete()
{
    std::cout << "\n=== Demo 2: Use-after-delete (classic raw pointer mistake) ===\n";

    int* raw = new int(100);
    std::cout << "Valid pointer: " << *raw << "\n";

    delete raw;
    raw = nullptr;  // Good practice: set to nullptr after delete

    // If we forgot the nullptr assignment:
    // std::cout << *raw;  // Would be dangling → undefined behavior

    std::cout << "After delete and nullptr assignment: pointer is " << raw << "\n";
}

// ---------------------------------------------------------------
// 3. Safe ownership with smart pointers (recommended)
// ---------------------------------------------------------------
std::unique_ptr<int> create_unique_int(int value)
{
    return std::unique_ptr<int>(new int(value));
    // Or better: return std::make_unique<int>(value);  // exception-safe
}

void demo_smart_pointers()
{
    std::cout << "\n=== Demo 3: Smart pointers prevent dangling situations ===\n";

    {
        auto ptr = create_unique_int(200);
        std::cout << "unique_ptr value: " << *ptr << "\n";
        // ptr goes out of scope → memory automatically freed
    }  // No dangling pointer possible here

    // Shared ownership example
    auto shared1 = std::make_shared<int>(300);
    {
        auto shared2 = shared1;  // reference count becomes 2
        std::cout << "shared_ptr (inside inner scope): " << *shared2 << "\n";
    }  // reference count drops to 1, memory still alive
    std::cout << "shared_ptr (after inner scope): " << *shared1 << "\n";
    // When last shared_ptr is destroyed, memory is freed automatically
}

// ---------------------------------------------------------------
// 4. Dangling reference example (similar danger)
// ---------------------------------------------------------------
const int& bad_reference()
{
    int temp = 123;
    return temp;  // Dangling reference!
}

void demo_dangling_reference()
{
    std::cout << "\n=== Demo 4: Dangling reference ===\n";

    const int& ref = bad_reference();
    // Again, undefined behavior:
    std::cout << "Dereferencing dangling reference: " << ref << "  (UNDEFINED BEHAVIOR!)\n";
}

// ---------------------------------------------------------------
// 5. Real-world pitfall: pointer to element in resized vector
// ---------------------------------------------------------------
void demo_vector_reallocation()
{
    std::cout << "\n=== Demo 5: Pointer invalidated by vector reallocation ===\n";

    std::vector<int> vec = {1, 2, 3};
    const int* ptr_to_element = &vec[1];  // points to 2

    std::cout << "Before resize — element via pointer: " << *ptr_to_element << "\n";

    vec.reserve(1000);  // Likely causes reallocation → all pointers/references invalidated
    // vec.push_back many times would also trigger this

    // Undefined behavior if we use ptr_to_element now!
    std::cout << "After reserve(1000) — pointer is now ";
    std::cout << "dangling (do NOT dereference!)\n";

    std::cout << "Correct way — access via index: vec[1] = " << vec[1] << "\n";
}

// ---------------------------------------------------------------
// Main function — runs all controlled demos
// ---------------------------------------------------------------
int main()
{
    std::cout << "Dangling Pointer Controlled Tutorial & Demo\n";
    std::cout << "==========================================\n";

    demo_return_local_address();
    demo_use_after_delete();
    demo_smart_pointers();
    demo_dangling_reference();
    demo_vector_reallocation();

    std::cout << "\n=== Summary ===\n";
    std::cout << "- Never return pointers/references to local stack variables.\n";
    std::cout << "- Always set raw pointers to nullptr after delete.\n";
    std::cout << "- Prefer std::unique_ptr / std::shared_ptr for dynamic ownership.\n";
    std::cout << "- Be careful with pointers/references into containers (vector, string) — they invalidate on reallocation.\n";
    std::cout << "- Modern C++ (return by value, smart pointers) largely eliminates these bugs.\n";

    return 0;
}
