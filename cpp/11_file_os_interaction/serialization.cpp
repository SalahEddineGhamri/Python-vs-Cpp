// binary_struct_serialization.cpp
// Complete C++17 tutorial and demo: Binary read/write of structs (serialization)
// Compile: g++ -std=c++17 -Wall -Wextra -O2 binary_struct_serialization.cpp -o binary_struct_serialization
// Run: ./binary_struct_serialization

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>     // fixed-width integers
#include <filesystem>

/*
This program teaches safe and practical binary serialization of structs in C++.

We cover:
1. Writing multiple structs to a binary file
2. Reading them back exactly as they were
3. Appending new structs to an existing file
4. Updating/modifying a specific struct by index
5. Key pitfalls: padding, alignment, portability

Core idea (simple explanation):
- Binary mode + write()/read() copies raw memory bytes to/from file
- Works perfectly if the struct is "Plain Old Data" (POD) and has no padding surprises
- We use fixed-width types (uint32_t, float, fixed char array) for better control

Interesting facts & tricks:
- Compilers add padding for alignment → sizeof(Record) may be larger than you expect!
- Order of members matters: putting char array first can eliminate padding
- Never rely on binary format across different compilers, architectures, or versions
- For true portability: use text (JSON, XML) or libraries like Protocol Buffers
- std::endian (C++20) helps detect byte order, but here we keep it simple
*/

namespace fs = std::filesystem;

// Careful design: char array first → often eliminates padding
struct Record {
    char     name[16];    // fixed-size, null-terminated if you want
    std::uint32_t id;
    float    score;
    std::uint16_t age;
};

// Helper to print one record nicely
void print_record(const Record& r, size_t index) {
    std::cout << "  [" << index << "] name: \"" << r.name
              << "\" id: " << r.id
              << " score: " << r.score
              << " age: " << r.age << "\n";
}

int main() {
    const std::string filename = "records.bin";

    std::vector<Record> initial_data = {
        {"Alice",      1, 95.5f, 21},
        {"Bob",        2, 87.0f, 19},
        {"Charlie",    3, 92.3f, 22},
        {"Diana",      4, 78.9f, 20}
    };

    // --------------------------------------------------------------------
    // 1. Write (overwrite) binary file with initial records
    // --------------------------------------------------------------------
    std::cout << "=== Step 1: Writing " << initial_data.size() << " records to new file ===\n";
    {
        std::ofstream out(filename, std::ios::binary | std::ios::trunc);
        if (!out) {
            std::cerr << "Error: Cannot create " << filename << "\n";
            return 1;
        }

        for (const auto& r : initial_data) {
            out.write(reinterpret_cast<const char*>(&r), sizeof(Record));
            if (!out.good()) {
                std::cerr << "Error during write!\n";
                return 1;
            }
        }
        out.flush();  // ensure all data hits disk
    }

    std::cout << "File size: " << fs::file_size(filename) << " bytes\n";
    std::cout << "Expected size: " << initial_data.size() << " * " << sizeof(Record)
              << " = " << initial_data.size() * sizeof(Record) << " bytes\n\n";

    // --------------------------------------------------------------------
    // 2. Read back and display
    // --------------------------------------------------------------------
    std::cout << "=== Step 2: Reading all records back ===\n";
    std::vector<Record> loaded;
    {
        std::ifstream in(filename, std::ios::binary);
        if (!in) {
            std::cerr << "Error: Cannot open file for reading\n";
            return 1;
        }

        Record r;
        while (in.read(reinterpret_cast<char*>(&r), sizeof(Record))) {
            loaded.push_back(r);
        }

        // Check for read errors other than EOF
        if (in.bad()) {
            std::cerr << "Error during read!\n";
            return 1;
        }
    }

    for (size_t i = 0; i < loaded.size(); ++i) {
        print_record(loaded[i], i);
    }
    std::cout << "\n";

    // --------------------------------------------------------------------
    // 3. Append new records
    // --------------------------------------------------------------------
    std::cout << "=== Step 3: Appending 2 new records ===\n";
    std::vector<Record> new_records = {
        {"Eve",   5, 99.9f, 23},
        {"Frank", 6, 65.4f, 18}
    };

    {
        std::ofstream out(filename, std::ios::binary | std::ios::app);
        if (!out) {
            std::cerr << "Error: Cannot append\n";
            return 1;
        }
        for (const auto& r : new_records) {
            out.write(reinterpret_cast<const char*>(&r), sizeof(Record));
        }
    }

    // Read everything again to confirm append worked
    loaded.clear();
    {
        std::ifstream in(filename, std::ios::binary);
        Record r;
        while (in.read(reinterpret_cast<char*>(&r), sizeof(Record))) {
            loaded.push_back(r);
        }
    }
    std::cout << "Now total " << loaded.size() << " records:\n";
    for (size_t i = 0; i < loaded.size(); ++i) {
        print_record(loaded[i], i);
    }
    std::cout << "\n";

    // --------------------------------------------------------------------
    // 4. Modify a specific record (random access)
    // --------------------------------------------------------------------
    std::cout << "=== Step 4: Modifying record #2 (Charlie → Zelda) ===\n";
    {
        std::fstream fio(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (!fio) {
            std::cerr << "Error: Cannot open for update\n";
            return 1;
        }

        const size_t index_to_change = 2;
        fio.seekp(index_to_change * sizeof(Record));

        Record updated = {"Zelda", 999, 100.0f, 25};
        fio.write(reinterpret_cast<const char*>(&updated), sizeof(Record));
        fio.flush();
    }

    // Read final state
    loaded.clear();
    {
        std::ifstream in(filename, std::ios::binary);
        Record r;
        while (in.read(reinterpret_cast<char*>(&r), sizeof(Record))) {
            loaded.push_back(r);
        }
    }
    for (size_t i = 0; i < loaded.size(); ++i) {
        print_record(loaded[i], i);
    }
    std::cout << "\n";

    // --------------------------------------------------------------------
    // 5. Important facts, pitfalls and constraints
    // --------------------------------------------------------------------
    std::cout << "=== Key facts and pitfalls ===\n";
    std::cout << "- sizeof(Record) = " << sizeof(Record) << " bytes (check with your compiler!)\n";
    std::cout << "  → Compiler may add padding for alignment.\n";
    std::cout << "  → Putting char array first often removes padding (as shown here).\n";
    std::cout << "- This binary format is NOT portable across:\n";
    std::cout << "    • Different compilers (GCC vs MSVC vs Clang)\n";
    std::cout << "    • Different architectures (x86_64 vs ARM vs big-endian)\n";
    std::cout << "    • Different struct versions (added/removed members)\n";
    std::cout << "- Always open binary files with std::ios::binary (especially on Windows).\n";
    std::cout << "- Use fixed-width types (uint32_t, int64_t) for more predictable size.\n";
    std::cout << "- For real-world cross-platform serialization, prefer:\n";
    std::cout << "    • Libraries: protobuf, flatbuffers, msgpack, cereal\n";
    std::cout << "    • Text formats: JSON, XML, CSV\n";

    std::cout << "\nDemo complete! File '" << filename << "' contains "
              << loaded.size() << " binary records.\n";
    std::cout << "You can inspect raw bytes with: hexdump -C " << filename << " (Linux/macOS)\n";

    return 0;
}
