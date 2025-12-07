#include <future>
#include <iostream>
#include <print>
#include <string>
#include <thread>
#include <zmq_addon.hpp>

#include "position.pb.h"

std::mutex m;

void PrintPositionMessages(const fx::Position& pos_msg) {
  m.lock();
  std::println("Sensor ID: {}\ttimestamp: {}", pos_msg.id(),
               pos_msg.timestamp_ms());
  std::println("Data3D: ({}, {}, {})", pos_msg.position_m().x(),
               pos_msg.position_m().y(), pos_msg.position_m().z());
  m.unlock();
}

void SubscriberThread(zmq::context_t* ctx,
                      std::string addr) {
  //  Prepare subscriber
  zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
  subscriber.connect(addr);

  // Receive everything!
  subscriber.set(zmq::sockopt::subscribe, "");

  while (true) {
    zmq::message_t recv_data;
    auto result = subscriber.recv(recv_data, zmq::recv_flags::none);
    if (!result) continue;

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
  zmq::context_t ctx(1);

  // Give the publisher a chance to bind, since inproc requires it
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  std::string addr{"tcp://localhost:5656"};
  auto thread2 = std::async(std::launch::async, SubscriberThread, &ctx, addr);
  thread2.wait();

  return 0;
}