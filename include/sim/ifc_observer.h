/* A Templated Interface to Subject and Observer in the observer pattern.
 */
#pragma once
#include <memory>

namespace fx {
/* Interface to the Object in the Observer pattern */
template <typename Data, std::floating_point T = float>
class IObserver {
 public:
  virtual ~IObserver() = default;

  virtual void Measure(const Data& p) = 0;
};

/* Interface to the Subject in the Observer pattern */
template <typename Data, std::floating_point T = float>
class ISubject {
 public:
  virtual ~ISubject() = default;
  virtual void Register(std::shared_ptr<IObserver<Data, T>> observer) = 0;
  virtual void Unregister(std::shared_ptr<IObserver<Data, T>> observer) = 0;
  virtual void Notify() const = 0;
};

}  // namespace fx