#include <iostream>
#include <limits>
#include <cstdint>
#include <cstddef>
#include <type_traits>

template<typename T>
void print_info(const std::string &name) {
    std::cout << "Type: " << name << "\n";
    std::cout << "  Size: " << sizeof(T) << " bytes\n";
    std::cout << "  Min: " << +std::numeric_limits<T>::min() << "\n";
    std::cout << "  Max: " << +std::numeric_limits<T>::max() << "\n";
    std::cout << "  Is signed: " << std::boolalpha << std::numeric_limits<T>::is_signed << "\n";
    std::cout << "  Alignment: " << alignof(T) << " bytes\n";
    std::cout << "---------------------------\n";
}

int main() {
    // Fundamental types
    print_info<char>("char");
    print_info<signed char>("signed char");
    print_info<unsigned char>("unsigned char");
    print_info<bool>("bool");
    print_info<short>("short");
    print_info<unsigned short>("unsigned short");
    print_info<int>("int");
    print_info<unsigned int>("unsigned int");
    print_info<long>("long");
    print_info<unsigned long>("unsigned long");
    print_info<long long>("long long");
    print_info<unsigned long long>("unsigned long long");
    print_info<float>("float");
    print_info<double>("double");
    print_info<long double>("long double");

    // Fixed-width types
    print_info<int8_t>("int8_t");
    print_info<uint8_t>("uint8_t");
    print_info<int16_t>("int16_t");
    print_info<uint16_t>("uint16_t");
    print_info<int32_t>("int32_t");
    print_info<uint32_t>("uint32_t");
    print_info<int64_t>("int64_t");
    print_info<uint64_t>("uint64_t");

    // Character variants
    print_info<wchar_t>("wchar_t");
    print_info<char16_t>("char16_t");
    print_info<char32_t>("char32_t");

    // Pointer types
    print_info<int*>("int*");
    print_info<void*>("void*");
    print_info<double*>("double*");

    // size_t and ptrdiff_t
    print_info<std::size_t>("size_t");
    print_info<std::ptrdiff_t>("ptrdiff_t");

    return 0;
}
