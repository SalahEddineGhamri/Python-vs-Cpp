// std_deque_tutorial.cpp
// A complete, self-contained tutorial and demonstration of std::deque in C++17
// Covers: construction, push_front/push_back, emplace_front/emplace_back,
//         pop_front/pop_back, reserve-like behavior (no direct reserve),
//         insertion/deletion at both ends and in the middle,
//         iteration (index, range-for, iterator), search, and custom comparator (sorting).
//
// Compile with: g++ -std=c++17 -Wall -Wextra -O2 std_deque_tutorial.cpp -o std_deque_tutorial
//
// This file builds cleanly and runs on any C++17-compliant compiler.

#include <iostream>
#include <deque>
#include <algorithm>    // std::sort, std::find, std::remove
#include <string>

/*
    std::deque (double-ended queue) is a sequence container that allows efficient
    insertion and deletion at BOTH the beginning and the end (amortized O(1)).

    Key differences from std::vector:
    - No capacity() or reserve() — internally uses chunks of fixed-size blocks.
    - push_front / pop_front are fast.
    - Random access is still O(1), but slightly slower than vector due to indirection.
    - Iterators remain valid after insertions/deletions at ends (but invalidated in middle).

    Interesting fact: deque is the default underlying container for std::queue and std::stack
    when you don't specify another one.

    Pitfall: Insertion or deletion in the middle is O(n) and invalidates iterators/references
    from the modification point onward — just like vector.
*/

struct Task {
    std::string description;
    int priority;  // higher number = more urgent

    Task(std::string d, int p) : description(std::move(d)), priority(p) {}

    friend std::ostream& operator<<(std::ostream& os, const Task& t) {
        os << "[" << t.priority << "] " << t.description;
        return os;
    }
};

// Custom comparator: sort tasks by priority descending
struct HigherPriorityFirst {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority > b.priority;
    }
};

int main() {
    /*
        1. Basic construction
    */
    std::deque<int> dq;
    std::cout << "Empty deque: size=" << dq.size() << '\n';

    /*
        2. Adding elements at both ends
           push_back / push_front  — copy/move existing object
           emplace_back / emplace_front — construct in-place (more efficient)
    */
    dq.push_back(10);
    dq.push_back(20);
    dq.push_front(5);
    dq.emplace_front(1);      // constructs int directly at front
    dq.emplace_back(30);

    std::cout << "\nAfter pushes:\n";
    for (const auto& v : dq) std::cout << v << ' ';
    std::cout << '\n';

    /*
        3. Removing elements from both ends
    */
    dq.pop_front();           // removes 1
    dq.pop_back();            // removes 30

    std::cout << "\nAfter pop_front and pop_back:\n";
    for (const auto& v : dq) std::cout << v << ' ';
    std::cout << '\n';

    /*
        4. Iteration methods — identical to vector
    */
    std::cout << "\nIndex-based access:\n";
    for (std::size_t i = 0; i < dq.size(); ++i) {
        std::cout << "dq[" << i << "] = " << dq[i] << '\n';
    }

    std::cout << "\nRange-based for (recommended):\n";
    for (const auto& v : dq) std::cout << v << ' ';
    std::cout << '\n';

    std::cout << "\nIterator loop:\n";
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    /*
        5. Insertion in the middle — insert() is O(n)
           Returns iterator to first inserted element
    */
    auto middle = dq.begin() + dq.size() / 2;
    dq.insert(middle, 999);   // insert 999 in the approximate middle

    std::cout << "\nAfter inserting 999 in the middle:\n";
    for (const auto& v : dq) std::cout << v << ' ';
    std::cout << '\n';

    /*
        6. Deletion — erase() single element or range
           Trick: Use the classic remove-erase idiom for removing by value
    */
    // Remove all occurrences of 10
    auto new_end = std::remove(dq.begin(), dq.end(), 10);
    dq.erase(new_end, dq.end());

    std::cout << "\nAfter removing all 10s:\n";
    for (const auto& v : dq) std::cout << v << ' ';
    std::cout << '\n';

    // Erase a single element by iterator
    if (!dq.empty()) {
        dq.erase(dq.begin() + 1);  // remove second element
    }

    /*
        7. Search — linear O(n) with std::find
           For sorted data you can use binary search algorithms
    */
    if (std::find(dq.begin(), dq.end(), 999) != dq.end()) {
        std::cout << "\n999 is still present.\n";
    }

    /*
        8. Real-world example: priority task queue using deque
    */
    std::deque<Task> tasks;
    tasks.emplace_back("Write report", 5);
    tasks.emplace_front("Emergency bug fix", 10);  // high priority → front
    tasks.emplace_back("Refactor code", 2);
    tasks.emplace_front("Answer urgent email", 8);

    std::cout << "\nTasks in arrival order:\n";
    for (const auto& t : tasks) std::cout << t << '\n';

    // Sort by priority (highest first) — O(n log n)
    std::sort(tasks.begin(), tasks.end(), HigherPriorityFirst());

    std::cout << "\nTasks sorted by priority descending:\n";
    for (const auto& t : tasks) std::cout << t << '\n';

    /*
        9. Important pitfalls and best practices

        - No reserve() or capacity() — you cannot pre-allocate like vector.
          Deque grows automatically in chunks; growth is efficient but not controllable.
        - Iterators are invalidated only by insertions/deletions that affect elements
          at or before the iterator position when operating in the middle.
        - push_front/pop_front are fast → perfect for queues, stacks, or sliding windows.
        - Random access (dq[i]) is slightly slower than vector due to chunk indirection.
        - shrink_to_fit() exists and requests deallocation of unused memory (non-binding).
        - Prefer emplace_front/emplace_back over push_front/push_back when constructing objects.
    */

    std::cout << "\nFinal deque size=" << dq.size() << '\n';

    // Optional: ask the implementation to release unused memory
    dq.shrink_to_fit();

    return 0;
}
