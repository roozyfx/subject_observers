#pragma once
#include <zmq_addon.hpp>

#include "net_publisher.h"

namespace fx {
class ZMQPublisher final : public NetPublisher {
  zmq::context_t* context_;
  std::string net_address_;
  zmq::socket_t publisher_;

 public:
  explicit ZMQPublisher(zmq::context_t* ctx, std::string_view net_address)
      : context_(std::move(ctx)), net_address_(net_address) {
    publisher_ = zmq::socket_t{*context_, zmq::socket_type::pub};
    publisher_.bind(net_address_);
  }

  ~ZMQPublisher() override = default;

  void SetNetAddress(std::string_view net_addr) override {
    net_address_ = net_addr;
  }

  void Send(const std::string& msg) override {
    publisher_.send(zmq::buffer(msg), zmq::send_flags::none);
  }
};

}  // namespace fx
