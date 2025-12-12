#include <iostream>
#include <string>

/*
 switch can be misleading but that is a bad code style
 somthing like this code

  while (x < 4 ) {
    switch (x) {
        case 1:
            cout << "start\n";
            for (x = 0; x < 2; ++x)
        case 2:
                cout << x << "\n";
            break;
        case 3:
            cout << "end\n";
            x = 4;
            break;
    }
  }
*/


using namespace std;

// Modern C++ enum (strongly typed)
enum class Color {
    Red,
    Green,
    Blue,
    Yellow
};

int main() {
    Color color = Color::Green;

    cout << "=== Basic switch with enum + default ===\n";
    switch (color) {
        case Color::Red:
            cout << "Red color\n";
            break;  // must break to avoid fallthrough
        case Color::Green:
            cout << "Green color\n";
            break;
        case Color::Blue:
            cout << "Blue color\n";
            break;
        default:
            cout << "Unknown color (default)\n";
            break;
    }

    // === Pitfall: forgetting break ===
    cout << "\n=== Pitfall: missing break ===\n";

    color = Color::Red;
    switch (color) {
        case Color::Red:
            cout << "Red\n";
        case Color::Green:  // execution continues here if previous case falls through
            cout << "Green (fallthrough) executed too! \n";
        default:
            cout << "Default executed too!\n";
    }

    // === Modern trick: [[fallthrough]] to document intentional fallthrough ===
    cout << "\n=== Explicit fallthrough ===\n";
    switch (color) {
        case Color::Red:
            [[fallthrough]]; // signals to compiler that this fallthrough is intentional
        case Color::Green:
            cout << "Red & Green\n";
            break;
        default:
            cout << "Default\n";
    }

    // === Best practice: exhaustive switch using compiler warnings ===
    // If you cover all enum values, you can omit default and let compiler warn on missing cases
    cout << "\n=== Exhaustive switch without default (C++17) ===\n";
    switch (color) {
        case Color::Red: cout << "Red\n"; break;
        case Color::Green: cout << "Green\n"; break;
        case Color::Blue: cout << "Blue\n"; break;
        case Color::Yellow: cout << "Yellow\n"; break;
    }

    return 0;
}
