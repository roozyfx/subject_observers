#pragma once

#include <array>
#include <concepts>
#include <random>
#include <type_traits>

#include "vectormath.h"

using std::uniform_real_distribution;
template <typename T>
concept AcceptableType = fx::is_tuple<T>::value or std::floating_point<T>;

class RNG {
 private:
  // Standard mersenne_twister_engine seeded with rd()
  std::mt19937_64 gen_;

 public:
  RNG() : gen_(std::random_device{}()) {}
  /**
   * @brief Updates @param value to a uniformly distributed of type T in the
   * range of low and high. T can be an fx::Vector, fx::Point or an
   * arithmetic type. For other types, this function does nothing,
   * @param value : variable to be populated
   * @param low : low range.
   * @param high: high range
   * For Vector and Point, value will be a random Vector/Point between (l.x to
   * h.x), (l.y to h.y), (l.z to h.z), where l and h are the lower or higher
   * number in that dimension, respectively.
   */
  template <AcceptableType T>
  void GenerateUniformRandom(T& value, const T& low, const T& high) {
    using std::min, std::max;
    // T = Point<double> e.g.
    if constexpr (fx::is_tuple<T>::value) {
      constexpr size_t n{3};
      // fix low-high range if needed
      T l(min(low.x, high.x), min(low.y, high.y), min(low.z, high.z));
      T h(max(low.x, high.x), max(low.y, high.y), max(low.z, high.z));

      // std::array<uniform_real_distribution<decltype(l[0])>, n> dis;
      std::array<uniform_real_distribution<double>, n> dis;
      dis[0] = uniform_real_distribution<double>(l[0], h[0]);
      dis[1] = uniform_real_distribution<double>(l[1], h[1]);
      dis[2] = uniform_real_distribution<double>(l[2], h[2]);
      //   for (size_t i = 0; i < n; ++i)
      // dis[i] = uniform_real_distribution<decltype(l[0])>(l[i], h[i]);
      // dis[i] = uniform_real_distribution<decltype(l[0])>(l[i], h[i]);

      // Transform 'uint' gen_ to T using dis_i
      value = T(dis[0](gen_), dis[1](gen_), dis[2](gen_));
    } else if constexpr (std::is_floating_point_v<T>) {  // simple floating point number
      // if low and high are given in the wrong order, swap them
      uniform_real_distribution<T> dis(min(low, high), max(low, high));
      value = dis(gen_);
    }
    // Leave value unchanged if it's of an unsupported type
  }
};
