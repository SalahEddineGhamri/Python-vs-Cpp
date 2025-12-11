#include <iostream>
#include <string>
#include <type_traits>
#include <bitset>
#include <cassert>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

// ============================================================================
// PART 1: BASIC ENUMS (C-Style Enums)
// ============================================================================

// Basic Enums should be avoided
namespace BasicEnums {
    // 1.1 Basic unscoped enum (C-style)
    enum Color { RED, GREEN, BLUE };  // Values: 0, 1, 2

    // 1.2 With explicit values
    enum HttpStatus {
        OK = 200,
        CREATED = 201,
        BAD_REQUEST = 400,
        NOT_FOUND = 404,
        INTERNAL_ERROR = 500
    };

    // 1.3 Auto-incremented values
    enum Days {
        MONDAY = 1,  // 1
        TUESDAY,     // 2
        WEDNESDAY,   // 3
        THURSDAY,    // 4
        FRIDAY = 10, // 10
        SATURDAY,    // 11
        SUNDAY       // 12
    };

    // 1.4 Shared namespace pollution (PITFALL)
    enum Apple { FUJI, MACINTOSH, GRANNY_SMITH };
    enum Orange { NAVEL, BLOOD, MANDARIN };
    // Problem: Can't have both FUJI and NAVEL in same scope!

    void demonstrateBasicEnums() {
        cout << "\n=== BASIC (C-STYLE) ENUMS ===\n";

        Color c = RED;
        cout << "Color value: " << c << " (RED=" << RED << ")\n";

        // Implicit conversion to int (PITFALL)
        int colorValue = GREEN;  // Works: 1
        cout << "GREEN as int: " << colorValue << endl;

        // Implicit conversion from int (PITFALL)
        Color c2 = static_cast<Color>(2);  // BLUE
        Color c3 = static_cast<Color>(99); // DANGER: Invalid value!
        cout << "c3 (invalid): " << c3 << endl;

        // Comparison works
        if (c == RED) {
            cout << "Color is RED\n";
        }

        // Arithmetic works but dangerous
        Color nextColor = static_cast<Color>(c + 1);  // GREEN
        cout << "Next color: " << nextColor << endl;
    }
}

// ============================================================================
// PART 2: SCOPED ENUMS (enum class - C++11)
// ============================================================================

namespace ScopedEnums {
    // 2.1 Scoped enum (enum class) - strongly typed
    enum class Color { RED, GREEN, BLUE };

    // 2.2 With underlying type specification
    enum class Byte : unsigned char {
        ZERO = 0,
        MAX = 255
    };

    // 2.3 Multiple enums can have same value names
    enum class Fruit { APPLE, ORANGE, BANANA };
    enum class Computer { APPLE, IBM, DELL };  // OK: No conflict

    // 2.4 Forward declaration
    enum class NetworkProtocol : int;  // Declaration
    enum class NetworkProtocol : int {  // Definition
        TCP = 6,
        UDP = 17,
        ICMP = 1
    };

    void demonstrateScopedEnums() {
        cout << "\n=== SCOPED ENUMS (enum class) ===\n";

        Color c = Color::RED;
        // int i = c;  // ERROR: No implicit conversion
        int i = static_cast<int>(c);  // Explicit conversion needed
        cout << "Color::RED as int: " << i << endl;

        // No namespace pollution
        Fruit f = Fruit::APPLE;
        Computer comp = Computer::APPLE;  // Same name, different enum

        // Type safety
        // if (c == f) { }  // ERROR: Can't compare different enum types

        // Underlying type control
        Byte b = Byte::MAX;
        cout << "Byte::MAX underlying size: " << sizeof(b) << " bytes\n";
        cout << "Byte::MAX value: " << static_cast<int>(b) << endl;

        // Forward declared enum
        NetworkProtocol proto = NetworkProtocol::TCP;
        cout << "TCP protocol number: " << static_cast<int>(proto) << endl;
    }
}

// ============================================================================
// PART 3: ENUM OPERATIONS AND UTILITIES
// ============================================================================
namespace EnumOperations {
    enum class Status { PENDING, ACTIVE, COMPLETED, FAILED };
    enum class Permissions : uint8_t {
        READ = 0b0001,    // 1
        WRITE = 0b0010,   // 2
        EXECUTE = 0b0100, // 4
        ALL = READ | WRITE | EXECUTE  // 7
    };

    // 3.1 Overloading operators for enums
    Permissions operator|(Permissions a, Permissions b) {
        return static_cast<Permissions>(
            static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }

    Permissions operator&(Permissions a, Permissions b) {
        return static_cast<Permissions>(
            static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }

    Permissions operator~(Permissions a) {
        return static_cast<Permissions>(~static_cast<uint8_t>(a));
    }

    // 3.2 Conversion to string (C++17: no reflection yet)
    const char* statusToString(Status s) {
        switch(s) {
            case Status::PENDING: return "PENDING";
            case Status::ACTIVE: return "ACTIVE";
            case Status::COMPLETED: return "COMPLETED";
            case Status::FAILED: return "FAILED";
            default: return "UNKNOWN";
        }
    }

    void demonstrateOperations() {
        cout << "\n=== ENUM OPERATIONS AND UTILITIES ===\n";

        // Bitmask operations
        Permissions userPerms = Permissions::READ | Permissions::WRITE;
        cout << "User permissions: " << static_cast<int>(userPerms) << endl;

        if ((userPerms & Permissions::WRITE) == Permissions::WRITE) {
            cout << "User has write permission\n";
        }

        // String conversion
        Status s = Status::ACTIVE;
        cout << "Status: " << statusToString(s) << endl;

        // Using bitset with enums
        bitset<8> permBits(static_cast<uint8_t>(Permissions::ALL));
        cout << "ALL permissions as bits: " << permBits << endl;
    }
}

/*
// ============================================================================
// PART 4: ENUM TEMPLATES AND METAPROGRAMMING
// ============================================================================

namespace EnumMeta {
    // 4.1 Type traits for enums
    template<typename T>
    struct is_scoped_enum : false_type {};

    template<typename T>
    requires(is_enum_v<T>)
    struct is_scoped_enum<T> : bool_constant<!is_convertible_v<T, underlying_type_t<T>>> {};

    // 4.2 Getting min/max values of enum
    template<typename E>
    constexpr auto enum_min() -> underlying_type_t<E> {
        using U = underlying_type_t<E>;
        U minVal = numeric_limits<U>::max();

        // In C++17, we can't iterate enum values at compile time
        // This is a placeholder - in real code you'd need to list values
        return minVal;
    }

    // 4.3 Enum as template parameter
    template<auto Value>
    struct EnumValue {
        static constexpr auto value = Value;
        using type = decltype(Value);

        static string to_string() {
            if constexpr (is_same_v<type, BasicEnums::Color>) {
                return "Basic Color enum";
            } else if constexpr (is_same_v<type, ScopedEnums::Color>) {
                return "Scoped Color enum";
            }
            return "Unknown enum type";
        }
    };

    // 4.4 Tag dispatch with enums
    enum class Algorithm { QUICK_SORT, MERGE_SORT, BUBBLE_SORT };

    template<Algorithm Algo>
    struct SortAlgorithm;

    template<>
    struct SortAlgorithm<Algorithm::QUICK_SORT> {
        template<typename It>
        static void sort(It begin, It end) {
            cout << "Using Quick Sort\n";
            sort(begin, end);  // Actual implementation
        }
    };

    template<>
    struct SortAlgorithm<Algorithm::MERGE_SORT> {
        template<typename It>
        static void sort(It begin, It end) {
            cout << "Using Merge Sort\n";
            // Implementation
        }
    };

    void demonstrateMeta() {
        cout << "\n=== ENUM METAPROGRAMMING ===\n";

        // Check if enum is scoped
        cout << "BasicEnums::Color is scoped: "
             << is_scoped_enum<BasicEnums::Color>::value << endl;
        cout << "ScopedEnums::Color is scoped: "
             << is_scoped_enum<ScopedEnums::Color>::value << endl;

        // Enum as template parameter
        EnumValue<BasicEnums::RED> ev;
        cout << "EnumValue type: " << ev.to_string() << endl;

        // Tag dispatch
        vector<int> data = {5, 2, 8, 1, 9};
        SortAlgorithm<Algorithm::QUICK_SORT>::sort(data.begin(), data.end());
    }
}

// ============================================================================
// PART 5: ADVANCED PATTERNS AND USE CASES
// ============================================================================

namespace AdvancedPatterns {
    // 5.1 State Machine with enums
    enum class TrafficLight { RED, YELLOW, GREEN };

    class TrafficSystem {
        TrafficLight current = TrafficLight::RED;

    public:
        void advance() {
            switch(current) {
                case TrafficLight::RED:
                    current = TrafficLight::GREEN;
                    break;
                case TrafficLight::GREEN:
                    current = TrafficLight::YELLOW;
                    break;
                case TrafficLight::YELLOW:
                    current = TrafficLight::RED;
                    break;
            }
            cout << "Traffic light: " << static_cast<int>(current) << endl;
        }

        TrafficLight getState() const { return current; }
    };

    // 5.2 Enum-based Visitor Pattern
    enum class ShapeType { CIRCLE, RECTANGLE, TRIANGLE };

    struct Shape {
        ShapeType type;
        virtual ~Shape() = default;
    };

    struct ShapeVisitor {
        virtual void visitCircle() = 0;
        virtual void visitRectangle() = 0;
        virtual void visitTriangle() = 0;
        virtual ~ShapeVisitor() = default;
    };

    // 5.3 Configuration flags
    enum class ConfigFlags : uint32_t {
        NONE = 0,
        ENABLE_LOGGING = 1 << 0,
        ENABLE_CACHE = 1 << 1,
        ENABLE_SSL = 1 << 2,
        DEBUG_MODE = 1 << 3,
        ALL = ENABLE_LOGGING | ENABLE_CACHE | ENABLE_SSL | DEBUG_MODE
    };

    // 5.4 Enum-based polymorphism (alternative to virtual functions)
    template<ShapeType Type>
    struct ConcreteShape : Shape {
        ConcreteShape() { this->type = Type; }

        double area() const {
            if constexpr (Type == ShapeType::CIRCLE) {
                return 3.14159 * 10 * 10;  // Example
            } else if constexpr (Type == ShapeType::RECTANGLE) {
                return 20 * 30;
            }
            return 0;
        }
    };

    // 5.5 Enum in union for type-safe variant (pre-C++17 std::variant)
    struct Variant {
        enum class Type { INT, DOUBLE, STRING } type;
        union {
            int i;
            double d;
            string s;  // Requires careful lifetime management!
        };

        Variant(int val) : type(Type::INT), i(val) {}
        Variant(double val) : type(Type::DOUBLE), d(val) {}
        Variant(const string& val) : type(Type::STRING) {
            new(&s) string(val);  // Placement new
        }

        ~Variant() {
            if (type == Type::STRING) {
                s.~string();
            }
        }

        // Rule of Five needed here - omitted for brevity
    };

    void demonstrateAdvanced() {
        cout << "\n=== ADVANCED PATTERNS ===\n";

        // State Machine
        TrafficSystem ts;
        ts.advance();  // RED -> GREEN
        ts.advance();  // GREEN -> YELLOW
        ts.advance();  // YELLOW -> RED

        // Configuration flags
        ConfigFlags config = ConfigFlags::ENABLE_LOGGING | ConfigFlags::ENABLE_SSL;
        cout << "Config value: " << static_cast<uint32_t>(config) << endl;

        // Enum-based polymorphism
        ConcreteShape<ShapeType::CIRCLE> circle;
        cout << "Circle area: " << circle.area() << endl;

        // Variant example
        Variant v1(42);
        Variant v2(3.14);
        Variant v3("Hello");

        cout << "Variant types: "
             << static_cast<int>(v1.type) << ", "
             << static_cast<int>(v2.type) << ", "
             << static_cast<int>(v3.type) << endl;
    }
}

// ============================================================================
// PART 6: PITFALLS AND EDGE CASES
// ============================================================================

namespace Pitfalls {
    // 6.1 Implicit conversion pitfalls
    enum OldEnum { VALUE_A, VALUE_B, VALUE_C };

    void functionTakingInt(int x) {
        cout << "Got int: " << x << endl;
    }

    // 6.2 Unscoped enum namespace pollution
    enum FileStatus { OPEN, CLOSED, ERROR };
    enum DoorStatus { OPEN, CLOSED, LOCKED };  // ERROR: OPEN/CLOSED redefined!
    // Solution: Use enum class

    // 6.3 Undefined/Invalid values
    void testInvalidValues() {
        OldEnum e = static_cast<OldEnum>(999);  // Invalid but compiles
        cout << "Invalid enum value: " << e << endl;

        // Switch without default (PITFALL)
        switch(e) {
            case VALUE_A: cout << "A\n"; break;
            case VALUE_B: cout << "B\n"; break;
            case VALUE_C: cout << "C\n"; break;
            // Missing default: undefined behavior for invalid values
        }
    }

    // 6.4 Enum size and alignment issues
    enum SmallEnum : char { A, B, C };  // 1 byte
    enum LargeEnum : long long { D, E, F };  // 8 bytes

    struct BadPadding {
        SmallEnum e1;  // 1 byte
        int i;         // 4 bytes
        // 3 bytes padding!
    };

    struct GoodLayout {
        int i;         // 4 bytes
        SmallEnum e1;  // 1 byte
        // Less padding
    };

    // 6.5 Forward declaration differences
    enum class ForwardScoped : int;  // OK
    // enum ForwardUnscoped : int;  // ERROR in C++17: unscoped must be defined

    // 6.6 Overloading with enums
    void process(int x) { cout << "process(int)\n"; }
    void process(OldEnum x) { cout << "process(OldEnum)\n"; }

    // 6.7 Template specialization with enums
    template<typename T>
    struct Traits {};

    template<>
    struct Traits<OldEnum> {
        static constexpr bool is_enum = true;
    };

    // 6.8 Enum in boolean context (PITFALL)
    void testBooleanContext() {
        OldEnum e = VALUE_A;
        if (e) {  // OK but dangerous: compares to 0
            cout << "e is truthy (not zero)\n";
        }

        // Better: explicit comparison
        if (e != VALUE_A) {
            cout << "e is not VALUE_A\n";
        }
    }

    // 6.9 Array indexing with enums
    const char* colors[] = { "Red", "Green", "Blue" };

    void testArrayIndexing() {
        // Unsafe if enum values don't match array indices
        cout << colors[VALUE_B] << endl;  // "Green" - but dangerous!

        // Safer: validate range
        if (VALUE_B >= 0 && VALUE_B < 3) {
            cout << colors[VALUE_B] << endl;
        }
    }

    void demonstratePitfalls() {
        cout << "\n=== PITFALLS AND EDGE CASES ===\n";

        // Implicit conversion
        functionTakingInt(VALUE_A);  // Implicitly converts to int

        // Invalid values
        testInvalidValues();

        // Size issues
        cout << "SmallEnum size: " << sizeof(SmallEnum) << " bytes\n";
        cout << "LargeEnum size: " << sizeof(LargeEnum) << " bytes\n";
        cout << "BadPadding size: " << sizeof(BadPadding) << " bytes\n";
        cout << "GoodLayout size: " << sizeof(GoodLayout) << " bytes\n";

        // Overloading
        process(42);          // Calls process(int)
        process(VALUE_A);     // Calls process(OldEnum) - exact match

        // Boolean context
        testBooleanContext();

        // Array indexing
        testArrayIndexing();
    }
}

// ============================================================================
// PART 7: BEST PRACTICES AND C++17 SPECIFICS
// ============================================================================

namespace BestPractices {
    // 7.1 Always specify underlying type for forward declarations
    enum class LogLevel : int;  // Good
    // enum class LogLevel;      // Bad in C++17 (defaults to int, but be explicit)

    // 7.2 Use constexpr with enums where possible
    enum class ConstexprEnum : int {
        VALUE = 42
    };

    constexpr int computeSomething(ConstexprEnum e) {
        return static_cast<int>(e) * 2;
    }

    // 7.3 Structured bindings with enums (C++17)
    enum class PointComponent { X, Y, Z };

    tuple<int, int, int> getPoint() {
        return {10, 20, 30};
    }

    void structuredBindingDemo() {
        auto [x, y, z] = getPoint();
        cout << "Point: " << x << ", " << y << ", " << z << endl;
    }

    // 7.4 if constexpr with enums (C++17)
    template<typename T>
    void processValue(T value) {
        if constexpr (is_enum_v<T>) {
            cout << "Processing enum value\n";
        } else {
            cout << "Processing non-enum value\n";
        }
    }

    // 7.5 Use std::underlying_type_t
    template<typename Enum>
    constexpr auto to_underlying(Enum e) noexcept {
        return static_cast<underlying_type_t<Enum>>(e);
    }

    // 7.6 Enum sets using bitset
    enum class Features { A, B, C, D, E, COUNT };

    class FeatureSet {
        bitset<to_underlying(Features::COUNT)> bits;

    public:
        void enable(Features f) {
            bits.set(to_underlying(f));
        }

        void disable(Features f) {
            bits.reset(to_underlying(f));
        }

        bool isEnabled(Features f) const {
            return bits.test(to_underlying(f));
        }
    };

    // 7.7 [[nodiscard]] with enums (C++17)
    enum class ErrorCode { SUCCESS, FAILURE, RETRY };

    [[nodiscard]] ErrorCode riskyOperation() {
        return ErrorCode::SUCCESS;
    }

    void demonstrateBestPractices() {
        cout << "\n=== BEST PRACTICES (C++17) ===\n";

        // Constexpr enums
        constexpr int result = computeSomething(ConstexprEnum::VALUE);
        cout << "Constexpr result: " << result << endl;

        // Structured bindings
        structuredBindingDemo();

        // if constexpr
        processValue(42);
        processValue(BasicEnums::RED);

        // to_underlying utility
        auto val = to_underlying(ScopedEnums::Color::GREEN);
        cout << "Underlying value: " << val << endl;

        // Feature set
        FeatureSet features;
        features.enable(Features::A);
        features.enable(Features::C);
        cout << "Feature A enabled: " << features.isEnabled(Features::A) << endl;
        cout << "Feature B enabled: " << features.isEnabled(Features::B) << endl;

        // [[nodiscard]]
        auto err = riskyOperation();  // Good: result used
        // riskyOperation();  // Warning: ignoring nodiscard result
    }
}

// ============================================================================
// PART 8: REAL-WORLD EXAMPLE COMPLETE APPLICATION
// ============================================================================

namespace RealWorldExample {
    // 8.1 Database ORM-like enum handling
    enum class UserRole : uint8_t {
        GUEST = 0,
        USER = 1,
        MODERATOR = 2,
        ADMIN = 3,
        SUPER_ADMIN = 4
    };

    enum class AccountStatus : uint8_t {
        PENDING = 0,
        ACTIVE = 1,
        SUSPENDED = 2,
        DELETED = 3
    };

    class User {
    private:
        string username;
        UserRole role;
        AccountStatus status;

    public:
        User(string name, UserRole r = UserRole::USER)
            : username(move(name)), role(r), status(AccountStatus::PENDING) {}

        // 8.2 Safe promotion/demotion
        bool promote() {
            if (role == UserRole::SUPER_ADMIN) return false;

            role = static_cast<UserRole>(static_cast<uint8_t>(role) + 1);
            return true;
        }

        // 8.3 Validation
        static bool isValidRole(uint8_t value) {
            return value <= static_cast<uint8_t>(UserRole::SUPER_ADMIN);
        }

        // 8.4 Serialization
        map<string, int> toMap() const {
            return {
                {"role", static_cast<int>(role)},
                {"status", static_cast<int>(status)}
            };
        }

        static User fromMap(const map<string, int>& data) {
            User user("Unknown");
            if (auto it = data.find("role"); it != data.end()) {
                if (isValidRole(it->second)) {
                    user.role = static_cast<UserRole>(it->second);
                }
            }
            return user;
        }

        // 8.5 String conversion with caching
        string roleToString() const {
            static const map<UserRole, string> roleNames = {
                {UserRole::GUEST, "Guest"},
                {UserRole::USER, "User"},
                {UserRole::MODERATOR, "Moderator"},
                {UserRole::ADMIN, "Administrator"},
                {UserRole::SUPER_ADMIN, "Super Administrator"}
            };

            if (auto it = roleNames.find(role); it != roleNames.end()) {
                return it->second;
            }
            return "Unknown";
        }

        // 8.6 Comparison operators
        bool operator==(const User& other) const {
            return role == other.role && status == other.status;
        }

        bool operator!=(const User& other) const {
            return !(*this == other);
        }

        // 8.7 Permission checking
        bool hasPermission(const string& permission) const {
            switch(role) {
                case UserRole::SUPER_ADMIN:
                    return true;  // All permissions
                case UserRole::ADMIN:
                    return permission != "system_shutdown";
                case UserRole::MODERATOR:
                    return permission == "edit_posts" ||
                           permission == "delete_comments";
                case UserRole::USER:
                    return permission == "create_posts";
                case UserRole::GUEST:
                    return permission == "view_content";
                default:
                    return false;
            }
        }

        void display() const {
            cout << "User: " << username
                 << ", Role: " << roleToString()
                 << " (" << static_cast<int>(role) << ")"
                 << ", Status: " << static_cast<int>(status) << endl;
        }
    };

    // 8.8 Enum-based factory
    enum class ShapeFactoryType { CIRCLE, SQUARE, TRIANGLE };

    class ShapeFactory {
    public:
        unique_ptr<Shape> create(ShapeFactoryType type) {
            switch(type) {
                case ShapeFactoryType::CIRCLE:
                    return make_unique<ConcreteCircle>();
                case ShapeFactoryType::SQUARE:
                    return make_unique<ConcreteSquare>();
                case ShapeFactoryType::TRIANGLE:
                    return make_unique<ConcreteTriangle>();
                default:
                    throw invalid_argument("Unknown shape type");
            }
        }

    private:
        struct Shape { virtual ~Shape() = default; virtual void draw() = 0; };
        struct ConcreteCircle : Shape { void draw() override { cout << "○\n"; } };
        struct ConcreteSquare : Shape { void draw() override { cout << "□\n"; } };
        struct ConcreteTriangle : Shape { void draw() override { cout << "△\n"; } };
    };

    void demonstrateRealWorld() {
        cout << "\n=== REAL-WORLD EXAMPLE ===\n";

        // User management system
        User admin("Alice", UserRole::ADMIN);
        User user("Bob", UserRole::USER);

        admin.display();
        user.display();

        // Permission checking
        cout << "Can admin edit posts? "
             << admin.hasPermission("edit_posts") << endl;
        cout << "Can user edit posts? "
             << user.hasPermission("edit_posts") << endl;

        // Promotion
        cout << "Promoting user... ";
        if (user.promote()) {
            user.display();
        }

        // Serialization/Deserialization
        auto data = admin.toMap();
        User restored = User::fromMap(data);
        cout << "Restored user role: " << restored.roleToString() << endl;

        // Factory pattern
        ShapeFactory factory;
        auto shape = factory.create(ShapeFactoryType::CIRCLE);
        shape->draw();

        // Validation
        cout << "Is 99 a valid role? "
             << User::isValidRole(99) << endl;
        cout << "Is 3 a valid role? "
             << User::isValidRole(3) << endl;
    }
}

// ============================================================================
// MAIN TEST FUNCTION
// ============================================================================
*/

int main() {
    cout << "========== COMPREHENSIVE C++17 ENUMS STUDY ==========\n";

    // Run all demonstrations
    BasicEnums::demonstrateBasicEnums();
    ScopedEnums::demonstrateScopedEnums();
    EnumOperations::demonstrateOperations();
    //EnumMeta::demonstrateMeta();
    //AdvancedPatterns::demonstrateAdvanced();
    //Pitfalls::demonstratePitfalls();
    //BestPractices::demonstrateBestPractices();
    //RealWorldExample::demonstrateRealWorld();

    cout << "\n========== STUDY SUMMARY ==========\n";
    cout << "Key Takeaways:\n";
    cout << "1. Prefer 'enum class' over plain 'enum' for type safety\n";
    cout << "2. Always specify underlying type for forward declarations\n";
    cout << "3. Use explicit casts when converting to/from underlying type\n";
    cout << "4. Validate enum values when receiving from external sources\n";
    cout << "5. Consider bitmask enums for flag combinations\n";
    cout << "6. Use constexpr with enums for compile-time computations\n";
    cout << "7. Be aware of size and alignment implications\n";
    cout << "8. Implement proper string conversions for debugging/logging\n";
    cout << "9. Use [[nodiscard]] for enum-returning functions in C++17\n";
    cout << "10. Leverage if constexpr for enum type handling in templates\n";

    return 0;
}
