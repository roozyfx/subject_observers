#include <future>
#include <iostream>
#include <print>
#include <string>
#include <zmq_addon.hpp>

#include "position.pb.h"

static void PrintPositionMessages(const fx::Position& pos_msg) {
  std::println("Sensor ID: {}\ttimestamp: {}", pos_msg.id(),
               pos_msg.timestamp_ms());
  std::println("Data3D: ({}, {}, {})", pos_msg.position_m().x(),
               pos_msg.position_m().y(), pos_msg.position_m().z());
}

[[noreturn]] static void SubscriberThread(zmq::context_t* ctx,
                                          std::string addr) {
  //  Prepare subscriber
  zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
  subscriber.connect(addr);

  // Receive everything!
  subscriber.set(zmq::sockopt::subscribe, "");

  while (true) {
    zmq::message_t recv_data;
    auto result = subscriber.recv(recv_data, zmq::recv_flags::none);
    if (!result) {
      std::cerr << "Unsuccessful data reception\n";
      continue;
    }
    if (recv_data.empty()) {
      std::cerr << "Empty message\n";
      continue;
    }

    fx::Position recv_msg;
    bool ok = recv_msg.ParseFromArray(recv_data.data(),
                                      static_cast<int>(recv_data.size()));
    if (!ok) {
      std::cerr << "Failed to parse Position messages\n";
      continue;
    }

    PrintPositionMessages(recv_msg);
  }
}

int main() {
  try {
    zmq::context_t ctx(1);

    std::string addr{"tcp://127.0.0.1:5656"};
    auto thread2 = std::async(std::launch::async, SubscriberThread, &ctx, addr);
    thread2.get();
  } catch (const std::exception& e) {
    std::cerr << e.what();
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
