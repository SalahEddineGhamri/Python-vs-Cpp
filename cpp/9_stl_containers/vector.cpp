// std_vector_tutorial.cpp
// A complete, self-contained tutorial and demonstration of std::vector in C++17
// Covers: construction, push_back/emplace_back, reserve, insertion/deletion/search,
//         iteration (index, range-based for, iterators), erase, and custom comparator (sorting).
//
// Compile with: g++ -std=c++17 -Wall -Wextra -O2 std_vector_tutorial.cpp -o std_vector_tutorial
//
// This single file is designed to build and run without errors on any compliant C++17 compiler.

#include <iostream>
#include <vector>
#include <algorithm>    // for std::sort, std::find
#include <string>

/*
    std::vector is a dynamic array that grows automatically when elements are added.
    It provides fast random access (O(1)), fast push/pop at the end (amortized O(1)),
    but insertion/deletion in the middle is O(n) because elements must be shifted.

    Key advantages over raw arrays:
    - Knows its own size (size(), capacity())
    - Automatic memory management
    - Bounds checking with .at() (throws if out of range)

    Interesting fact: push_back may invalidate all iterators/references when reallocation happens.
    That's why reserve() is extremely useful for performance.
*/

struct Person {
    std::string name;
    int age;

    // Simple constructor for emplace_back demonstration
    Person(std::string n, int a) : name(std::move(n)), age(a) {}

    // Needed for printing
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.name << " (" << p.age << ")";
        return os;
    }
};

// Custom comparator for sorting Persons by age descending
struct PersonAgeDescending {
    bool operator()(const Person& a, const Person& b) const {
        return a.age > b.age;  // > for descending order
    }
};

int main() {
    /*
        1. Basic construction and capacity concepts
    */
    std::vector<int> vec;                    // empty vector, size=0, capacity=0
    std::cout << "Initial: size=" << vec.size()
              << ", capacity=" << vec.capacity() << '\n';

    /*
        2. reserve() - pre-allocate memory to avoid multiple reallocations
           Trick: Always reserve when you know the final size in advance!
    */
    vec.reserve(100);                        // capacity >= 100, no reallocation until >100
    std::cout << "After reserve(100): capacity=" << vec.capacity() << '\n';

    /*
        3. Adding elements: push_back vs emplace_back
           push_back copies/moves an existing object
           emplace_back constructs the object in-place -> often more efficient
    */
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i * 10);               // copies/moves an int
    }

    // emplace_back for complex types avoids temporary object creation
    std::vector<Person> people;
    people.emplace_back("Alice", 30);         // constructs Person directly in vector
    people.emplace_back("Bob", 25);
    people.push_back(Person("Charlie", 35)); // creates temporary, then moves

    /*
        4. Iteration methods
    */
    std::cout << "\nVector contents (index-based loop):\n";
    for (std::size_t i = 0; i < vec.size(); ++i) {
        std::cout << "vec[" << i << "] = " << vec[i] << '\n';
    }

    std::cout << "\nRange-based for loop (preferred in modern C++):\n";
    for (const auto& value : vec) {          // const & avoids copying
        std::cout << value << ' ';
    }
    std::cout << '\n';

    std::cout << "\nIterator loop (most flexible):\n";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    /*
        5. Insertion in the middle - insert()
           Returns iterator to the newly inserted element
           Pitfall: O(n) time because elements after insertion point are shifted
    */
    auto pos = std::find(vec.begin(), vec.end(), 50); // find position of 50
    if (pos != vec.end()) {
        vec.insert(pos, 999);                // insert 999 before 50
        std::cout << "\nAfter inserting 999 before 50:\n";
        for (const auto& v : vec) std::cout << v << ' ';
        std::cout << '\n';
    }

    /*
        6. Deletion - erase()
           Single element or range
           Pitfall: erase invalidates iterators pointing to erased elements
                    and all iterators after the erased range
           Trick: erase returns iterator to the element following the erased one
    */
    // Remove all elements == 30
    auto to_erase = std::remove(vec.begin(), vec.end(), 30); // moves unwanted to end
    vec.erase(to_erase, vec.end());                          // actually removes them
    std::cout << "\nAfter removing all 30s (using remove-erase idiom):\n";
    for (const auto& v : vec) std::cout << v << ' ';
    std::cout << '\n';

    // Erase by position (single element)
    if (!vec.empty()) {
        vec.erase(vec.begin());              // remove first element
    }

    /*
        7. Search
           Linear search: std::find (O(n))
           For sorted vectors: std::binary_search, std::lower_bound (O(log n))
    */
    if (std::find(vec.begin(), vec.end(), 999) != vec.end()) {
        std::cout << "999 is still in the vector.\n";
    }

    /*
        8. Sorting with custom comparator
           std::vector can be sorted with std::sort using any comparator
    */
    std::cout << "\nPeople before sorting:\n";
    for (const auto& p : people) std::cout << p << '\n';

    std::sort(people.begin(), people.end(), PersonAgeDescending());

    std::cout << "\nPeople after sorting by age descending:\n";
    for (const auto& p : people) std::cout << p << '\n';

    /*
        9. Common pitfalls and best practices summary

        - Don't use iterators/references after operations that may reallocate
          (push_back, insert, reserve if capacity changes, etc.)
        - Prefer emplace_back over push_back when constructing objects in-place
        - Use reserve() when approximate size is known -> huge performance win
        - erase() in a loop: use the returned iterator or the remove-erase idiom
        - vec[i] is unchecked; vec.at(i) throws std::out_of_range if invalid
        - Shrinking: vec.shrink_to_fit() requests to reduce capacity (non-binding)
    */

    std::cout << "\nFinal vector size=" << vec.size()
              << ", capacity=" << vec.capacity() << '\n';

    // Request to free unused memory (optional, not guaranteed)
    vec.shrink_to_fit();

    return 0;
}
