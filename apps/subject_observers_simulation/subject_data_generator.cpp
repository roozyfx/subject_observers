#include <exception>
#include <iostream>
#include <zmq_addon.hpp>

#include "sim/player.h"
#include "sim/simulation.h"

int main() {
  try {
    zmq::context_t ctx(1);

    fx::Player<float> pl{};
    fx::Simulation<float> sim(pl);

    sim.StartSimulation(&ctx);
    // Rethrow the exception from a failure
    sim.Wait();
  } catch (const std::exception& e) {
    std::cerr << e.what();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
