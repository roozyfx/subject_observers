#pragma once
#include <concepts>
#include <exception>
#include <memory>
#include <stop_token>
#include <string>
#include <thread>
#include <vector>
#include <zmq_addon.hpp>

#include "player.h"
#include "sensor.h"
#include "utils/config.h"

namespace fx {

template <std::floating_point T>
class Simulation {
  void RunSimulation(zmq::context_t* ctx, std::stop_token stoken);
  void JoinWorker() noexcept;

  // Player, i.e. subject of the observation
  Player<T>& player_;
  // array of measuring sensors, i.e. observers
  std::vector<std::shared_ptr<Sensor<T>>> sensors_;
  std::string net_address_{"tcp://127.0.0.1:5656"};

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

  void StartSimulation(zmq::context_t* ctx);
  void StopSimulation() noexcept { worker_.request_stop(); }
  void Wait();
};

}  // namespace fx
