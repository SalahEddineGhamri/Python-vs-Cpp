// abstract_class.cpp
// Topic: Abstract classes & pure virtual functions — contracts, design intent, and pitfalls.
// C++17, single-file, self-contained, pedagogical.
//
// Compile:
//   g++ -std=c++17 -Wall -Wextra -Woverloaded-virtual -pedantic abstract_class.cpp
//
// Core idea:
//   Abstract classes define *interfaces + partial behavior*.
//   Pure virtual functions express *mandatory overrides*.
//   They enforce contracts at compile time.

#include <iostream>
#include <memory>

// ------------------------------------------------------------
// 1. Pure virtual function => abstract class
// ------------------------------------------------------------
struct Interface {
    virtual void run() = 0;        // pure virtual
    virtual ~Interface() = default;
};

// Interface i; // ❌ cannot instantiate abstract class

// ------------------------------------------------------------
// 2. Concrete implementation
// ------------------------------------------------------------
struct Implementation : Interface {
    void run() override {
        std::cout << "Implementation::run\n";
    }
};

// ------------------------------------------------------------
// 3. Abstract class with shared behavior
// ------------------------------------------------------------
class Device {
public:
    void start() {                 // non-virtual public API
        init();
        perform();
        shutdown();
    }

    virtual ~Device() = default;

protected:
    virtual void perform() = 0;    // required specialization

private:
    void init()     { std::cout << "init\n"; }
    void shutdown() { std::cout << "shutdown\n"; }
};

class Printer : public Device {
protected:
    void perform() override {
        std::cout << "printing\n";
    }
};

// ------------------------------------------------------------
// 4. Multiple pure virtual functions
// ------------------------------------------------------------
struct Shape {
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual ~Shape() = default;
};

// ------------------------------------------------------------
// 5. Partial override keeps class abstract
// ------------------------------------------------------------
struct Rectangle : Shape {
    double area() const override { return 1.0; }
    // double perimeter() const override; // ❌ missing
};

// ------------------------------------------------------------
// 6. Final override completes abstraction
// ------------------------------------------------------------
struct Square final : Shape {
    double area() const override { return 1.0; }
    double perimeter() const override { return 4.0; }
};

// ------------------------------------------------------------
// 7. Abstract destructor (subtle but important)
// ------------------------------------------------------------
struct Resource {
    virtual ~Resource() = 0;       // still abstract
};

Resource::~Resource() = default;   // definition required

struct File : Resource {
    ~File() { std::cout << "File closed\n"; }
};

// ------------------------------------------------------------
// 8. Polymorphic usage
// ------------------------------------------------------------
void process(const Shape& s) {
    std::cout << "Area: " << s.area() << "\n";
}

// ------------------------------------------------------------
// 9. Interface vs implementation inheritance
// ------------------------------------------------------------
struct Flyable {
    virtual void fly() = 0;
    virtual ~Flyable() = default;
};

struct Swimmable {
    virtual void swim() = 0;
    virtual ~Swimmable() = default;
};

struct Duck : Flyable, Swimmable {
    void fly() override  { std::cout << "fly\n"; }
    void swim() override { std::cout << "swim\n"; }
};

// ------------------------------------------------------------
// 10. Anti-pattern: data in abstract base
// ------------------------------------------------------------
struct BadInterface {
    int state;                     // ❌ layout constraint
    virtual void f() = 0;
};

// ------------------------------------------------------------
// main(): demonstrations
// ------------------------------------------------------------
int main() {
    Implementation impl;
    impl.run();

    Printer p;
    p.start();

    Square s;
    process(s);

    std::unique_ptr<Resource> r = std::make_unique<File>();

    Duck d;
    d.fly();
    d.swim();

    return 0;
}

/*
============================================================
RULES OF THUMB (STRONG OPINIONS)
============================================================

1. Abstract class = at least one pure virtual function.
2. Always give abstract bases a virtual destructor.
3. Use abstract classes to express *contracts*, not reuse.
4. Prefer non-virtual public functions + virtual protected hooks (NVI).
5. Keep abstract bases small and stable.
6. Never put data members in interface-like bases.
7. Use `override` on ALL overrides.

============================================================
COMMON PITFALLS
============================================================

- Forgetting to override all pure virtual functions
- Assuming abstract classes cannot have implementations
- Missing destructor definition for pure virtual dtor
- Using abstract classes as data containers
- Fragile base class problem

============================================================
WHEN TO USE ABSTRACT CLASSES
============================================================

- Framework interfaces
- Plugin systems
- Strategy / State patterns
- Hardware or OS abstraction layers

============================================================
WHEN NOT TO USE THEM
============================================================

- Closed type hierarchies
- Value semantics
- Performance-critical paths

Prefer:
- Templates
- std::variant
- Function objects

============================================================
RELATED / ADVANCED
============================================================

- Interface segregation principle
- Type erasure
- CRTP for static interfaces
- std::function as behavioral interface

This file is suitable as a standalone topic document.
*/
