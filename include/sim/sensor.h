/* ***************************************************************************
 * A Sensor class, simulating a sensor, i.e. the concrete observer in the
 * simulation. It observes the subject, and adds uniform random noise in the
 * range specified in the configuration file, to simulate the measurement noise.
 * Each sensor also adds its ID and the measurement timestamp, in ms.
 * All packed as Position structure defined in the protobuffer file.
 * ***************************************************************************
 */
#pragma once

#include <chrono>
#include <concepts>
#include <mutex>

#include "ifc_observer.h"
#include "position.pb.h"
#include "utils/rng.h"
#include "utils/vectormath.h"

namespace fx {
using lock_guard = std::lock_guard<std::mutex>;

template <std::floating_point T = float>
class Sensor : public IObserver<Point<T>, T> {
 public:
  Sensor() : measurement_{}, rng_{}, mutex_{} {}
  ~Sensor() override = default;

  inline void SetID(const uint64_t& id) {
    lock_guard l(mutex_);
    measurement_.set_id(id);
  }

  /*
   * Measures, i.e. simulates measurement, of the current location of the
   * subject, recording the timestamp (ms) of the measurement and adds a uniform
   * random noise to simulate the measurement noise/imprecisions.
   */
  void Measure(const Point<T>& p) final;

  inline void ReadMeasurement(Position& probe) const {
    lock_guard l(mutex_);
    probe = measurement_;
  }

  void PrintData() const;

 private:
  Vector<T> GenerateNoise();
  /* Add measurement noise*/
  void AddMeasurementNoise(const Vector<T>& noise);

  inline void SetPosition_M(const T& pos_x, const T& pos_y, const T& pos_z) {
    // For thread-safety
    measurement_.mutable_position_m()->set_x(pos_x);
    measurement_.mutable_position_m()->set_y(pos_y);
    measurement_.mutable_position_m()->set_z(pos_z);
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
