#include <iostream>
#include <optional>
#include <map>
#include <memory>
#include <mutex>

int* find(int x) {
    static int val = 42;
    return x == 1 ? &val : nullptr; // if else
}

int compute() { return 5; }
int foo() { return 7; }

std::optional<int> maybe(bool b) { return b ? std::optional<int>(9) : std::nullopt; }

struct Base { virtual ~Base() = default; };
struct Derived : Base { int v = 123; };

int main() {
    int x = 1;

    // 1. if-init with pointer
    if (auto p = find(x); p != nullptr) {
        std::cout << "found pointer: " << *p << "\n";
    } else {
        std::cout << "missing\n";
    }

    // 2. if-init with value
    if (auto v = compute(); v > 0) {
        std::cout << "compute: " << v << "\n";
    }

    // 3. classic C-style init
    if (int r = foo()) {
        std::cout << "foo: " << r << "\n";
    }

    // 4. optional
    if (auto opt = maybe(true); opt) {
        std::cout << "optional: " << *opt << "\n";
    }

    // 5. map lookup pattern
    std::map<int, int> m{{1,10}, {2,20}};
    if (auto it = m.find(2); it != m.end()) {
        std::cout << "map: " << it->second << "\n";
    }

    // 6. dynamic_cast
    std::unique_ptr<Base> b = std::make_unique<Derived>();
    if (auto* d = dynamic_cast<Derived*>(b.get())) {
        std::cout << "dyn cast: " << d->v << "\n";
    }

    // 7. scoped lock inside if-init
    std::mutex mu;
    if (auto lock = std::unique_lock(mu); lock.owns_lock()) {
        std::cout << "lock acquired\n";
    } // lock released

    return 0;
}
