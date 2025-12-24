// virtual_overriding.cpp
// one interface many behaviors

// Topic: Virtual methods & overriding — correctness, pitfalls, and best practices.
// C++17, single-file, self-contained, pedagogical.
//
// Compile:
//   g++ -std=c++17 -Wall -Wextra -Woverloaded-virtual -pedantic virtual_overriding.cpp
//
// Core idea:
//   Virtual dispatch enables runtime polymorphism.
//   Incorrect overriding silently breaks behavior.
//   `override` is non-optional in modern C++.

#include <iostream>
#include <memory>

// ------------------------------------------------------------
// 1. Baseline: virtual function
// ------------------------------------------------------------
struct Base {
    virtual void speak() const {
        std::cout << "Base::speak\n";
    }

    // Always virtual destructor for polymorphic bases
    virtual ~Base() = default;
};

// ------------------------------------------------------------
// 2. Correct overriding
// ------------------------------------------------------------
struct Derived final : Base {
    void speak() const override {
        std::cout << "Derived::speak\n";
    }
};

// ------------------------------------------------------------
// 3. The silent bug: missing virtual in base
// ------------------------------------------------------------
struct BadBase {
    void run() const {
        std::cout << "BadBase::run\n";
    }
};

struct BadDerived : BadBase {
    void run() const { // NOT virtual, NOT override
        std::cout << "BadDerived::run\n";
    }
};

// ------------------------------------------------------------
// 4. The WORST bug: signature mismatch
// ------------------------------------------------------------
struct Animal {
    virtual void move(int speed) const {
        std::cout << "Animal::move " << speed << "\n";
    }
};

struct Dog : Animal {
    void move(double speed) const { // ❌ does NOT override
        std::cout << "Dog::move " << speed << "\n";
    }
};

// ------------------------------------------------------------
// 5. override keyword saves you
// ------------------------------------------------------------
struct SafeDog : Animal {
    // void move(double) const override; // ❌ compile error (GOOD)
    void move(int speed) const override {
        std::cout << "SafeDog::move " << speed << "\n";
    }
};

// ------------------------------------------------------------
// 6. final prevents further overriding
// ------------------------------------------------------------
struct Locked {
    virtual void f() const final {
        std::cout << "Locked::f\n";
    }
};

// struct Breaker : Locked {
//     void f() const override; // ❌ compile error
// };

// ------------------------------------------------------------
// 7. Covariant return types
// ------------------------------------------------------------
struct Shape {
    virtual Shape* clone() const {
        return new Shape(*this);
    }
    virtual ~Shape() = default;
};

struct Circle : Shape {
    Circle* clone() const override {
        return new Circle(*this);
    }
};

// ------------------------------------------------------------
// 8. Virtual dispatch through base references
// ------------------------------------------------------------
void speak_ref(const Base& b) {
    b.speak(); // runtime dispatch
}

void speak_value(Base b) {
    b.speak(); // object slicing
}

// ------------------------------------------------------------
// 9. NVI (Non-Virtual Interface) pattern
// ------------------------------------------------------------
class Processor {
public:
    void process() const {        // stable public API
        pre();
        do_process();
        post();
    }

    virtual ~Processor() = default;

protected:
    virtual void do_process() const = 0;

private:
    void pre() const  { std::cout << "pre\n"; }
    void post() const { std::cout << "post\n"; }
};

class CustomProcessor : public Processor {
protected:
    void do_process() const override {
        std::cout << "custom logic\n";
    }
};

// ------------------------------------------------------------
// 10. Performance note: virtual vs non-virtual
// ------------------------------------------------------------
struct Fast {
    void f() const { /* inlinable */ }
};

struct Polymorphic {
    virtual void f() const { /* not inlined */ }
};

// ------------------------------------------------------------
// main(): demonstrations
// ------------------------------------------------------------
int main() {
    // --- Correct polymorphism ---
    Derived d;
    speak_ref(d);    // Derived::speak
    speak_value(d);  // Base::speak (slicing!)

    // --- Missing virtual ---
    BadDerived bd;
    BadBase& bb = bd;
    bb.run();        // BadBase::run (BUG)

    // --- Signature mismatch ---
    Dog dog;
    Animal& a = dog;
    a.move(10);      // Animal::move (BUG)

    // --- Safe overriding ---
    SafeDog sd;
    Animal& a2 = sd;
    a2.move(20);     // SafeDog::move

    // --- Covariant return ---
    std::unique_ptr<Shape> s(new Circle{});
    std::unique_ptr<Shape> c(s->clone());
    std::cout << "Cloned shape\n";

    // --- NVI pattern ---
    CustomProcessor p;
    p.process();

    return 0;
}

/*
============================================================
RULES OF THUMB (STRONG OPINIONS)
============================================================

1. If a class has ANY virtual function → destructor MUST be virtual.
2. Every override MUST use `override`.
3. Never rely on “same name = override”.
4. Pass polymorphic objects by reference or pointer ONLY.
5. Use `final` aggressively to lock APIs and enable devirtualization.
6. Prefer NVI pattern for stable public interfaces.
7. Avoid virtuals in hot inner loops.

============================================================
COMMON PITFALLS
============================================================

- Object slicing
- Signature mismatches
- Missing virtual destructors
- Accidental non-polymorphic bases
- Performance assumptions

============================================================
WHEN NOT TO USE VIRTUAL
============================================================

- Value typ
*/
