#ifndef HEARTBEATER_CPP_LIBRARY_H
#define HEARTBEATER_CPP_LIBRARY_H

#include "absl/synchronization/mutex.h"
#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

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
  void addHeartbeat(std::string key, Heartbeat *value);
  std::unordered_map<std::string, Heartbeat> *heartbeaterMap;

private:
  absl::Mutex *mapMutex;
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
  HeartbeaterContainer *heartbeatMap;
  int Retries;
  static Heartbeater &getInstance() {
    static Heartbeater instance;
    return instance;
  }
  Heartbeater(Heartbeater const&) = delete;
  void operator=(Heartbeater const&) = delete;


private:
  void doSend(Heartbeat beat);
  void sendAll();
  void startHeartBeater();
};

} // namespace Heartbeater

#endif // HEARTBEATER_CPP_LIBRARY_H
