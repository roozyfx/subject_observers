#include "sim/simulation.h"

#include <chrono>
#include <concepts>
#include <memory>
#include <thread>

#include "utils/config.h"

namespace fx {

template <std::floating_point T>
Simulation<T>::Simulation(Player<T>& player, const size_t num_sensors)
    : player_{player} {
  sensors_.reserve(num_sensors);
  for (size_t id = 0; id < num_sensors; ++id) {
    auto sensor{std::make_shared<Sensor<T>>()};
    sensor->SetID(static_cast<uint64_t>(id));
    // register sensor as an observer of player_
    player_.Register(sensor);
    sensors_.push_back(std::move(sensor));
  }
}

template <std::floating_point T>
void Simulation<T>::StartSimulation(zmq::context_t* ctx) {
  if (worker_.joinable())
    throw std::logic_error("Simulation has already started!");

  worker_ = std::jthread([this, ctx](std::stop_token stoken) {
    try {
      RunSimulation(ctx, stoken);
    } catch (...) {
      worker_exception_ = std::current_exception();
    }
  });
}

template <std::floating_point T>
void Simulation<T>::RunSimulation(zmq::context_t* ctx, std::stop_token stoken) {
  // Setup publisher
  zmq::socket_t sensors_publisher(*ctx, zmq::socket_type::pub);

  sensors_publisher.bind(net_address_);

  // Give the subscribers a chance to connect, so they don't lose any messages
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  const T simulation_duration{Config<T>::GetValues().at("simulation_duration")};
  using sc = std::chrono::steady_clock;
  const auto start_time{sc::now()};

  while (std::chrono::duration<T>(sc::now() - start_time).count() <
             simulation_duration and
         not stoken.stop_requested()) {
    player_.Update();
    // TODO Change so each sensor has its own thread and sends
    for (auto& sensor : sensors_) {
      {
        Position msg;
        if (sensor) {
          sensor->ReadMeasurement(msg);

          std::string msg_buffer;
          if (msg.SerializeToString(&msg_buffer))
            sensors_publisher.send(zmq::buffer(msg_buffer),
                                   zmq::send_flags::none);
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::duration<T>(player_.delta_t()));
  }
}

template <std::floating_point T>
void Simulation<T>::JoinWorker() noexcept {
  if (worker_.joinable()) {
    worker_.request_stop();
    worker_.join();
  }
}

template <std::floating_point T>
void Simulation<T>::Wait() {
  if (worker_.joinable()) worker_.join();
  if (worker_exception_) {
    std::rethrow_exception(std::exchange(worker_exception_, nullptr));
  }
}

}  // namespace fx

template class fx::Simulation<float>;
template class fx::Simulation<double>;
