#include <concepts>
#include <memory>
#include <thread>

#include "sim/simulation.h"
#include "utils/vectormath.h"

namespace fx {

template <std::floating_point T>
Simulation<T>::Simulation(Player<T>& player, const size_t num_sensors)
    : player_{player},
      sensors_{vector<shared_ptr<Sensor<T>>>(num_sensors)} {
  sensors_.reserve(num_sensors);
  for (size_t id = 0; id < num_sensors; ++id) {
    auto sens = make_shared<Sensor<T>>();
    sens->SetID(static_cast<uint64_t>(id));
    sensors_.push_back(sens);
    // register sensor as an observer of player_
    player_.Register(sensors_[id]);
  }
}

template <std::floating_point T>
inline void Simulation<T>::Simulate(zmq::context_t* ctx) {
  // Setup publisher
  zmq::socket_t sensors_publisher(*ctx, zmq::socket_type::pub);

  sensors_publisher.bind(net_address_);

  // Give the subscribers a chance to connect, so they don't lose any messages
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  while (true) {
    player_.Update();
    for (auto sen : sensors_) {
      Position msg;
      sen->ReadMeasurement(msg);

      std::string msg_buffer;
      msg.SerializeToString(&msg_buffer);
      sensors_publisher.send(std::move(zmq::buffer(msg_buffer)),
                             zmq::send_flags::none);
    }
  }
}
}  // namespace fx

template class fx::Simulation<float>;
template class fx::Simulation<double>;