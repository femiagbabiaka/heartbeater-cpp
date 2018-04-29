#ifndef HEARTBEATER_CPP_LIBRARY_H
#define HEARTBEATER_CPP_LIBRARY_H

#include <chrono>
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

namespace Heartbeater {
class Heartbeat {
 public:
  Heartbeat(std::string ServiceName, std::string Hostname, int SecondsBehind);
  std::string ServiceName;
  std::string Hostname;
  int SecondsBehind;
};

class HeartbeaterContainer {
 public:
  HeartbeaterContainer();
  void lockContainer();
  void unlockContainer();
  void addHeartbeat(std::string key, std::shared_ptr<Heartbeat> value);
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Heartbeat>>> heartbeaterMap;

 private:
  std::mutex _mutex;
};

class Heartbeater {
 public:
  Heartbeater(std::string Hostname, std::string HeartbeaterEndpoint,
              std::chrono::seconds IntervalBetweenHeartbeatsInSeconds,
              std::chrono::milliseconds RequestTimeoutInMilliseconds,
              int Retries);
  void sendHeartbeat(std::string ServiceName);
  void sendSecondsBehind(std::string ServiceName, int SecondsBehind);
  void dumpHeartbeats();
  std::string Hostname;
  std::string HeartbeaterEndpoint;
  std::chrono::seconds IntervalBetweenHeartbeatsInSeconds;
  std::chrono::milliseconds RequestTimeoutInMilliseconds;
  std::shared_ptr<HeartbeaterContainer> heartbeatMap;
  int Retries;
 private:
  void doSend(Heartbeat beat);
  void sendAll();
  void startHeartBeater();
};

} // namespace Heartbeater

#endif // HEARTBEATER_CPP_LIBRARY_H
