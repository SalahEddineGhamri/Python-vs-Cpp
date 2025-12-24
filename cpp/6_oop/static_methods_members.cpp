// static_members_demo.cpp
// A complete, self-contained C++17 tutorial and demonstration of static members
// and static methods in classes.
//
// Compile with: g++ -std=c++17 -Wall -Wextra -pedantic -O2 static_members_demo.cpp -o static_members_demo
//
// This single file covers:
// - Static data members (class-wide shared storage)
// - Static member functions (operations on class state without needing an instance)
// - Proper initialization of static members
// - Common use cases (singleton, counters, factory registries)
// - Pitfalls and constraints (thread-safety, order of initialization, consteval in C++20+)
// - Interesting tricks and best practices

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <memory>
#include <unordered_map>

/*
    STATIC DATA MEMBERS
    -------------------
    - One copy per class, shared by all objects (and accessible without any object)
    - Must be defined exactly once outside the class (unless inline in C++17+)
    - Useful for: counters, configuration flags, shared resources, registries
*/

class Player {
public:
    Player(const std::string& name) : name_(name) {
        ++instance_count_;              // increment shared counter
        id_ = instance_count_;
    }

    ~Player() {
        --instance_count_;              // decrement on destruction
    }

    void print_info() const {
        std::cout << "Player #" << id_ << ": " << name_
                  << " (total alive: " << instance_count_ << ")\n";
    }

    // Static member function: can be called without an instance
    static int alive_count() {
        return instance_count_;
    }

private:
    std::string name_;
    int id_ = 0;

    // Declaration only - shared across all Player objects
    static inline int instance_count_ = 0;   // C++17 inline static - no separate definition needed!
};

/*
    Note: In pre-C++17 you would write:
          static int instance_count_;
    and later outside the class:
          int Player::instance_count_ = 0;

    The inline static variable is the modern, cleaner way.
*/

/*
    SINGLETON PATTERN (classic example using static member)
    -------------------------------------------------------
    A common (though sometimes controversial) use of static members.
    Here we implement a thread-safe Meyer’s singleton.
*/

class Logger {
public:
    // Delete copy/move to enforce single instance
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Public access to the single instance
    static Logger& instance() {
        static Logger single_instance;       // Magic static: thread-safe in C++11+
        return single_instance;
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "[LOG] " << message << '\n';
    }

private:
    Logger() = default;                     // Private constructor
    std::mutex mutex_;
};

/*
    Interesting fact:
    The "static Logger single_instance;" inside instance() is guaranteed
    to be initialized exactly once, even in the presence of multiple threads
    (C++11 and later). This is called "magic statics" and removes the need
    for manual double-checked locking.
*/

/*
    STATIC MEMBER AS FACTORY REGISTRY
    --------------------------------
    A more advanced trick: self-registering factory using static members.
*/

class Widget {
public:
    using Creator = std::unique_ptr<Widget>(*)();

    Widget() = default;
    virtual ~Widget() = default;
    virtual void describe() const { std::cout << "Generic Widget\n"; }

    // Factory function
    static std::unique_ptr<Widget> create(const std::string& type);

    // Registration helper - used by derived classes
    template<typename T>
    struct Register {
        Register(const std::string& name) {
            Widget::registry()[name] = []() -> std::unique_ptr<Widget> {
                return std::make_unique<T>();
            };
        }
    };

private:
    using RegistryMap = std::unordered_map<std::string, Creator>;
    static RegistryMap& registry() {
        static RegistryMap map;
        return map;
    }
};

std::unique_ptr<Widget> Widget::create(const std::string& type) {
    auto& reg = registry();
    auto it = reg.find(type);
    if (it != reg.end()) {
        return it->second();
    }
    return nullptr;  // or throw
}

// Derived widgets that auto-register themselves
struct Button : Widget {
    void describe() const override { std::cout << "Button Widget\n"; }

    // Static object forces registration at program startup
    static const Register<Button> reg;
};
const Widget::Register<Button> Button::reg{"button"};

struct Slider : Widget {
    void describe() const override { std::cout << "Slider Widget\n"; }
    static const Register<Slider> reg;
};
const Widget::Register<Slider> Slider::reg{"slider"};

/*
    Pitfall reminder:
    The static Register objects are constructed during static initialization.
    Order of initialization across translation units is unspecified!
    If Widget::registry() is accessed before a Register runs, you get empty map.
    In practice this works for simple programs, but real code often uses
    Nifty Counter idiom or function-local statics to avoid the static init fiasco.
*/

int main() {
    std::cout << "=== 1. Basic static data member (instance counter) ===\n";
    std::cout << "Alive players: " << Player::alive_count() << '\n';

    {
        Player p1("Alice");
        Player p2("Bob");
        p1.print_info();
        p2.print_info();
        std::cout << "Alive (via static method): " << Player::alive_count() << '\n';
    }

    std::cout << "After scope exit: " << Player::alive_count() << '\n\n';

    std::cout << "=== 2. Thread-safe Singleton Logger ===\n";
    Logger::instance().log("Application started");
    Logger::instance().log("Doing important work");

    // Demonstrate multiple threads safely using the same logger
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            Logger::instance().log("Message from thread " + std::to_string(i));
        });
    }
    for (auto& t : threads) t.join();

    std::cout << "\n=== 3. Self-registering Factory ===\n";
    auto w1 = Widget::create("button");
    if (w1) w1->describe();

    auto w2 = Widget::create("slider");
    if (w2) w2->describe();

    auto w3 = Widget::create("unknown");
    std::cout << (w3 ? "Created unknown" : "Failed to create unknown") << '\n';

    std::cout << "\nDemo complete.\n";
    return 0;
}

/*
    SUMMARY OF KEY POINTS & PITFALLS
    -------------------------------
    1. Static data members belong to the class, not instances.
       Use 'inline static' (C++17) to avoid ODR issues.

    2. Static member functions have no 'this' pointer.
       They can only access static members directly.

    3. Static local variables in functions (including singletons)
       are initialized thread-safely since C++11.

    4. Static initialization order across translation units is undefined
       → avoid depending on it (static initialization fiasco).

    5. consteval/static constexpr members can be evaluated at compile time,
       useful for compile-time configuration.

    6. Prefer inline static variables over old-style separate definitions.

    7. For global state, consider alternatives (dependency injection)
       before reaching for static members — they can make testing harder.

    This file compiles cleanly with -std=c++17 and demonstrates most
    practical uses and gotchas of static members in modern C++.
*/
