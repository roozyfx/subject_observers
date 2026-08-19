#pragma once
#include <string>
#include <string_view>

namespace fx {

/* Bridge class interface.
For example to be passed to Simulation, to be a bridge to ZeroMQ or gRPC network
publishers.
*/
class NetPublisher {
 public:
  virtual ~NetPublisher() = default;

  virtual void SetNetAddress(std::string_view net_addr) = 0;

  virtual void Send(const std::string& msg) = 0;
};

}  // namespace fx
