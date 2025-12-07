#include <algorithm>
#include <print>

#include "sim/sensor.h"

template <std::floating_point T>
void fx::Sensor<T>::AddMeasurementNoise() {
  T measurement_noise = T(Config<T>::GetValues().at("measurement_noise"));
  T posx{}, posy{}, posz{};
  {
    std::lock_guard l(mutex_);
    // Add (white) noise according to the measurement_noise in configuration
    T noise_x{};
    rng_.GenerateUniformRandom(noise_x, -measurement_noise, measurement_noise);
    T noise_y{};
    rng_.GenerateUniformRandom(noise_y, -measurement_noise, measurement_noise);
    T noise_z{};
    rng_.GenerateUniformRandom(noise_z, -measurement_noise, measurement_noise);
    posx = std::clamp(measurement_.position_m().x() + noise_x, T(Config<T>::GetValues().at("x_min")),
                      T(Config<T>::GetValues().at("x_max")));
    posy = std::clamp(measurement_.position_m().y() + noise_y, T(Config<T>::GetValues().at("y_min")),
                      T(Config<T>::GetValues().at("y_max")));
    posz = std::clamp(measurement_.position_m().z() + noise_z, T(Config<T>::GetValues().at("z_min")),
                      T(Config<T>::GetValues().at("z_max")));
  }

  SetPosition_M(posx, posy, posz);
}

template <std::floating_point T>
void fx::Sensor<T>::Measure(const Point<T>& p) {
  measurement_.set_timestamp_ms(CreateTimestamp());

  // Setting the position and then adding the noise, which in turn sets the
  // position again may seems redundant, but I find it more realistic
  SetPosition_M(p.x, p.y, p.z);
  AddMeasurementNoise();
}

template <std::floating_point T>
void fx::Sensor<T>::PrintData() const {
  std::lock_guard l(mutex_);
  std::println("Sensor_{}:\t timestamp: {}, ({}, {}, {})",
               measurement_.id(), measurement_.timestamp_ms(),
               measurement_.position_m().x(), measurement_.position_m().y(),
               measurement_.position_m().z());
}

template class fx::Sensor<float>;
template class fx::Sensor<double>;