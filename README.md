# Python-vs-Cpp-vs-Rust

A repo I use as revision basis of c++ and python.

target versions:
+  c++17
+  python3.13

advanced features supassing defaults versions will be in dedicated folders.

# C++ Roadmap

## **1. Language Fundamentals**

* [x] Basic “Hello World”
* [x] Variables of all primitive types (`int`, `long long`, `float`, `double`, `char`, `bool`)
* [x]  `auto` type deduction examples (simple, ambiguous cases)
* [x]  `constexpr` variable and `constexpr` function
* [x]  Input/output with `std::cin` and `std::cout`
* [x]  Enum, enum class, strong typing

---

## **2. Control Flow**

* [X] `if / else` examples covering nested + chained
* [X] `switch` with enum + default branch
* [X] All loop types: `for`, `while`, `do while`
* [X] Range-based for loop over arrays, vectors, initializer_list
* [X] Early return, `continue`, `break`

---

## **3. Functions**

* [x] Basic function returning value
* [x] Pass-by-value / reference / pointer versions
* [x] `inline` function demo
* [x] Function overloading (matching rules)
* [x] Default arguments
* [x] Variadic template function (`template<typename... Args>`)
* [x] Lambda expressions: captures (by value, by ref, mixed), mutable lambda

---

## **4. Error Handling**

* [ ] Basic `try/catch`
* [ ] Throwing custom exceptions
* [ ] Exception safety example (RAII rollback on failure)
* [ ] `noexcept` function with and without violation

---

## **5. Essential Memory & Pointers**

* [X] Pointer basics: address printing, dereferencing, pointer arithmetic
* [x] Double pointer (`T**`) example
* [ ] `new` / `delete`
* [ ] `new[]` / `delete[]`
* [ ] Dangling pointer example (controlled)
* [X] Smart pointers:
  * `std::unique_ptr` (create, release, reset)
  * `std::shared_ptr` (use_count)
  * `std::weak_ptr` (lock and expiry detection)
* [x] RAII wrapper class you write yourself

---

## **6. Object-Oriented Programming**

* [x] Class with constructor, destructor
* [x] Overloaded constructors
* [x] `explicit` constructor and why it matters
* [x] Copy constructor + copy assignment operator
* [x] Move constructor + move assignment operator
* [x] Virtual methods + overriding
* [x] Abstract class + pure virtual function
* [x] Inheritance example (Vehicle → Car)
* [x] `final` class and `override` keyword
* [ ] Rule of 3, 5, 0 examples
* [x] Static methods and static members
* [x] Operator overloading (`<<`, `==`, `<`, `[]`, `()`, arithmetic operators)

---

## **7. Modern C++17 Features**

* [ ] Structured bindings (auto [a, b] = …)
* [ ] `std::optional`
* [ ] `std::variant` + `std::visit`
* [ ] `std::any`
* [ ] `if constexpr` inside template
* [ ] Inline variables
* [ ] Fold expressions

---

## **8. Templates (Mastery Required)**

* [ ] Function templates
* [ ] Class templates
* [ ] Template specialization (full + partial)
* [ ] SFINAE example (`std::enable_if`)
* [ ] Trait class: write your own `is_integer<T>`
* [ ] CRTP (Curiously Recurring Template Pattern)
* [ ] Non-type template parameters (`template<int N>`)
* [ ] Compile-time array operations using templates

---

## **9. STL Containers**

Create full examples for:

* [x] `std::vector` (push, emplace, erase, reserve)
* [x] `std::deque`
* [x] `std::stack` / `std::queue`
* [ ] `std::map` / `std::unordered_map`
* [ ] `std::pair`
* [ ] `std::tuple`
* [ ] `std::list`
* [ ] `std::forward_list`
* [ ] `std::set` / `std::unordered_set`
* [ ] `std::priority_queue`
* [ ] `std::array`

For each container:
    * iteration (classic, range-for, iterator)
    * insertion, deletion, search
    * custom comparator

---

## **10. Algorithms (Master the `<algorithm>` Library)**

Write examples for:

* [ ] `std::sort`, custom comparator
* [ ] `std::stable_sort`
* [ ] `std::find`, `std::find_if`
* [ ] `std::all_of`, `std::any_of`, `std::none_of`
* [ ] `std::transform`
* [ ] `std::accumulate`
* [ ] `std::copy`, `std::copy_if`
* [ ] `std::for_each`
* [ ] `std::remove_if` + erase idiom
* [ ] `std::binary_search`

---

## **11. File & OS Interaction**

*  [ ] Read text file line-by-line
*  [ ] Write file
*  [ ] Append file
*  [ ] Binary file read/write (struct serialization)
*  [ ] Implement a simple config parser
*  [ ] Read command line arguments (`int argc, char* argv[]`)
*  [ ] Parse environment variables (`std::getenv`)

---

## **12. Concurrency & Multithreading**

* Launching threads (`std::thread`)
* Lambda inside thread
* Join vs detach
* `std::mutex` + `std::lock_guard`
* `std::unique_lock` + condition_variable
* Producer–consumer queue
* Thread-safe counter
* `std::async` task
* `std::promise` / `std::future`
* Atomic operations with `std::atomic<int>`

---

## **13. Advanced Memory / Low-Level**

* [ ] Manual memory pool allocator
* [ ] Custom allocator for `std::vector`
* [ ] Placement new + explicit destructor
* [ ] Writing a small arena allocator
* [ ] Alignment: `std::aligned_storage`, `alignas`, `alignof`
* [ ] Memory layout exploration with `sizeof` and offsets

---

## **14. Compile-Time Programming**

* [ ] constexpr math (compile-time factorial, Fibonacci)
* [ ] constexpr array operations
* [ ] Type traits: create custom ones + use `<type_traits>`
* [ ] Tag dispatching example
* [ ] Metaprogramming recursion (template recursion)

---

## **15. Design Patterns (C++ implementations)**

* [ ] Singleton (thread-safe C++17 version)
* [ ] Factory Method
* [ ] Abstract Factory
* [ ] Strategy Pattern
* [ ] Observer Pattern
* [ ] Decorator
* [ ] Builder
* [ ] RAII pattern wrapped around a system resource (FD, file handle, mutex)

---

## **16. Networking / Systems**

* [ ] Basic socket client (POSIX)
* [ ] Basic socket server
* [ ] Write a tiny HTTP GET client
* [ ] Parse a JSON manually (no libs)
* [ ] Use `<filesystem>`:
  * Walk directories
  * Query file size
  * Create/remove directories

---

## **17. Testing & Tooling**

* [ ] Minimal CMake project
* [ ] Header vs source file separation
* [ ] Unit testing with Catch2 or GoogleTest
* [ ] Static analysis demonstration (clang-tidy)
* [ ] Benchmarks using `<chrono>` high_resolution_clock

## ** 18. General software patterns **

### Creational Patterns
- [ ] Factory Method
- [ ] Abstract Factory
- [ ] Builder
- [ ] Singleton
- [ ] Prototype

### Structural Patterns
- [ ] Adapter
- [ ] Bridge
- [ ] Composite
- [ ] Decorator
- [ ] Facade
- [ ] Flyweight
- [ ] Proxy

### Behavioral Patterns
-  [ ] Strategy
-  [ ] Observer
-  [ ] Command
-  [ ] State
-  [ ] Visitor
-  [ ] Mediator
-  [ ] Memento
-  [ ] Chain of Responsibility

### C++-Specific Idioms
-  [ ] RAII (Resource Acquisition Is Initialization)
-  [ ] Pimpl (Pointer to Implementation)
-  [ ] Curiously Recurring Template Pattern (CRTP)
-  [ ] Type Erasure
-  [ ] Policy-based Design
-  [ ] Expression Templates
-  [ ] Self-registering Factory

# Python roadmap
# Rust roadmap
