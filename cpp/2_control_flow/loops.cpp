#include <iostream>
#include <vector>
#include <optional>

using namespace std;

int main() {

    cout << "=== 1. Classic for loop ===\n";
    // Most flexible. All 3 clauses optional.
    // for (init; condition; increment)
    for (int i = 0; i < 3; ++i)
        cout << i << ' ';

    // All parts optional → infinite loop.
    for (;;) {
        cout << "\nInfinite-for break demo\n";
        break;                 // always escape manually
    }

    // Pitfall: modifying the loop variable inside the body.
    for (int i = 0; i < 5; ++i) {
        if (i == 2) i += 2;    // legal but confusing
        cout << i << ' ';
    }

    // Best practice: keep variable lifetime inside loop.
    for (int i = 0; i < 3; ++i)
        cout << "\nScoped i: " << i;


    cout << "\n\n=== 2. Range-based for loop (C++11–17 idiomatic) ===\n";
    vector<int> v{1,2,3};

    // Best practice: use const auto& unless modification needed.
    for (const auto &x : v)
        cout << x << ' ';

    // If you want to mutate:
    for (auto &x : v) x *= 2;

    // If you want a copy:
    for (auto x : v) cout << x << ' ';

    // Edge case: iterating over temporary
    cout << "\nTemporary vector ... \n";
    for (int x : vector<int>{9,8,7})
        cout << x << ' ';


    cout << "\n\n=== 3. while loop ===\n";
    int w = 0;

    while (w < 3) {            // condition checked first
        cout << w << ' ';
        ++w;
    }

    // Infinite loop idiom:
    while (true) {
        cout << "\nWhile-infinite break demo";
        break;
    }

    // Pitfall: forgetting to update the variable → infinite loop.
    int frozen = 0;
    while (frozen < 2) {       // frozen never increments → infinite
        cout << "";            // intentionally empty
        break;                 // avoiding infinite freeze in demo
    }


    cout << "\n\n=== 4. do...while loop ===\n";
    int d = 0;

    // Always executes at least once.
    do {
        cout << d << ' ';
        ++d;
    } while (d < 3);

    // Edge case: run-once pattern.
    // Useful when a block must run before condition is checked.
    do {
        cout << "\nRun once even if condition false";
    } while (false);


    cout << "\n\n=== 5. continue & break ===\n";
    for (int i = 0; i < 5; ++i) {
        if (i == 1) continue;  // skip this iteration
        if (i == 3) break;     // exit loop entirely
        cout << i << ' ';
    }


    cout << "\n\n=== 6. Nested loops ===\n";
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            cout << '(' << i << ',' << j << ") ";


    cout << "\n\n=== 7. Loop with early exit using optional ===\n";
    // Idiomatic C++17: use optional to represent "found" state
    vector<int> data{10, 30, 50};
    optional<int> found;

    for (int x : data) {
        if (x > 20) {
            found = x;
            break;            // early exit
        }
    }
    if (found) cout << "\nFound: " << *found;


    cout << "\n\n=== 8. Loop + RAII safety ===\n";
    struct Guard {
        ~Guard() { cout << "\nGuard cleaned (destructor)."; }
    };

    for (int i = 0; i < 2; ++i) {
        Guard g;              // clean exit even on break/continue
        if (i == 0) continue; // safe: destructor still runs
        cout << "\nBody " << i;
    }


    cout << "\n\n=== 9. Python comparison ===\n";
    cout << R"(
Python      | C++
---------------------------------------
for x in it | range-for or iterator for
while cond  | while
--          | do...while (no Python equivalent)
break       | break
continue    | continue
)";


    cout << "\n\n=== 10. Exotic: for with empty body ===\n";
    // Useful for scanning until condition meets.
    int scan = 0;
    for (; scan < 5; ++scan)
        ; // <-- intentional empty body
    cout << "scan=" << scan;


    return 0;
}
