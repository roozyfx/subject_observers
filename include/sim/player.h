/* *****************************************************************************
 * A simplistic simulation of a Player, i.e. the concrete subject in the
 * simulation. It is templated with the restriction of a floating point type.
 * The player has a location, indicated as a Point<T>, can randomly move in any
 * direction in 2-D space and within a field with dimensions set in the config
 * file in x and y directions. The player's speed is updated with a given speed
 * and sampling frequency, also set in the config file. The player notifies its
 * observers (i.e. the concrete observers in the simulation) upon each update of
 * its location.
 * *****************************************************************************
 */
#pragma once
#include <concepts>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "ifc_observer.h"
#include "utils/rng.h"
#include "utils/vectormath.h"

namespace fx {
using lock_guard = std::lock_guard<std::mutex>;

template <std::floating_point T = float>
class Player : public ISubject<Point<T>, T> {
 public:
  using IObs_shpt = std::shared_ptr<IObserver<Point<T>, T>>;

  Player();
  ~Player() override = default;

  // As an alternative to the random location during construction, set player's
  // initial location
  inline void SetInitialLocation(const Point<T>& p) { p_ = p; }

  void Update();

  inline void Register(IObs_shpt observer) final {
    lock_guard l(mutex_);
    observers_.push_back(observer);
  }

  inline void Unregister(IObs_shpt observer) final {
    lock_guard l(mutex_);
    observers_.remove(observer);
  }

 private:
  inline void Notify() const final {
    std::list<IObs_shpt> snapshot;
    Point<T> p;
    {
      lock_guard l(mutex_);
      snapshot = observers_;
      p = p_;
    }
    for (auto& obs : snapshot)
      if (obs) obs->Measure(p);
  }

  mutable std::mutex mutex_;
  std::list<IObs_shpt> observers_;
  // Player's position
  Point<T> p_;
  Vector<T> v_;
  // Random Number Generator for Vector
  RNG rng_;
  std::unordered_map<std::string_view, T> configurations_;
};

}  // namespace fx
