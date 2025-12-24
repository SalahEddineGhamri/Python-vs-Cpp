// std_stack_tutorial.cpp
// A complete, self-contained tutorial and demonstration of std::stack in C++17
// Covers: construction, push/emplace, pop, top, size/empty,
//         usage with different underlying containers,
//         common patterns, pitfalls, and constraints.
//
// Compile with: g++ -std=c++17 -Wall -Wextra -O2 std_stack_tutorial.cpp -o std_stack_tutorial
//
// This file builds cleanly and runs on any C++17-compliant compiler.

#include <iostream>
#include <stack>
#include <vector>
#include <deque>
#include <list>
#include <string>

/*
    std::stack is a container adaptor that provides a LIFO (Last-In-First-Out) interface.
    It restricts access to only the top element — no random access, no iterators.

    By default, it uses std::deque as the underlying container.
    You can specify any container that supports:
        - push_back
        - pop_back
        - back
        - empty
        - size (optional)

    Common underlying containers: std::deque (default), std::vector, std::list.

    Interesting fact: std::stack has no iterators and no begin()/end() — it's intentionally limited
    to enforce the stack abstraction.

    Key operations (all O(1) amortized for default deque):
        - push / emplace  → add to top
        - pop             → remove from top (does NOT return the element!)
        - top             → access top element
        - empty / size

    Pitfall: pop() does not return the removed element — you must call top() first if you need the value.
*/

struct Operation {
    std::string name;
    int value;

    Operation(std::string n, int v) : name(std::move(n)), value(v) {}

    friend std::ostream& operator<<(std::ostream& os, const Operation& op) {
        os << op.name << ": " << op.value;
        return os;
    }
};

int main() {
    /*
        1. Basic stack with default underlying container (std::deque<int>)
    */
    std::stack<int> s;
    std::cout << "Empty stack: empty=" << std::boolalpha << s.empty()
              << ", size=" << s.size() << '\n';

    /*
        2. Adding elements: push vs emplace
    */
    s.push(10);                    // copies/moves existing int
    s.push(20);
    s.emplace(30);                 // constructs int in-place (more efficient for complex types)

    std::cout << "\nAfter pushing 10, 20, 30:\n";
    std::cout << "Top element: " << s.top() << '\n';
    std::cout << "Size: " << s.size() << '\n';

    /*
        3. Accessing and removing
    */
    std::cout << "\nPopping elements (LIFO order):\n";
    while (!s.empty()) {
        std::cout << "Top: " << s.top() << " -> popping\n";
        s.pop();                   // removes the top element, returns void
    }

    /*
        4. Real-world example: storing complex objects
    */
    std::stack<Operation> history;
    history.emplace("Load", 100);
    history.emplace("Process", 200);
    history.emplace("Save", 300);

    std::cout << "\nOperation history (most recent first):\n";
    while (!history.empty()) {
        std::cout << history.top() << '\n';
        history.pop();
    }

    /*
        5. Changing the underlying container
           Example: use std::vector instead of default deque
           Useful when you want contiguous memory and cache-friendly access internally
    */
    std::stack<int, std::vector<int>> vec_stack;
    vec_stack.push(1);
    vec_stack.push(2);
    vec_stack.emplace(3);

    std::cout << "\nStack using std::vector as underlying container:\n";
    while (!vec_stack.empty()) {
        std::cout << vec_stack.top() << ' ';
        vec_stack.pop();
    }
    std::cout << '\n';

    /*
        6. Another underlying container: std::list
           Good when you need frequent insertions/removals internally (though stack doesn't expose them)
    */
    std::stack<std::string, std::list<std::string>> list_stack;
    list_stack.push("first");
    list_stack.emplace("second");
    list_stack.push("third");

    std::cout << "\nStack using std::list as underlying container:\n";
    while (!list_stack.empty()) {
        std::cout << list_stack.top() << ' ';
        list_stack.pop();
    }
    std::cout << '\n';

    /*
        7. Common pattern: temporary storage with safe cleanup (e.g., undo stack)
    */
    std::stack<int> backup;
    backup.push(42);
    backup.push(100);

    // Simulate rollback
    std::cout << "\nRolling back last operation:\n";
    if (!backup.empty()) {
        std::cout << "Undo value: " << backup.top() << '\n';
        backup.pop();
    }

    /*
        8. Important pitfalls and best practices

        - pop() does NOT return the element → always use top() first if you need the value.
          Wrong: int x = s.pop();          // compile error!
          Correct: int x = s.top(); s.pop();

        - No iterators, no indexing, no begin()/end() → you cannot loop over all elements directly.
          If you need to inspect all elements, consider using a different container.

        - All operations are fast (O(1)) as long as the underlying container supports them efficiently.

        - Prefer emplace() over push() when constructing complex objects to avoid temporary copies.

        - You can compare two stacks with ==, !=, <, >, <=, >= (lexicographical comparison of underlying sequences).

        - std::stack is NOT thread-safe by default — protect with mutex if used concurrently.

        Trick: If you need both stack and queue behavior, use the underlying container directly
        (e.g., std::deque) instead of adapting it.
    */

    // Demonstrate comparison (requires same container type)
    std::stack<int> s1, s2;
    s1.push(1); s1.push(2);
    s2.push(1); s2.push(2);
    std::cout << "\nStack comparison: s1 == s2 ? " << std::boolalpha << (s1 == s2) << '\n';

    return 0;
}
