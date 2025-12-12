#include <iostream>
#include <string>
#include <optional>
#include <limits>
#include <type_traits>

using std::cout;
using std::string;

// Utility for demonstration
constexpr bool is_even(int x) noexcept { return (x & 1) == 0; }

int main() {
    cout << "=== Basic if/else ===\n";
    {
        int x = 5;
        if (x > 3) cout << "x > 3\n";
        else       cout << "x <= 3\n";
    }

    cout << "\n=== Dangling else demonstration ===\n";
    {
        // Classic pitfall: "else" attaches to closest "if"
        // ALWAYS use braces in complex conditions.
        int a = 1, b = 2;
        if (a)
            if (b)
                cout << "a && b\n";
            else
                cout << "else binds to inner-if ONLY\n";
    }

    cout << "\n=== Nested if/else with early return ===\n";
    {
        int score = 87;
        if (score >= 90) {
            cout << "A\n";
        } else if (score >= 80) {
            cout << "B\n";
        } else if (score >= 70) {
            cout << "C\n";
        } else {
            cout << "Fail\n";
        }
    }

    cout << "\n=== Deep nesting refactored using guard clauses ===\n";
    {
        auto authenticate = [](bool okPassword, bool okIP, bool isAdmin) {
            if (!okPassword) return "Password fail";
            if (!okIP)       return "IP blocked";
            if (!isAdmin)    return "Not admin";
            return "Authenticated"; // flat, avoids pyramid
        };
        cout << authenticate(true, true, true) << '\n';
    }

    cout << "\n=== if + initializer (C++17 hidden gem) ===\n";
    {
        // Scope-limited variable, reduces accidental variable leakage
        if (int x = 42; is_even(x))
            cout << "x even: " << x << '\n';
        else
            cout << "x odd: " << x << '\n';
        // x no longer exists here → safer than pre-declaring above
    }

    cout << "\n=== if constexpr for compile-time branching ===\n";
    {
        auto describe = [](auto v) {
            if constexpr (std::is_integral_v<decltype(v)>) {
                return "integral";
            } else if constexpr (std::is_floating_point_v<decltype(v)>) {
                return "floating";
            } else {
                return "other";
            }
        };
        cout << describe(10) << '\n';     // integral
        cout << describe(3.14) << '\n';   // floating
    }

    cout << "\n=== Short-circuit evaluation caveats ===\n";
    {
        int x = 0;

        // Right side will NOT execute because left is false → safe for null checks
        if (x != 0 && (10 / x) == 2) { } // not evaluated

        // BUT: avoid putting side effects in expressions
        int a = 0;
        if (true || ++a == 1) { }
        cout << "a remains 0 due to short-circuit\n"; // ++a never happens
    }

    cout << "\n=== Comparing signed/unsigned pitfalls ===\n";
    {
        unsigned u = 1;
        int s = -5;

        // if (s < u) is TRUE because s gets converted to unsigned → huge value!
        if (s < u)
            cout << "Signed/unsigned surprise: TRUE\n";
    }

    cout << "\n=== if with std::optional ===\n";
    {
        std::optional<int> v = 10;
        if (v) cout << "Optional has value = " << *v << '\n';
        else   cout << "Optional empty\n";

        // Combined with C++17 initializer
        if (auto x = std::optional<int>(5); x && *x > 3)
            cout << "Optional + guard\n";
    }

    cout << "\n=== String comparison traps ===\n";
    {
        const char* a = "hello";
        const char* b = "hello";

        // Comparing pointers, not content
        if (a == b)
            cout << "Pointer-equal (not string-equal guarantee)\n";

        // Correct
        if (string(a) == string("hello"))
            cout << "String match\n";
    }

    // TODO: correct this
    cout << "\n=== Branch prediction hint (indirect) ===\n";
    {
        // Modern compilers guess based on code frequency.
        // Heavy code inside cold branch → ensure it's last.
        int x = 1;
        if (x == 0) {                  // unlikely path
            cout << "rare\n";
        } else {                       // likely path
            cout << "common\n";
        }
    }

    cout << "\n=== Avoiding if-chain using lookup tables ===\n";
    {
        int state = 2;
        // Example: replace large if-else chains where branching is predictable.
        static const char* tbl[] = {
            "zero", "one", "two", "three"
        };
        if (state >= 0 && state < 4)
            cout << tbl[state] << '\n';
        else
            cout << "unknown\n";
    }

    cout << "\n=== Nested if vs combined conditions ===\n";
    {
        int a = 5, b = 10;
        if (a > 0) {
            if (b > 0)
                cout << "Both positive (nested)\n";
        }

        if (a > 0 && b > 0)
            cout << "Both positive (combined)\n";

        // Combined version is cleaner; nested version sometimes needed for scoping.
    }

    cout << "\n=== Multi-branch logic with early-exit for speed ===\n";
    {
        int x = 20;
        if (x < 0) {
            cout << "neg\n";
            return 0;
        }
        if (x < 10) {
            cout << "small\n";
            return 0;
        }
        if (x < 100) {
            cout << "mid\n";
        }
    }

    cout << "\n=== Constant conditions anti-pattern ===\n";
    {
        if (false) cout << "never\n";  // compiled out
        if (true)  cout << "always\n"; // compiled in
        // OK in compile-time meta code, but avoid in runtime logic.
    }

    cout << "\n=== if guarding heavy work lazily ===\n";
    {
        auto expensive = [] {
            cout << "[expensive work]\n";
            return 42;
        };

        bool condition = false;

        // Cheap skip. No expensive() call.
        if (condition && expensive() == 42) {}

        condition = true;
        if (condition && expensive() == 42) {}
    }

    cout << "\n=== Avoiding deep nesting by inverting early checks ===\n";
    {
        auto process = [](int x) {
            if (x <= 0) return;       // early exit
            if (x > 100) return;      // guard
            cout << "processing " << x << '\n';
        };
        process(10);
    }

    cout << "\n=== Using if to enforce invariants ===\n";
    {
        int x = 10;
        if (!(x > 0)) {
            std::terminate(); // strict invariant
        }
        cout << "Invariant OK\n";
    }

    cout << "\n=== if + noexcept edge case ===\n";
    {
        auto f = []() noexcept { return 1; };
        if constexpr (noexcept(f())) {
            cout << "Guaranteed no-throw\n";
        }
    }
}
