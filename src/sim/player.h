/* *****************************************************************************
 * A simplistic simulation of a Player, i.e. the concrete subject in the
 * simulation. It is templated with the restiction of a floating point type.
 * The player has a location, indicated as a Point<T>, can randomly move in any
 * direction in 2-D space and within a field with dimensions set in the config file
 * in x and y directions. The player's speed is updated with a given speed and sampling
 * frequency, also set in the config file. The player notifies its observers
 * (i.e. the concrete observers in the simulation) upon each update of its location.
 * *****************************************************************************
 */
#pragma once
#include <cmath>
#include <concepts>
#include <limits>
#include <list>
#include <memory>
#include <mutex>

#include "ifc_observer.h"
#include "position.pb.h"
#include "utils/config.h"
#include "utils/rng.h"
#include "utils/vectormath.h"

namespace fx {
template <std::floating_point T = float>
class Player : public ISubject<Point<T>, T> {
 public:
  using spt_iobs = std::shared_ptr<IObserver<Point<T>, T>>;

  Player();
  ~Player() = default;

  // As an alternative to the random location during construction, set player's
  // initial location
  inline void SetInitialLocation(const Point<T>& p) { p_ = p; }

  void Update();

  inline void Register(spt_iobs observer) final {
    std::lock_guard l(mutex_);
    observers_.push_back(observer);
  }

  inline void Unregister(spt_iobs observer) final {
    std::lock_guard l(mutex_);
    observers_.remove(observer);
  }

 private:
  inline void Notify() const final {
    std::lock_guard l(mutex_);
    for (auto& obs : observers_) obs->Measure(p_);
  }

  mutable std::mutex mutex_;
  std::list<spt_iobs> observers_;
  // Player's position
  Point<T> p_;
  Vector<T> v_;
  // Random Number Generator for Vector
  RNG rng_;
  std::unordered_map<std::string_view, T> configurations_;
};

}  // namespace fx
