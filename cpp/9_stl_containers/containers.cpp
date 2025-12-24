// ============================================================
// C++ DATA STRUCTURES (STL CONTAINERS) — EXHAUSTIVE GUIDE
// Single-file reference | C++17 | Idiomatic | Pitfalls included
// ============================================================

#include <iostream>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <forward_list>

#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <stack>
#include <queue>
#include <priority_queue>

#include <tuple>
#include <utility>
#include <string>
#include <string_view>

// ------------------------------------------------------------
// GENERAL RULES (IMPORTANT)
// ------------------------------------------------------------
/*
1. Prefer value semantics.
2. Prefer contiguous containers when possible.
3. Avoid premature use of linked structures.
4. Know iterator invalidation rules.
5. Always choose the container based on access pattern.
*/

// ============================================================
// 1. std::array (Fixed-size, stack allocated)
// ============================================================
void array_demo() {
    std::array<int, 3> a{1, 2, 3};

    // Pros:
    // - No heap
    // - constexpr-friendly
    // - STL compatible

    // Pitfall: size fixed at compile-time
    // a.push_back(4); ❌

    for (int x : a)
        std::cout << x << ' ';
}

// ============================================================
// 2. std::vector (Dynamic array, contiguous)
// ============================================================
void vector_demo() {
    std::vector<int> v;

    v.reserve(10); // BEST PRACTICE

    v.push_back(1);
    v.emplace_back(2); // avoids temporary

    // Random access O(1)
    std::cout << v[0];

    // Pitfall: iterator invalidation
    // push_back may invalidate iterators

    // Removal idiom:
    v.erase(std::remove(v.begin(), v.end(), 1), v.end());
}

// ============================================================
// 3. std::deque (Segmented array)
// ============================================================
void deque_demo() {
    std::deque<int> d;

    d.push_front(1);
    d.push_back(2);

    // Pros:
    // - Fast front/back insert
    // - Random access

    // Cons:
    // - Not contiguous → worse cache locality
}

// ============================================================
// 4. std::list (Doubly-linked list)
// ============================================================
void list_demo() {
    std::list<int> l{1, 2, 3};

    auto it = l.begin();
    l.insert(it, 0); // O(1)

    // Pros:
    // - Stable iterators
    // - O(1) splice

    // Cons:
    // - Terrible cache performance
    // - Almost always inferior to vector/deque
}

// ============================================================
// 5. std::forward_list (Singly-linked list)
// ============================================================
void forward_list_demo() {
    std::forward_list<int> fl{1, 2, 3};

    fl.push_front(0);

    // Use only if:
    // - Minimal memory
    // - Forward traversal only
}

// ============================================================
// 6. std::set (Ordered, unique keys)
// ============================================================
void set_demo() {
    std::set<int> s{3, 1, 2};

    s.insert(4);

    // Ordered (RB-tree)
    // O(log n)

    // Pitfall:
    // No random access
}

// ============================================================
// 7. std::map (Ordered key → value)
// ============================================================
void map_demo() {
    std::map<std::string, int> m;

    m["a"] = 1;        // inserts if missing
    m.emplace("b", 2);

    // Pitfall:
    // operator[] inserts default value

    if (m.find("c") == m.end()) {
        // safe lookup
    }
}

// ============================================================
// 8. std::unordered_set (Hash set)
// ============================================================
void unordered_set_demo() {
    std::unordered_set<int> us;

    us.insert(1);

    // Average O(1)
    // Worst-case O(n)

    // Pitfall:
    // Iterator order undefined
    // Rehash invalidates iterators
}

// ============================================================
// 9. std::unordered_map (Hash map)
// ============================================================
void unordered_map_demo() {
    std::unordered_map<int, std::string> um;

    um.emplace(1, "one");

    // Best practice:
    um.reserve(100); // reduce rehashing
}

// ============================================================
// 10. std::stack (Container adaptor)
// ============================================================
void stack_demo() {
    std::stack<int> s;

    s.push(1);
    s.pop();

    // No iteration
    // Uses deque by default
}

// ============================================================
// 11. std::queue (FIFO)
// ============================================================
void queue_demo() {
    std::queue<int> q;

    q.push(1);
    q.pop();
}

// ============================================================
// 12. std::priority_queue (Heap)
// ============================================================
void priority_queue_demo() {
    std::priority_queue<int> pq;

    pq.push(10);
    pq.push(1);

    // Top is max by default

    // Min-heap:
    std::priority_queue<int, std::vector<int>, std::greater<>> minpq;
}

// ============================================================
// 13. std::pair (2 values)
// ============================================================
void pair_demo() {
    std::pair<int, double> p{1, 3.14};

    auto [i, d] = p; // structured binding
}

// ============================================================
// 14. std::tuple (N values, heterogeneous)
// ============================================================
void tuple_demo() {
    std::tuple<int, double, std::string> t{1, 2.5, "hi"};

    auto [a, b, c] = t;

    // Prefer struct if semantic meaning exists
}

// ============================================================
// 15. std::string / std::string_view
// ============================================================
void string_demo() {
    std::string s = "hello";

    std::string_view sv = s; // non-owning

    // Pitfall:
    // sv must not outlive underlying string
}

// ============================================================
// DESIGN GUIDELINES (STRONG OPINIONS)
// ============================================================
/*
- 80% of the time: std::vector
- Need key lookup: unordered_map
- Need order: map / set
- Avoid list unless splice-heavy
- Prefer flat structures (Boost flat_map, abseil::flat_hash_map)
- For real-time: custom allocators
*/

// ============================================================
// OTHER ECOSYSTEM OPTIONS
// ============================================================
/*
- boost::container (flat_map, small_vector)
- abseil::node_hash_map / flat_hash_map
- folly containers
- EASTL (game dev)
*/

// ============================================================
// MAIN
// ============================================================
int main() {
    array_demo();
    vector_demo();
    deque_demo();
    list_demo();
    forward_list_demo();
    set_demo();
    map_demo();
    unordered_set_demo();
    unordered_map_demo();
    stack_demo();
    queue_demo();
    priority_queue_demo();
    pair_demo();
    tuple_demo();
    string_demo();
}
