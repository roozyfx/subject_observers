#include <future>
#include <zmq_addon.hpp>

#include "sim/player.h"
#include "sim/simulation.h"

int main() {
  zmq::context_t ctx(1);

  fx::Player<float> pl;
  fx::Simulation<float> sim(pl);
  auto thread1 =
      std::async(std::launch::async,
                 &fx::Simulation<float>::Simulate, &sim, &ctx);

  thread1.wait();

  return 0;
}
