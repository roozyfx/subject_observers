#pragma once
#include <cassert>
#include <cmath>
#include <concepts>
#include <limits>
#include <ostream>

namespace fx {
// TODO Add checks for validity of x, y, z (e.g. if NaN)
template <template <typename> class Child, std::floating_point T = float>
struct Tuple {
  T x{}, y{}, z{};

  Tuple() = default;
  Tuple(const T& x, const T& y, const T& z) : x{x}, y{y}, z{z} {}
  virtual ~Tuple() = default;

  bool operator==(const Child<T>&) const = delete;

  // So we can compare Child<float> and Child<double> e.g.
  template <typename TT>
  bool IsAlmostEqual(
      const Child<TT>& rhs,
      T epsilon = std::numeric_limits<T>::epsilon()) const noexcept {
    using std::abs;
    return abs(x - rhs.x) < epsilon && abs(y - rhs.y) < epsilon &&
           abs(z - rhs.z) < epsilon;
  }

  bool operator!=(const Child<T>& q) { return !IsAlmostEqual(q); }

  // So that we can have Children of different types(Point/Vector)
  template <typename TT>
  inline Child<T>& operator+=(const Child<TT>& ch) {
    x += ch.x;
    y += ch.y;
    z += ch.z;
    return static_cast<Child<T>&>(*this);
  }

  template <typename K>
  auto operator*(K k) const -> Child<decltype(T{} * K{})> {
    return {k * x, k * y, k * z};
  }

  template <typename K>
  inline Child<T>& operator*=(const K k) {
    x *= k;
    y *= k;
    z *= k;
    return static_cast<Child<T>&>(*this);
  }

  inline T operator[](const size_t i) const noexcept {
    assert(i < 3);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  inline T& operator[](const size_t i) noexcept {
    assert(i < 3);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  friend std::ostream& operator<<(std::ostream& os, const Child<T>& v) {
    os << v.x << ", " << v.y << ", " << v.z;
    return os;
  }
};

template <template <class> class C, typename U, typename K>
inline auto operator*(const K& k, const Tuple<C, U>& t) {
  return t * k;
}
template <std::floating_point T = float>
struct Vector : public Tuple<Vector, T> {
  using Tuple<Vector, T>::x;
  using Tuple<Vector, T>::y;
  using Tuple<Vector, T>::z;

  Vector() = default;
  Vector(const T x, const T y, const T z) : Tuple<Vector, T>(x, y, z) {}
  template <typename V>
  explicit Vector(const Vector<V>& v)
      : Tuple<Vector, T>(T(v.x), T(v.y), T(v.z)) {}

  ~Vector() = default;

  template <typename U>
  friend inline Vector<T> operator+(const Vector<T>& v, const Vector<U> u) {
    return {v.x + u.x, v.y + u.y, v.z + u.z};
  }
};

template <std::floating_point T = float>
struct Point : public Tuple<Point, T> {
  using Tuple<Point, T>::x;
  using Tuple<Point, T>::y;
  using Tuple<Point, T>::z;

  Point() { x = y = z = 0; }
  Point(const T x, const T y, const T z) : Tuple<Point, T>(x, y, z) {}

  template <typename P>
  explicit Point(const Point<P>& p) : Tuple<Point, T>(T(p.x), T(p.y), T(p.z)) {}

  template <typename V>
  Point<T>& operator+=(const Vector<V>& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
};

template <typename>
struct is_tuple : std::false_type {};

// For Tuple itself
template <template <typename> class Child, std::floating_point T>
struct is_tuple<Tuple<Child, T>> : std::true_type {};

// Specialization for Vector and Point
template <template <typename> class Child, std::floating_point T>
struct is_tuple<Child<T>>
    : std::bool_constant<std::is_base_of_v<Tuple<Child, T>, Child<T>>> {};

// TODO The following works with is_tuple_v<Vector, T>, but not for <Vector<T>>.
// TODO Solve it
//  mimic STL approach of the helper
template <template <typename> class Child, std::floating_point T>
inline constexpr bool is_tuple_v = is_tuple<Child<T>>::value;

}  // namespace fx