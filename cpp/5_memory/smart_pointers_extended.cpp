// smart_pointers_demo.cpp
// A complete, self-contained C++17 tutorial and demonstration of the three standard smart pointers:
//   - std::unique_ptr
//   - std::shared_ptr
//   - std::weak_ptr
//
// This single file compiles with: g++ -std=c++17 -Wall -Wextra -pedantic smart_pointers_demo.cpp -o smart_pointers_demo
//
// The code is heavily documented with multi-line comments to explain concepts simply, assuming you may be new to smart pointers.
// Comments are kept concise but highlight important facts, common pitfalls, and useful tricks.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*
    SMART POINTERS IN C++: WHY THEY EXIST
    Manual memory management with new/delete is error-prone:
    - Forgetting to delete → memory leaks
    - Deleting too early → dangling pointers / use-after-free
    - Deleting twice → undefined behavior

    Smart pointers automate ownership and lifetime management using RAII (Resource Acquisition Is Initialization).
    They destroy the managed object automatically when the last owner goes out of scope.
*/

struct Widget {
    std::string name;
    explicit Widget(std::string n) : name(std::move(n)) {
        std::cout << "Widget \"" << name << "\" constructed\n";
    }
    ~Widget() {
        std::cout << "Widget \"" << name << "\" destroyed\n";
    }
};

/*
    1. std::unique_ptr<T>  → Exclusive ownership
    - Only one unique_ptr can own an object at a time.
    - Cannot be copied (deleted copy constructor), but can be moved.
    - Zero overhead compared to raw pointer when used correctly.
    - Perfect for single-owner resources.
*/

void unique_ptr_basics() {
    std::cout << "\n=== std::unique_ptr basics ===\n";

    // Creation: prefer std::make_unique (C++14+) — it's exception-safe
    auto up1 = std::make_unique<Widget>("Alpha");

    // Access the managed object with -> or *
    up1->name = "Alpha-Modified";
    std::cout << "Access: " << (*up1).name << "\n";

    // reset() destroys the current object and takes ownership of a new one
    std::cout << "Calling reset() with a new Widget...\n";
    up1.reset(new Widget("Beta"));                // raw new is allowed but not recommended
    // Better: up1 = std::make_unique<Widget>("Beta");

    // release() relinquishes ownership → returns raw pointer and sets unique_ptr to nullptr
    // You become responsible for deleting the returned pointer!
    std::cout << "Calling release()...\n";
    Widget* raw = up1.release();
    std::cout << "After release, up1 is " << (up1 ? "not null" : "null") << "\n";
    delete raw;  // must manually delete after release!

    // Moving ownership
    auto up2 = std::make_unique<Widget>("Gamma");
    auto up3 = std::move(up2);  // up2 becomes null, up3 now owns the object
    std::cout << "After move, up2 is " << (up2 ? "not null" : "null") << "\n";
}

/*
    Common unique_ptr pitfalls:
    - Never create two unique_ptrs from the same raw pointer:
          Widget* p = new Widget("Bad");
          std::unique_ptr<Widget> a(p);
          std::unique_ptr<Widget> b(p);  // double delete → undefined behavior!
    - Don't pass raw pointers obtained via get() to another unique_ptr.
    - Use std::make_unique whenever possible.
*/

void unique_ptr_with_containers() {
    std::cout << "\n=== unique_ptr in containers (good usage) ===\n";
    std::vector<std::unique_ptr<Widget>> widgets;

    widgets.emplace_back(std::make_unique<Widget>("Vec1"));
    widgets.push_back(std::make_unique<Widget>("Vec2"));

    // Containers store the smart pointers, ownership is transferred automatically.
    // When vector is destroyed, all Widgets are properly deleted.
}

/*
    2. std::shared_ptr<T>  → Shared ownership
    - Multiple shared_ptrs can co-own the same object.
    - Uses reference counting (control block with strong + weak count).
    - Object is destroyed when strong count reaches zero.
    - Slightly more overhead than unique_ptr (atomic ref-count operations).
*/

void shared_ptr_basics() {
    std::cout << "\n=== std::shared_ptr basics ===\n";

    auto sp1 = std::make_shared<Widget>("SharedOne");  // preferred creation

    {
        auto sp2 = sp1;  // copy → strong count increases
        std::cout << "Inside inner scope: use_count() = " << sp1.use_count() << "\n";

        auto sp3 = std::make_shared<Widget>("AnotherShared");
        sp1 = sp3;  // assignment: old object count decreases, new increases
        std::cout << "After assignment: original sp1.use_count() = " << sp1.use_count() << "\n";
    }  // sp2 goes out of scope → strong count decreases

    std::cout << "After inner scope: use_count() = " << sp1.use_count() << "\n";
    // When main() ends, sp1 is destroyed → count reaches 0 → Widget destroyed
}

/*
    Pitfall: Creating shared_ptr from raw pointer multiple times → separate control blocks → double delete!
    Bad example:
        Widget* raw = new Widget("Danger");
        std::shared_ptr<Widget> s1(raw);
        std::shared_ptr<Widget> s2(raw);  // undefined behavior!

    Always use std::make_shared or pass an existing shared_ptr.
    Trick: If you must use raw pointer, use std::shared_ptr<Widget>(raw) only once,
    then copy that shared_ptr.
*/

/*
    3. std::weak_ptr<T>  → Non-owning observer of a shared_ptr-managed object
    - Does not increase strong reference count.
    - Used to break reference cycles (most common use case).
    - Can be converted to shared_ptr via lock().
    - expired() tells if the object has been destroyed.
*/

/*
    THE PROBLEM THAT std::weak_ptr SOLVES

    std::shared_ptr uses a reference count: the managed object is deleted when the last
    shared_ptr owning it is destroyed (count reaches 0).

    This works perfectly for tree-like or DAG structures, but fails when objects form a
    CYCLE of shared_ptrs pointing to each other.

    Example of the problem (DO NOT DO THIS):
        auto a = std::make_shared<Node>("A");
        auto b = std::make_shared<Node>("B");
        a->next = b;   // shared_ptr
        b->next = a;   // shared_ptr → cycle!

    Now both objects have strong reference count = 2 (one from the local variables a/b,
    one from the other's next pointer). When a and b go out of scope, the count drops to 1
    for each object (still held by the cycle). No count ever reaches 0 → neither object is
    destroyed → MEMORY LEAK!

    This is the classic "reference cycle" problem.

    std::weak_ptr solves it by being a non-owning reference:
    - It does NOT increase the strong reference count.
    - It can still observe the object and obtain a temporary shared_ptr via lock().
    - When all strong references are gone, the object is destroyed, and any weak_ptrs
      become "expired".

    By making one direction of the cycle weak, we break the cycle while still allowing
    safe access to the other object.
*/

struct Node {
    std::string name;
    // std::shared_ptr<Node> next;  // ← would create cycle if bidirectional!
    std::weak_ptr<Node> next;       // breaks cycle
    std::shared_ptr<Node> prev;     // owning direction is fine

    explicit Node(std::string n) : name(std::move(n)) {
        std::cout << "Node " << name << " created\n";
    }
    ~Node() {
        std::cout << "Node " << name << " destroyed\n";
    }
};

void weak_ptr_cycle_prevention() {
    std::cout << "\n=== std::weak_ptr breaking cycles ===\n";

    auto a = std::make_shared<Node>("A");
    auto b = std::make_shared<Node>("B");

    a->next = b;  // weak, no ownership
    b->prev = a;  // strong ownership

    std::cout << "use_count of A: " << a.use_count() << "\n";
    std::cout << "use_count of B: " << b.use_count() << "\n";

    // Check if weak_ptr is still valid
    if (auto locked = a->next.lock()) {
        std::cout << "From A, next node is still alive: " << locked->name << "\n";
    }

    // When a and b go out of scope at the end of this function,
    // strong count reaches zero → both Nodes are properly destroyed.
    // No memory leak, even though they reference each other.
}

void weak_ptr_expiry_demo() {
    std::cout << "\n=== weak_ptr expiry detection ===\n";

    std::weak_ptr<Widget> weak;

    {
        auto shared = std::make_shared<Widget>("Temporary");
        weak = shared;  // weak observes shared

        if (auto locked = weak.lock()) {
            std::cout << "Inside scope: object still alive, name = " << locked->name << "\n";
        }
    }  // shared destroyed here

    if (weak.expired()) {
        std::cout << "Outside scope: weak.expired() == true → object was destroyed\n";
    }

    // Trying to lock an expired weak_ptr returns empty shared_ptr
    if (auto locked = weak.lock()) {
        std::cout << "This line will not print\n";
    } else {
        std::cout << "lock() on expired weak_ptr returns empty shared_ptr\n";
    }
}

/*
    Summary of best practices:
    - Prefer unique_ptr for exclusive ownership (zero overhead).
    - Use shared_ptr only when true shared ownership is needed.
    - Always use std::make_unique / std::make_shared.
    - Use weak_ptr to break cycles in data structures (graphs, trees with parent pointers, observer patterns).
    - Never mix raw pointers and smart pointers carelessly.
*/

int main() {
    std::cout << "Smart Pointers Complete Tutorial & Demo\n";

    unique_ptr_basics();
    unique_ptr_with_containers();
    shared_ptr_basics();
    weak_ptr_cycle_prevention();
    weak_ptr_expiry_demo();

    std::cout << "\nProgram ending — all smart pointers go out of scope → automatic cleanup.\n";
    return 0;
}
