#pragma once
#include <concepts>
#include <exception>
#include <memory>
#include <stop_token>
#include <thread>
#include <vector>

#include "net_publisher.h"
#include "player.h"
#include "sensor.h"
#include "utils/config.h"

namespace fx {

template <std::floating_point T>
class Simulation {
  void RunSimulation(NetPublisher& net_publisher, std::stop_token stoken);
  void JoinWorker() noexcept;

  // Player, i.e. subject of the observation
  Player<T>& player_;
  // array of measuring sensors, i.e. observers
  std::vector<std::shared_ptr<Sensor<T>>> sensors_;

  std::exception_ptr worker_exception_{};
  // Keep this the last data member, so it is destroyed first before all other
  // members
  std::jthread worker_;

 public:
  explicit Simulation(
      Player<T>& player,
      const size_t num_sensors = Config<T>::GetValues().at("number_sensors"));
  Simulation(const Simulation&) = delete;
  Simulation& operator=(const Simulation&) = delete;

  Simulation(Simulation&&) = delete;
  Simulation& operator=(Simulation&&) = delete;

  ~Simulation() {
    JoinWorker();
    // unregister sensors
    for (auto& sens : sensors_) {
      player_.Unregister(sens);
    }
  }

  void StartSimulation(NetPublisher& net_publisher);
  void StopSimulation() noexcept { worker_.request_stop(); }
  void Wait();
};

}  // namespace fx
