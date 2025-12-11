#include <iostream>

// 1. simple function
constexpr int square(int x) {
  return x * x;
}

static_assert(square(5) == 25, "square(5) must equal 25");

// 2. recursive
constexpr int factorial_recursive(int n) {
  return (n <= 1) ? 1 : n * factorial_recursive(n-1);
}

static_assert(factorial_recursive(5) == 120, "fact(5) must be 120 ");

// 3. we can use loops
constexpr int factorial_iter(int n) {
  int result = 1;
  for (int i = 2; i<=n ; ++i) result *= i;
  return result;
}

static_assert(factorial_iter(5) == 120, "fact(5) must be 120 ");

// 4. constexpr objects
struct Vec2 {
  int x, y;
  constexpr Vec2(int x_, int y_) : x(x_), y(y_) {}
  constexpr int dot(const Vec2& o) const { return x * o.x + y * o.y;}
};

constexpr Vec2 A{2, 3};
constexpr Vec2 B{4, 1};

static_assert(A.dot(B) == 11, "A.dot(B) must be 11");

// 5. Template + if constexpr: compile-time branch selection
template <typename T>
constexpr T id_or_abs(T v) {
    if constexpr (std::is_signed_v<T>) {
        return (v < 0) ? -v : v;   // signed: return absolute value
    } else {
        return v;                 // unsigned: identity
    }
}
static_assert(id_or_abs(-7) == 7, "id_or_abs works for signed types");
static_assert(id_or_abs(9u) == 9u, "id_or_abs works for unsigned types");

int runtime_input() { return 9; }

int main(){
  constexpr int compile_time_val = square(8);      // computed at compile time
  int main_input = runtime_input();
  int runtime_val = square(main_input);
  std::cout << "81 ? " << runtime_val << std::endl;
  return 0;
}
