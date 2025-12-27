// file_io_basics.cpp
// A complete standalone C++17 tutorial and demo for basic file writing and appending
// Compile with: g++ -std=c++17 -Wall -Wextra -O2 file_io_basics.cpp -o file_io_basics
// Run with: ./file_io_basics

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

/*
This single file demonstrates two fundamental file operations in modern C++:
1. Writing (creating or overwriting) a file
2. Appending to an existing file (or creating it if it doesn't exist)

We use std::ofstream (output file stream) from <fstream>, which is the standard,
type-safe way to write text or binary data to files.

Key concepts explained simply:
- std::ofstream opens a file for writing.
- By default, opening with std::ofstream TRUNCATES (overwrites) the file.
- To append instead of overwrite, we pass std::ios::app flag.
- Always check if the stream is open successfully — file operations can fail
  (permissions, disk full, invalid path, etc.).
- Use RAII: the destructor of ofstream automatically closes the file.
- Modern C++17 adds <filesystem> to check if files exist, create directories, etc.

Interesting facts & tricks:
- std::ios::app forces EVERY write to go to the end of the file, even after seekp().
- You can combine flags with | : std::ios::out | std::ios::app | std::ios::binary
- flush() forces data to disk; close() also flushes.
- Exceptions can be enabled with exceptions() member, but most code prefers checking fail().
*/

namespace fs = std::filesystem;

int main() {
    const std::string filename = "demo_file.txt";
    const std::vector<std::string> lines_to_write = {
        "First line - this will be written when we CREATE/OVERWRITE the file.\n",
        "Second line - same operation.\n",
        "Third line.\n"
    };

    const std::vector<std::string> lines_to_append = {
        "First appended line - added later.\n",
        "Second appended line.\n",
        "This proves we did NOT overwrite the original content!\n"
    };

    // --------------------------------------------------------------------
    // 1. Writing (creating or overwriting) a file
    // --------------------------------------------------------------------
    std::cout << "=== Step 1: Writing (overwriting) the file ===\n";
    { // we use namespace so RAII take care of closing resources when getting out
      // Open in default mode: output + truncate
      std::ofstream outfile(filename);  // equivalent to std::ofstream(filename, std::ios::out | std::ios::trunc);

      if (!outfile.is_open()) {
          std::cerr << "Error: Could not open " << filename << " for writing!\n";
          return 1;
      }

      // Good usage: check stream state after each operation
      for (const auto& line : lines_to_write) {
          outfile << line;
          if (!outfile.good()) {
              std::cerr << "Error writing to file!\n";
              return 1;
          }
      }

      // outfile closes automatically when it goes out of scope (RAII)
      std::cout << "Successfully wrote " << lines_to_write.size() << " lines to " << filename << "\n\n";
    }


    // --------------------------------------------------------------------
    // Verify what we wrote
    // --------------------------------------------------------------------
    std::cout << "Current content of " << filename << ":\n";
    {
        std::ifstream infile(filename);
        std::string line;
        while (std::getline(infile, line)) {
            std::cout << " " << line << "\n";
        }
    }
    std::cout << "\n";

    // --------------------------------------------------------------------
    // 2. Appending to the file
    // --------------------------------------------------------------------
    { // we use namespace so RAII take care of closing resources when getting out
      std::cout << "=== Step 2: Appending to the file ===\n";

      // Open with append flag — this is the key difference!
      std::ofstream append_stream(filename, std::ios::app);

      if (!append_stream.is_open()) {
          std::cerr << "Error: Could not open " << filename << " for appending!\n";
          return 1;
      }

      for (const auto& line : lines_to_append) {
          append_stream << line;
      }

      std::cout << "Successfully appended " << lines_to_append.size() << " lines.\n\n";
    }

    // --------------------------------------------------------------------
    // Final content after appending
    // --------------------------------------------------------------------
    std::cout << "Final content of " << filename << " after appending:\n";
    {
        std::ifstream final_read(filename);
        std::string line;
        while (std::getline(final_read, line)) {
            std::cout << "  " << line << "\n";
        }
    }

    // --------------------------------------------------------------------
    // Pitfalls and constraints highlighted
    // --------------------------------------------------------------------
    std::cout << "\n=== Common pitfalls and useful tricks ===\n";
    std::cout << "- Opening std::ofstream without flags → truncates (overwrites) the file.\n";
    std::cout << "- Forgetting to check is_open() or good() → silent failures on permission errors.\n";
    std::cout << "- std::ios::app forces ALL writes to the end; you cannot seek and insert in the middle.\n";
    std::cout << "- If you need both overwrite and append in the same program, use two different ofstream objects.\n";
    std::cout << "- Binary data: add std::ios::binary flag to avoid newline translation on Windows.\n";

    // Bonus: C++17 filesystem check
    if (fs::exists(filename)) {
        std::cout << "\nFile size is " << fs::file_size(filename) << " bytes (C++17 <filesystem>).\n";
    }

    std::cout << "\nDemo complete! Check '" << filename << "' in the current directory.\n";

    return 0;
}
