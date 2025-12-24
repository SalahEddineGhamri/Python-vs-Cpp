// explicit_constructor.cpp
// Topic: `explicit` constructors — what they prevent, why they matter, and best practices.
// C++17, single-file, self-contained, pedagogical.
//
// Compile:
//   g++ -std=c++17 -Wall -Wextra -Wconversion -pedantic explicit_constructor.cpp
//
// Key idea:
//   `explicit` prevents *implicit conversions* via constructors.
//   This avoids silent bugs, overload ambiguities, and unintended temporaries.

#include <iostream>
#include <string>
#include <vector>

// ------------------------------------------------------------
// 1. The classic pitfall: implicit conversion
// ------------------------------------------------------------
struct ImplicitInt {
    int value;
    ImplicitInt(int v) : value(v) {} // NOT explicit
};

void takesImplicit(ImplicitInt x) {
    std::cout << "takesImplicit: " << x.value << "\n";
}

// ------------------------------------------------------------
// 2. Correct version: explicit constructor
// ------------------------------------------------------------
struct ExplicitInt {
    int value;
    explicit ExplicitInt(int v) : value(v) {}
};

void takesExplicit(ExplicitInt x) {
    std::cout << "takesExplicit: " << x.value << "\n";
}

// ------------------------------------------------------------
// 3. Subtle bug: overload resolution chaos
// ------------------------------------------------------------
struct FileSize {
    std::size_t bytes;
    explicit FileSize(std::size_t b) : bytes(b) {}
};

void log(FileSize) {
    std::cout << "log(FileSize)\n";
}

void log(int) {
    std::cout << "log(int)\n";
}

// ------------------------------------------------------------
// 4. Single-argument constructors are NOT the only case
//    `explicit` also matters for multi-argument constructors
//    with default values.
// ------------------------------------------------------------
struct Path {
    std::string value;
    explicit Path(std::string p = ".") : value(std::move(p)) {}
};

// ------------------------------------------------------------
// 5. STL + containers: where implicit hurts badly
// ------------------------------------------------------------
struct UserId {
    int id;
    explicit UserId(int v) : id(v) {}
};

// ------------------------------------------------------------
// 6. When implicit conversion IS acceptable (rare)
// ------------------------------------------------------------
struct Degrees {
    double v;
    explicit Degrees(double d) : v(d) {}
};

struct Radians {
    double v;
    Radians(Degrees d) : v(d.v * 3.141592653589793 / 180.0) {}
};

// ------------------------------------------------------------
// 7. Idiomatic C++17 patterns
// ------------------------------------------------------------
class Resource {
public:
    explicit Resource(int fd) noexcept : fd_(fd) {}

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    Resource(Resource&&) noexcept = default;
    Resource& operator=(Resource&&) noexcept = default;

private:
    int fd_;
};

// ------------------------------------------------------------
// main(): demonstrations
// ------------------------------------------------------------
int main() {
    // --- Implicit constructor allows silent conversions ---
    takesImplicit(42);        // OK (implicit temporary created)
    ImplicitInt a = 10;       // OK (copy-initialization)

    // --- Explicit constructor blocks them ---
    // takesExplicit(42);     // ❌ compile error (GOOD)
    takesExplicit(ExplicitInt{42}); // ✅ explicit intent

    // Explicit blocks copy-initialization:
    // ExplicitInt b = 5;     // ❌
    ExplicitInt b{5};         // ✅

    // --- Overload safety ---
    // log(10);               // would call log(int)
    log(FileSize{10});        // unambiguous

    // --- Default args + explicit ---
    // Path p = "home";       // ❌
    Path p{"home"};           // ✅
    std::cout << "Path: " << p.value << "\n";

    // --- Containers ---
    std::vector<UserId> users;
    // users.push_back(1);    // ❌ prevents accidental API misuse
    users.emplace_back(1);   // ✅ explicit construction

    // --- Controlled conversion example ---
    Degrees d{180};
    Radians r{d};             // explicit step still visible
    std::cout << "Radians: " << r.v << "\n";

    return 0;
}

/*
============================================================
RULES OF THUMB (STRONG OPINIONS)
============================================================

1. Make ALL single-argument constructors `explicit` by default.
2. Also mark constructors with default parameters `explicit`.
3. Allow implicit conversion ONLY for:
   - mathematical value types
   - cheap, lossless, obvious conversions
4. Prefer:
     T x{arg};
   over:
     T x = arg;
5. `explicit` is API design, not syntax sugar.
6. Public constructors without `explicit` are a liability.

============================================================
COMMON PITFALLS PREVENTED
============================================================

- Accidental temporary objects
- Wrong overload selection
- Silent performance costs
- Ambiguous APIs
- Container misuse
- Boolean-like bugs (e.g., passing int to strong types)

============================================================
RELATED / ADVANCED
============================================================

- C++20: `explicit(bool)` for conditional explicitness
- Strong typedefs (Boost, Abseil)
- `std::chrono` as a gold-standard example
- Named constructors as an alternative
- User-defined literals for clarity

*/
