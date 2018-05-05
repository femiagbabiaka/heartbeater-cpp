#ifndef HEARTBEATER_CPP_LIBRARY_H
#define HEARTBEATER_CPP_LIBRARY_H

#include <chrono>
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <cpr/cpr.h>

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
  void addHeartbeat(std::string key, Heartbeat value);
  std::unordered_map<std::string, Heartbeat> reset();
  std::shared_ptr<std::unordered_map<std::string, Heartbeat>> heartbeaterMap;

private:
  std::mutex _mutex;
};

class Heartbeater {
public:
  Heartbeater(std::string Hostname, std::string HeartbeaterEndpoint,
              std::chrono::seconds IntervalBetweenHeartbeatsInSeconds,
              int RequestTimeoutInMilliseconds,
              int Retries);
  ~Heartbeater();
  void sendHeartbeat(std::string ServiceName);
  void sendSecondsBehind(std::string ServiceName, int SecondsBehind);
  void dumpHeartbeats() noexcept;
  std::string Hostname;
  std::string HeartbeaterEndpoint;
  std::chrono::seconds IntervalBetweenHeartbeatsInSeconds;
  int RequestTimeoutInMilliseconds;
  std::unique_ptr<HeartbeaterContainer> heartbeatMap;
  int Retries;
private:
  void doSend(Heartbeat beat);
  void sendAll();
  void startHeartbeater();
  void stopHeartbeater();
  std::atomic<bool> _execute;
  std::thread _thread;
};

} // namespace Heartbeater

#endif // HEARTBEATER_CPP_LIBRARY_H
