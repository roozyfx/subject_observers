#include "utils/rng.h"

#include <gtest/gtest.h>

#include <cmath>
#include <type_traits>

TEST(RNGConceptTest, AcceptableTypeTrait) {
  // Scalar floating point should be acceptable
  static_assert(AcceptableType<double>);
  static_assert(AcceptableType<float>);

  static_assert(AcceptableType<fx::Point<float>>);
  static_assert(AcceptableType<fx::Point<double>>);
  static_assert(AcceptableType<fx::Point<long double>>);

  static_assert(AcceptableType<fx::Vector<float>>);
  static_assert(AcceptableType<fx::Vector<double>>);
  static_assert(AcceptableType<fx::Vector<long double>>);

  static_assert(!AcceptableType<int>);
}

TEST(RNGFloatingPointTest, GeneralCorrectness) {
  RNG rng;

  double low{-2.4};
  double high{3.1415};
  double val{0.0};

  for (int i = 0; i < 100; ++i) {
    rng.GenerateUniformRandom(val, low, high);
    EXPECT_GE(val, low);
    EXPECT_LE(val, high);
  }
}

TEST(RNGFloatingPointTest, SwappedBoundaryCorrectness) {
  RNG rng;

  double high{-2.4};
  double low{3.1415};
  double val{0.0};

  double l{std::min(low, high)};
  double h{std::max(low, high)};
  for (int i = 0; i < 1000; ++i) {
    rng.GenerateUniformRandom(val, low, high);
    EXPECT_GE(val, l);
    EXPECT_LE(val, h);
  }
}

TEST(RNGFloatingPointTest, EqualLowHighCorrectness) {
  RNG rng;

  double low{3.1415};
  double high{3.1415};
  double val{0.0};

  for (int i = 0; i < 10; ++i) {
    rng.GenerateUniformRandom(val, low, high);
    EXPECT_DOUBLE_EQ(val, low);
  }
}

template <typename T>
  requires fx::is_tuple<T>::value
static void ExpectTupleInRange(const T& v, const T& low, const T& high) {
  auto check = [](double val, double lo, double hi) {
    double l{std::min(lo, hi)};
    double h{std::max(lo, hi)};
    EXPECT_GE(val, l);
    EXPECT_LE(val, h);
  };
  check(v.x, low.x, high.x);
  check(v.y, low.y, high.y);
  check(v.z, low.z, high.z);
}

TEST(RNGVectorTest, GeneralExpectedCorrectness) {
  RNG rng;

  fx::Vector<float> low{0.0, 0.0, 1.2};
  fx::Vector<float> high{100.0, 100.0, 6.6};

  fx::Vector<float> value{};
  for (int i = 0; i < 1000; ++i) {
    rng.GenerateUniformRandom(value, low, high);
    ExpectTupleInRange(value, low, high);
  }
}

TEST(RNGVectorTest, MixedupBoundariesCorrectness) {
  RNG rng;

  fx::Vector<double> bad_low{100.0, 100.0, 1.2};
  fx::Vector<double> bad_high{0.0, 0.0, 6.6};

  fx::Vector<double> val{};
  for (int i = 0; i < 1000; ++i) {
    rng.GenerateUniformRandom(val, bad_low, bad_high);
    ExpectTupleInRange(val, bad_low, bad_high);
  }
}

TEST(RNGVectorTest, EqualBoundariesCorrectness) {
  RNG rng;

  fx::Vector<float> no_change{1.0, 2.0, 3.0};
  fx::Vector<float> low = no_change;
  fx::Vector<float> high = no_change;

  fx::Vector<float> v{};
  for (int i = 0; i < 10; ++i) {
    rng.GenerateUniformRandom(v, low, high);
    EXPECT_DOUBLE_EQ(v.x, no_change.x);
    EXPECT_DOUBLE_EQ(v.y, no_change.y);
    EXPECT_DOUBLE_EQ(v.z, no_change.z);
  }
}

TEST(RNGPointTest, GeneralExpectedCorrectness) {
  RNG rng;

  fx::Point<float> low{0.0, 0.0, 1.2};
  fx::Point<float> high{100.0, 100.0, 6.6};

  fx::Point<float> value{};
  for (int i = 0; i < 1000; ++i) {
    rng.GenerateUniformRandom(value, low, high);
    ExpectTupleInRange(value, low, high);
  }
}