#ifndef HEARTBEATER_CPP_LIBRARY_H
#define HEARTBEATER_CPP_LIBRARY_H

#include <chrono>
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <thread>

namespace Heartbeater {
class Heartbeat {
public:
  Heartbeat(std::string const &ServiceName, std::string const &Hostname, int const &SecondsBehind);
  std::string ServiceName;
  std::string Hostname;
  int SecondsBehind;
};

class HeartbeaterContainer {
public:
  HeartbeaterContainer();
  void addHeartbeat(std::string const &key, Heartbeat const &value);
  std::unordered_map<std::string, Heartbeat> reset();
  std::shared_ptr<std::unordered_map<std::string, Heartbeat>> heartbeaterMap;

private:
  std::mutex _mutex;
};

class Heartbeater {
public:
  Heartbeater(std::string const &Hostname, std::string const &HeartbeaterEndpoint,
              std::chrono::seconds const &IntervalBetweenHeartbeatsInSeconds,
              int const &RequestTimeoutInMilliseconds,
              int const &Retries);
  ~Heartbeater();
  void sendHeartbeat(std::string const &ServiceName);
  void sendSecondsBehind(std::string const &ServiceName, int const &SecondsBehind);
  std::string Hostname;
  std::string HeartbeaterEndpoint;
  std::chrono::seconds IntervalBetweenHeartbeatsInSeconds;
  int RequestTimeoutInMilliseconds;
  std::unique_ptr<HeartbeaterContainer> heartbeatMap;
  int Retries;
private:
  void doSend(Heartbeat const &beat);
  void sendAll();
  void startHeartbeater();
  void stopHeartbeater();
  std::atomic<bool> _execute;
  std::thread _thread;
};

} // namespace Heartbeater

#endif // HEARTBEATER_CPP_LIBRARY_H
