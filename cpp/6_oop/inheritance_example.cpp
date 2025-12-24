// Complete C++17 demonstration and mini-course on inheritance
// Topic: Inheritance example using Vehicle → Car hierarchy
// Compile with: g++ -std=c++17 -Wall -Wextra -pedantic inheritance_demo.cpp -o inheritance_demo

#include <iostream>
#include <string>
#include <vector>

// ================================================================
// 1. Base class: Vehicle
// ================================================================
class Vehicle {
protected:                  // Accessible to derived classes
    std::string brand;
    int         year;

public:
    // Constructor
    Vehicle(const std::string& b = "Unknown", int y = 2020)
        : brand(b), year(y) {
        std::cout << "Vehicle constructor called for " << brand << " (" << year << ")\n";
    }

    // Virtual destructor - CRUCIAL for polymorphic behavior
    virtual ~Vehicle() {
        std::cout << "Vehicle destructor called for " << brand << "\n";
    }

    // Virtual function - can be overridden
    virtual void start() const {
        std::cout << brand << " vehicle is starting (generic engine sound).\n";
    }

    // Another virtual function with a default implementation
    virtual void stop() const {
        std::cout << brand << " vehicle has stopped.\n";
    }

    // Non-virtual function - common to all vehicles
    void displayInfo() const {
        std::cout << "Brand: " << brand << ", Year: " << year << "\n";
    }

    // Getter and setter examples
    const std::string& getBrand() const { return brand; }
    void setBrand(const std::string& b) { brand = b; }

    int getYear() const { return year; }
};

// ================================================================
// 2. Derived class: Car (inherits publicly from Vehicle)
// ================================================================
class Car : public Vehicle {
private:
    int         numDoors;
    std::string fuelType;           // e.g., "Gasoline", "Electric", "Hybrid"

public:
    // Constructor using base class initializer list
    Car(const std::string& b = "Unknown",
        int y = 2020,
        int doors = 4,
        const std::string& fuel = "Gasoline")
        : Vehicle(b, y),            // Call base constructor
          numDoors(doors),
          fuelType(fuel) {
        std::cout << "Car constructor called: " << numDoors << "-door " << fuelType << "\n";
    }

    // Destructor
    ~Car() override {
        std::cout << "Car destructor called for " << brand << "\n";
    }

    // Override virtual function - specific behavior for Car
    void start() const override {
        std::cout << brand << " car engine roars to life! (Vroom vroom)\n";
    }

    // New function specific to Car
    void honk() const {
        std::cout << brand << " car: Beep beep!\n";
    }

    // Access and display car-specific info
    void displayCarInfo() const {
        displayInfo();                       // Call base class function
        std::cout << "Doors: " << numDoors << ", Fuel: " << fuelType << "\n";
    }
};

// ================================================================
// 3. Demonstration of inheritance concepts
// ================================================================
int main() {
    std::cout << "=== C++17 Inheritance Demo: Vehicle → Car ===\n\n";

    // ------------------------------------------------------------
    // Section A: Basic usage
    // ------------------------------------------------------------
    std::cout << "Section A: Creating objects\n";
    Car myCar("Tesla", 2024, 4, "Electric");
    std::cout << "\n";

    myCar.displayCarInfo();
    myCar.start();
    myCar.honk();
    myCar.stop();
    std::cout << "\n";

    // ------------------------------------------------------------
    // Section B: Polymorphism with pointers
    // ------------------------------------------------------------
    std::cout << "Section B: Polymorphism with base class pointers\n";
    Vehicle* ptr1 = new Car("BMW", 2023, 4, "Gasoline");
    Vehicle* ptr2 = new Vehicle("Generic Truck", 2018);

    std::vector<Vehicle*> garage = { ptr1, ptr2 };

    for (const auto* v : garage) {
        v->displayInfo();   // Non-virtual → always Vehicle version
        v->start();         // Virtual → calls the correct overridden version
        v->stop();
        std::cout << "---\n";
    }

    // Clean up
    delete ptr1;
    delete ptr2;
    std::cout << "\n";

    // ------------------------------------------------------------
    // Section C: Slicing demonstration (avoid this!)
    // ------------------------------------------------------------
    std::cout << "Section C: Object slicing (what NOT to do)\n";
    Car sportsCar("Porsche", 2025, 2, "Gasoline");
    Vehicle sliced = sportsCar;  // Copies only the Vehicle part → slicing!

    sliced.start();  // Calls Vehicle::start(), not Car::start()
    // sliced.honk(); // Error! honk() does not exist in Vehicle
    std::cout << "\n";

    // ------------------------------------------------------------
    // Section D: Using override and final (C++11/17 features)
    // ------------------------------------------------------------
    std::cout << "Section D: Modern C++ features (override already used above)\n";
    // The 'override' keyword ensures we are actually overriding a virtual function.
    // It catches errors at compile time if the signature doesn't match.

    std::cout << "Demo complete. Observe the order of constructors/destructors!\n";

    return 0;
}
