#include "sim/sensor.h"

#include <algorithm>
#include <concepts>
#include <print>

#include "utils/config.h"
#include "utils/vectormath.h"

template <std::floating_point T>
fx::Vector<T> fx::Sensor<T>::GenerateNoise() {
  T measurement_noise{
      static_cast<T>(Config<T>::GetValues().at("measurement_noise"))};
  // Add (white) noise according to the measurement_noise in configuration
  T noise_x{};
  rng_.GenerateUniformRandom(noise_x, -measurement_noise, measurement_noise);
  T noise_y{};
  rng_.GenerateUniformRandom(noise_y, -measurement_noise, measurement_noise);
  T noise_z{};
  rng_.GenerateUniformRandom(noise_z, -measurement_noise, measurement_noise);

  return fx::Vector<T>(noise_x, noise_y, noise_z);
}

template <std::floating_point T>
void fx::Sensor<T>::AddMeasurementNoise(const Vector<T>& noise) {
  T pos_x{std::clamp(static_cast<T>(measurement_.position_m().x()) + noise.x,
                     T(Config<T>::GetValues().at("x_min")),
                     T(Config<T>::GetValues().at("x_max")))};
  T pos_y{std::clamp(static_cast<T>(measurement_.position_m().y()) + noise.y,
                     T(Config<T>::GetValues().at("y_min")),
                     T(Config<T>::GetValues().at("y_max")))};
  T pos_z{std::clamp(static_cast<T>(measurement_.position_m().z()) + noise.z,
                     T(Config<T>::GetValues().at("z_min")),
                     T(Config<T>::GetValues().at("z_max")))};

  SetPosition_M(pos_x, pos_y, pos_z);
}

template <std::floating_point T>
void fx::Sensor<T>::Measure(const Point<T>& p) {
  auto noise{GenerateNoise()};
  lock_guard l(mutex_);
  measurement_.set_timestamp_ms(CreateTimestamp());

  // Setting the position and then adding the noise, which in turn sets the
  // position again may seems redundant, but I find it more realistic
  SetPosition_M(p.x, p.y, p.z);
  AddMeasurementNoise(noise);
}

template <std::floating_point T>
void fx::Sensor<T>::PrintData() const {
  lock_guard l(mutex_);
  std::println("Sensor_{}:\t timestamp: {}, ({}, {}, {})",
               measurement_.id(), measurement_.timestamp_ms(),
               measurement_.position_m().x(), measurement_.position_m().y(),
               measurement_.position_m().z());
}

template class fx::Sensor<float>;
template class fx::Sensor<double>;
