// std_queue_tutorial.cpp
// A complete, self-contained tutorial and demonstration of std::queue in C++17
// Covers: construction, push/emplace, front/back, pop, size/empty,
//         usage with different underlying containers (deque, list),
//         common patterns, pitfalls, and constraints.
//
// Compile with: g++ -std=c++17 -Wall -Wextra -O2 std_queue_tutorial.cpp -o std_queue_tutorial
//
// This file builds cleanly and runs on any C++17-compliant compiler.

#include <iostream>
#include <queue>
#include <deque>
#include <list>
#include <string>

/*
    std::queue is a container adaptor that provides a FIFO (First-In-First-Out) interface.
    Elements are inserted at the back and removed from the front.

    By default, it uses std::deque as the underlying container.
    You can use any container that supports:
        - push_back
        - pop_front (not available for std::vector)
        - front
        - back
        - empty
        - size (optional)

    Typical alternatives: std::deque (default), std::list.
    std::vector cannot be used because it lacks efficient pop_front().

    Interesting fact: Both push and pop are amortized O(1) with the default deque.

    Key operations:
        - push / emplace   →  add to back
        - front            →  access first element
        - back             →  access last element
        - pop              →  remove first element (returns void)

    Pitfall: pop() removes the front element but does NOT return it.
    Always call front() first if you need the value.
*/

struct Message {
    std::string sender;
    std::string text;

    Message(std::string s, std::string t) : sender(std::move(s)), text(std::move(t)) {}

    friend std::ostream& operator<<(std::ostream& os, const Message& m) {
        os << m.sender << ": " << m.text;
        return os;
    }
};

int main() {
    /*
        1. Basic queue with default underlying container (std::deque<int>)
    */
    std::queue<int> q;
    std::cout << "Empty queue: empty=" << std::boolalpha << q.empty()
              << ", size=" << q.size() << '\n';

    /*
        2. Adding elements: push vs emplace
    */
    q.push(10);                    // copies/moves existing int
    q.push(20);
    q.emplace(30);                 // constructs int in-place

    std::cout << "\nAfter pushing 10, 20, 30:\n";
    std::cout << "Front: " << q.front() << '\n';
    std::cout << "Back:  " << q.back()  << '\n';
    std::cout << "Size:  " << q.size()   << '\n';

    /*
        3. Processing elements (FIFO order)
    */
    std::cout << "\nProcessing queue:\n";
    while (!q.empty()) {
        std::cout << "Front: " << q.front() << " -> popping\n";
        q.pop();                   // removes front element, returns void
    }

    /*
        4. Real-world example: message queue
    */
    std::queue<Message> messages;
    messages.emplace("Alice", "Hello!");
    messages.emplace("Bob",   "How are you?");
    messages.emplace("Charlie", "Meeting at 3pm");

    std::cout << "\nProcessing incoming messages (FIFO):\n";
    while (!messages.empty()) {
        std::cout << messages.front() << '\n';
        messages.pop();
    }

    /*
        5. Changing the underlying container
           Example: use std::list instead of default deque
           Useful when you need splice operations or very frequent front removals
    */
    std::queue<int, std::list<int>> list_queue;
    list_queue.push(100);
    list_queue.push(200);
    list_queue.emplace(300);

    std::cout << "\nQueue using std::list as underlying container:\n";
    while (!list_queue.empty()) {
        std::cout << list_queue.front() << ' ';
        list_queue.pop();
    }
    std::cout << '\n';

    /*
        6. Common pattern: producer-consumer simulation
    */
    std::queue<std::string> tasks;
    // Producer adds tasks
    tasks.push("Compile code");
    tasks.emplace("Run tests");
    tasks.push("Deploy");

    // Consumer processes tasks
    std::cout << "\nTask processing order:\n";
    while (!tasks.empty()) {
        std::cout << "Processing: " << tasks.front() << '\n';
        tasks.pop();
    }

    /*
        7. Important pitfalls and best practices

        - pop() does NOT return the removed element.
          Wrong: int x = q.pop();                // compile error
          Correct: int x = q.front(); q.pop();

        - No iterators, no indexing, no begin()/end().
          You cannot access middle elements or iterate arbitrarily.

        - back() returns a reference to the last element (the one most recently pushed).
          Useful for checking what is waiting at the end.

        - Prefer emplace() over push() for complex types to avoid temporary objects.

        - You can compare two queues with ==, !=, <, >, <=, >=
          (lexicographical comparison of underlying sequences).

        - std::queue is NOT thread-safe by default.
          Use mutexes or concurrent queues (e.g., from TBB or Boost) in multithreaded code.

        Trick: If you need both queue and priority access, consider std::priority_queue.
        If you need random access or frequent middle operations, use the underlying container directly (e.g., std::deque).
    */

    // Demonstrate comparison
    std::queue<int> q1, q2;
    q1.push(1); q1.push(2);
    q2.push(1); q2.push(2);
    std::cout << "\nQueue comparison: q1 == q2 ? " << std::boolalpha << (q1 == q2) << '\n';

    return 0;
}
