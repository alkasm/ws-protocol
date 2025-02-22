#include <foxglove/websocket/server.hpp>

#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/util/time_util.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

#include "foxglove/SceneUpdate.pb.h"

static uint64_t nanosecondsSinceEpoch() {
  return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count());
}

// Adapted from:
// https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594
// https://github.com/protocolbuffers/protobuf/blob/01fe22219a0312b178a265e75fe35422ea6afbb1/src/google/protobuf/compiler/csharp/csharp_helpers.cc
static std::string Base64Encode(std::string_view data) {
  constexpr const char ALPHABET[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string result;
  // Every 3 bytes of data yields 4 bytes of output
  result.reserve((data.size() + (3 - 1 /* round up */)) / 3 * 4);

  size_t i = 0;
  for (; i + 2 < data.size(); i += 3) {
    result.push_back(ALPHABET[data[i] >> 2]);
    result.push_back(ALPHABET[((data[i] & 0b11) << 4) | (data[i + 1] >> 4)]);
    result.push_back(ALPHABET[((data[i + 1] & 0b1111) << 2) | (data[i + 2] >> 6)]);
    result.push_back(ALPHABET[data[i + 2] & 0b111111]);
  }
  switch (data.size() - i) {
    case 2:
      result.push_back(ALPHABET[data[i] >> 2]);
      result.push_back(ALPHABET[((data[i] & 0b11) << 4) | (data[i + 1] >> 4)]);
      result.push_back(ALPHABET[(data[i + 1] & 0b1111) << 2]);
      result.push_back('=');
      break;
    case 1:
      result.push_back(ALPHABET[data[i] >> 2]);
      result.push_back(ALPHABET[(data[i] & 0b11) << 4]);
      result.push_back('=');
      result.push_back('=');
      break;
  }

  return result;
}

// Writes the FileDescriptor of this descriptor and all transitive dependencies
// to a string, for use as a channel schema.
static std::string SerializeFdSet(const google::protobuf::Descriptor* toplevelDescriptor) {
  google::protobuf::FileDescriptorSet fdSet;
  std::queue<const google::protobuf::FileDescriptor*> toAdd;
  toAdd.push(toplevelDescriptor->file());
  std::unordered_set<std::string> seenDependencies;
  while (!toAdd.empty()) {
    const google::protobuf::FileDescriptor* next = toAdd.front();
    toAdd.pop();
    next->CopyTo(fdSet.add_file());
    for (int i = 0; i < next->dependency_count(); ++i) {
      const auto& dep = next->dependency(i);
      if (seenDependencies.find(dep->name()) == seenDependencies.end()) {
        seenDependencies.insert(dep->name());
        toAdd.push(dep);
      }
    }
  }
  return fdSet.SerializeAsString();
}

// https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html
static void setAxisAngle(foxglove::Quaternion* q, double x, double y, double z, double angle) {
  double s = std::sin(angle / 2);
  q->set_x(x * s);
  q->set_y(y * s);
  q->set_z(z * s);
  q->set_w(std::cos(angle / 2));
}

int main() {
  foxglove::websocket::Server server{8765, "C++ Protobuf example server"};

  const auto chanId = server.addChannel({
    .topic = "example_msg",
    .encoding = "protobuf",
    .schemaName = foxglove::SceneUpdate::descriptor()->full_name(),
    .schema = Base64Encode(SerializeFdSet(foxglove::SceneUpdate::descriptor())),
  });

  server.setSubscribeHandler([&](foxglove::websocket::ChannelId chanId) {
    std::cout << "first client subscribed to " << chanId << std::endl;
  });
  server.setUnsubscribeHandler([&](foxglove::websocket::ChannelId chanId) {
    std::cout << "last client unsubscribed from " << chanId << std::endl;
  });

  std::shared_ptr<asio::steady_timer> timer;
  std::function<void()> setTimer = [&] {
    timer = server.getEndpoint().set_timer(50, [&](std::error_code const& ec) {
      if (ec) {
        std::cerr << "timer error: " << ec.message() << std::endl;
        return;
      }

      const auto now = nanosecondsSinceEpoch();

      foxglove::SceneUpdate msg;
      auto* entity = msg.add_entities();
      *entity->mutable_timestamp() = google::protobuf::util::TimeUtil::NanosecondsToTimestamp(now);
      entity->set_frame_id("root");
      auto* cube = entity->add_cubes();
      auto* size = cube->mutable_size();
      size->set_x(1);
      size->set_y(1);
      size->set_z(1);
      auto* position = cube->mutable_pose()->mutable_position();
      position->set_x(2);
      position->set_y(0);
      position->set_z(0);
      auto* orientation = cube->mutable_pose()->mutable_orientation();
      setAxisAngle(orientation, 0, 0, 1, double(now) / 1e9 * 0.5);
      auto* color = cube->mutable_color();
      color->set_r(0.6);
      color->set_g(0.2);
      color->set_b(1);
      color->set_a(1);

      server.sendMessage(chanId, now, msg.SerializeAsString());
      setTimer();
    });
  };

  setTimer();

  asio::signal_set signals(server.getEndpoint().get_io_service(), SIGINT);

  signals.async_wait([&](std::error_code const& ec, int sig) {
    if (ec) {
      std::cerr << "signal error: " << ec.message() << std::endl;
      return;
    }
    std::cerr << "received signal " << sig << ", shutting down" << std::endl;
    server.removeChannel(chanId);
    server.stop();
    if (timer) {
      timer->cancel();
    }
  });

  server.run();

  return 0;
}
