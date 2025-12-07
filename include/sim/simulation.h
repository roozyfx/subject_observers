
#pragma once
#include <zmq_addon.hpp>

#include "player.h"
#include "sensor.h"
using namespace std;

namespace fx {
template <std::floating_point T>
class Simulation {
  // Player, i.e. subject of the observation
  Player<T>& player_;
  // array of measuring sensors, i.e. observers
  vector<shared_ptr<Sensor<T>>> sensors_;
  std::string net_address_{"tcp://localhost:5656"};

 public:
  explicit Simulation(Player<T>& player, const size_t num_sensors = 10);
  Simulation(const Simulation&) = delete;
  Simulation& operator=(const Simulation&) = delete;

  Simulation(Simulation&&) = delete;
  Simulation& operator=(Simulation&&) = delete;

  ~Simulation() {
    // unregister sensors
    for (auto sens : sensors_) {
      player_.Unregister(sens);
    }
  }

  inline void SetNetAddress(const std::string addr) { net_address_ = addr; }

  void Simulate(zmq::context_t* ctx);
};

}  // namespace fx