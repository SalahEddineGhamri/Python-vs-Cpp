// read_text_file_line_by_line.cpp
// A complete, self-contained C++17 tutorial and demonstration
// on reading a text file line by line.
//
// This single file covers:
// - Basic usage with std::ifstream and std::getline
// - Good practices (RAII, error checking, handling different line endings)
// - Common pitfalls and constraints
// - Interesting tricks (empty line detection, trailing newline, performance notes)
//
// Compile with: g++ -std=c++17 -Wall -Wextra -O2 read_text_file_line_by_line.cpp -o read_text_file
//
// Priority: Code is correct, builds cleanly, and demonstrates real-world usage.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>

/*
    READING A TEXT FILE LINE BY LINE IN C++17

    The most idiomatic and safe way is to use:
        std::ifstream  - for file input
        std::getline   - reads until '\n' (or custom delimiter)

    std::getline consumes the delimiter but does NOT include it in the string.

    Key points:
    - Lines are defined by newline characters ('\n' on Unix, "\r\n" on Windows).
      std::getline handles both transparently.
    - If the file does not end with a newline, the last line is still returned
      (no empty extra line).
    - If the file ends with a newline, std::getline will return an empty string
      for that final empty line.

    Good usage:
    - Always check if the file opened successfully.
    - Use RAII: let std::ifstream close the file automatically.
    - Process lines immediately (streaming) to keep memory usage low.
*/

int main() {
    const std::string filename = "sample.txt";

    // ------------------------------------------------------------
    // 1. Basic line-by-line reading (recommended for most cases)
    // ------------------------------------------------------------
    std::ifstream file(filename);                // RAII - file closes automatically

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return 1;
    }

    std::string line;
    int line_number = 0;

    std::cout << "=== Content of " << filename << " ===\n";

    while (std::getline(file, line)) {
        ++line_number;
        std::cout << line_number << ": " << line << '\n';

        // Interesting fact:
        // If the line was empty (just a newline in the file), line will be ""
        if (line.empty()) {
            std::cout << "   (this was an empty line)\n";
        }
    }

    // ------------------------------------------------------------
    // 2. Check why the loop ended - important for error detection
    // ------------------------------------------------------------
    if (file.eof()) {
        std::cout << "\nReached end of file normally.\n";
    }
    if (file.fail()) {
        // failbit can be set on I/O error (rare with text files)
        std::cerr << "I/O failure while reading the file.\n";
    }
    if (file.bad()) {
        std::cerr << "Serious I/O error (corrupted stream).\n";
    }

    // ------------------------------------------------------------
    // 3. Common alternative: store all lines in a vector
    // ------------------------------------------------------------
    // Only do this when the file is reasonably small!
    file.clear();                   // clear EOF flag
    file.seekg(0);                  // rewind to beginning

    std::vector<std::string> all_lines;
    while (std::getline(file, line)) {
        all_lines.push_back(line);
    }

    std::cout << "\nStored " << all_lines.size() << " lines in memory.\n";

    // ------------------------------------------------------------
    // 4. Pitfall demonstration: detecting if file ended with newline
    // ------------------------------------------------------------
    // If the file ends with '\n', std::getline returns an extra empty line.
    // Example file content:
    // line1\n
    // line2\n
    // line3\n   <- trailing newline
    // -> all_lines will contain {"line1", "line2", "line3", ""}

    if (!all_lines.empty() && all_lines.back().empty()) {
        std::cout << "Note: The file ended with a newline (empty line detected at end).\n";
    }

    // ------------------------------------------------------------
    // 5. Advanced trick: reading lines with custom delimiter
    // ------------------------------------------------------------
    file.clear();
    file.seekg(0);

    std::cout << "\nReading until '|' character (custom delimiter demo):\n";
    while (std::getline(file, line, '|')) {
        std::cout << "Part: [" << line << "]\n";
    }

    // ------------------------------------------------------------
    // 6. Constraints & Performance notes
    // ------------------------------------------------------------
    /*
        - std::getline allocates for each line dynamically -> fine for typical text files.
        - For extremely large lines (> few MB), consider reading in chunks.
        - Binary files: do NOT use getline; use file.read() instead.
        - Unicode: std::string holds bytes. For proper UTF-8 processing you may need
          additional libraries (e.g., iconv, ICU). Basic ASCII/Latin-1 works out of the box.
        - Exception safety: if you enable exceptions on streams (file.exceptions(...))
          you can catch failures, but most code prefers checking flags as shown.
    */

    return 0;
}

/*
    SAMPLE FILE (create "sample.txt" next to the executable):

    Hello world
    This is line 2

    Line 4 is empty above
    Final line

    (note the trailing newline after "Final line")

    Running the program will clearly show:
    - Line numbers
    - Empty line detection
    - Trailing newline detection
    - Custom delimiter example
*/
