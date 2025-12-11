#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

// Utility: clear cin after failure
void clear_stdin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    // --- Basic integer input ---
    std::cout << "Enter an integer: ";
    int x{};
    if (!(std::cin >> x)) {                       // failure check
        std::cout << "Invalid integer.\n";
        clear_stdin();
    } else {
        std::cout << "You typed: " << x << "\n";
    }

    // --- Reading string without spaces ---
    std::cout << "Enter a word: ";
    std::string word;
    std::cin >> word;                             // stops at whitespace
    std::cout << "Word = " << word << "\n";

    // --- Reading full line including spaces ---
    clear_stdin();                                // flush leftover '\n'
    std::cout << "Enter a full line: ";
    std::string line;
    std::getline(std::cin, line);
    std::cout << "Line = \"" << line << "\"\n";

    // --- Mixing numeric and string input (common pitfall) ---
    std::cout << "Enter a number then a sentence:\n";
    int n{};
    if (std::cin >> n) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string sentence;
        std::getline(std::cin, sentence);
        std::cout << "n=" << n << ", sentence=" << sentence << "\n";
    } else {
        std::cout << "Invalid number.\n";
        clear_stdin();
    }

    // --- Handling EOF and detecting end of input ---
    std::cout << "Type numbers until EOF (Ctrl-D/Ctrl-Z):\n";
    long long sum = 0, tmp;
    while (std::cin >> tmp) sum += tmp;
    if (std::cin.eof()) {
        std::cout << "EOF reached. sum=" << sum << "\n";
    } else {
        std::cout << "Input failure before EOF.\n";
    }
    clear_stdin();

    // --- Reading characters, including whitespace ---
    std::cout << "Enter any character (including space): ";
    char c;
    c = std::cin.get();                            // raw char, can read '\n'
    std::cout << "ASCII=" << static_cast<int>(c) << "\n";

    // --- Output formatting ---
    std::cout << "Hex of 255 = " << std::hex << 255 << std::dec << "\n";
    std::cout << "Width/pad: [" << std::setw(6) << std::setfill('0') << 42 << "]\n";

    return 0;
}
