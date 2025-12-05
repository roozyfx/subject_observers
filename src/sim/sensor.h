/* ***************************************************************************
 * A Sensor class, simulating a sensor, i.e. the concrete observer in the
 * simulation. It observes the subject, and adds uniform random noise in the range
 * specified in the configuration file, to simulate the measurement noise.
 * Each sensor also adds its ID and the measurement timestamp (in ms).
 * All packed as Position structure defined in the protobuffer file.
 * ***************************************************************************
 */
#pragma once

#include <algorithm>
#include <chrono>
#include <concepts>
#include <mutex>
#include <print>

#include "ifc_observer.h"
#include "position.pb.h"
#include "utils/config.h"
#include "utils/rng.h"
#include "utils/vectormath.h"

namespace fx {
template <std::floating_point T = float>
class Sensor : public IObserver<Point<T>, T> {
 public:
  Sensor() : measurement_{}, rng_{}, mutex_{} {}
  ~Sensor() = default;

  inline void SetID(const uint64_t& id) { measurement_.set_id(id); }

  /*
   * Measures, i.e. simulates measurement, of the current location of the
   * subject, recording the timestamp (ms) of the measurement and adds a uniform
   * random noise to simulate the measurement noise/imprecisions.
   */
  void Measure(const Point<T>& p) final;

  inline void ReadMeasurement(Position& probe) const {
    std::lock_guard l(mutex_);
    probe = measurement_;
  }

  void PrintData() const;

 private:
  /* Add measurement noise*/
  void AddMeasurementNoise();

  inline void SetPosition_M(const T& posx, const T& posy, const T& posz) {
    // For thread-safety
    std::lock_guard l(mutex_);
    measurement_.mutable_position_m()->set_x(posx);
    measurement_.mutable_position_m()->set_y(posy);
    measurement_.mutable_position_m()->set_z(posz);
  }

  inline uint64_t CreateTimestamp() const {
    using namespace std::chrono;
    const auto now{system_clock::now().time_since_epoch()};
    return static_cast<uint64_t>(duration_cast<milliseconds>(now).count());
  }

  Position measurement_;
  RNG rng_;
  mutable std::mutex mutex_;
};
}  // namespace fx