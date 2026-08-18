#include <exception>
#include <iostream>
#include <memory>
#include <zmq_addon.hpp>

#include "sim/player.h"
#include "sim/simulation.h"
#include "sim/zmq_publisher.h"

int main() {
  try {
    zmq::context_t ctx(1);

    fx::Player<float> pl{};
    fx::Simulation<float> sim(pl);
    std::shared_ptr<fx::NetPublisher> zmq_publisher =
        std::make_shared<fx::ZMQPublisher>(&ctx, "tcp://127.0.0.1:5656");
    if (zmq_publisher) {
      sim.StartSimulation(*zmq_publisher);
      // Rethrow the exception from a failure
      sim.Wait();
    } else {
      std::cerr << "No ZMQPublisher available.\n";
      return EXIT_FAILURE;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
