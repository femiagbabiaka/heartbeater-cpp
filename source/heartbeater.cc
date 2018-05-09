#include "heartbeater.h"

#include <cpr/cpr.h>

namespace Heartbeater {
Heartbeat::Heartbeat(std::string const &ServiceName, const std::string &Hostname,
                     int const &SecondsBehind) {
  this->ServiceName = ServiceName;
  this->Hostname = Hostname;
  this->SecondsBehind = SecondsBehind;
}

HeartbeaterContainer::HeartbeaterContainer() {
  this->heartbeaterMap = std::make_shared<std::unordered_map<std::string, Heartbeat>>();
}

void HeartbeaterContainer::addHeartbeat(std::string const &key, Heartbeat const &value) {
  heartbeaterMap->emplace(key, value);
}

std::unordered_map<std::string, Heartbeat> HeartbeaterContainer::reset() {
  std::lock_guard<std::mutex> guard(_mutex);
  // Make copy of shared heartbeaterMap and then clear the original.
  std::unordered_map<std::string, Heartbeat> heartbeaterMapCopy = *heartbeaterMap;
  heartbeaterMap->clear();

  return heartbeaterMapCopy;
}

Heartbeater::Heartbeater(
    std::string const &Hostname, std::string const &HeartbeaterEndpoint,
    std::chrono::seconds const &IntervalBetweenHeartbeatsInSeconds,
    int const &RequestTimeoutInMilliseconds, int const &Retries) {
  this->Hostname = Hostname;
  this->HeartbeaterEndpoint = HeartbeaterEndpoint;
  this->IntervalBetweenHeartbeatsInSeconds = IntervalBetweenHeartbeatsInSeconds;
  this->RequestTimeoutInMilliseconds = RequestTimeoutInMilliseconds;
  this->Retries = Retries;
  this->heartbeatMap = std::unique_ptr<HeartbeaterContainer>(new HeartbeaterContainer());

  startHeartbeater();
}

void Heartbeater::sendHeartbeat(std::string const &ServiceName) {
  Heartbeat heartbeatToSend = Heartbeat(ServiceName, Hostname, 0);
  heartbeatMap->addHeartbeat(ServiceName, heartbeatToSend);
}

void Heartbeater::sendSecondsBehind(std::string const &ServiceName, int const &SecondsBehind) {
  Heartbeat heartbeatToSend = Heartbeat(ServiceName, Hostname, SecondsBehind);
  heartbeatMap->addHeartbeat(ServiceName, heartbeatToSend);
}
void Heartbeater::doSend(Heartbeat const &beat) {

  auto payload = cpr::Payload{{"service", beat.ServiceName}, {"host", beat.Hostname},
                              {"seconds_behind", std::to_string(beat.SecondsBehind)}};

  auto response = cpr::Post(cpr::Url{HeartbeaterEndpoint}, cpr::Timeout{RequestTimeoutInMilliseconds}, payload);
}
void Heartbeater::sendAll() {
  auto heartbeaterMapCopy = this->heartbeatMap->reset();

  for (auto item : heartbeaterMapCopy) {
    doSend(item.second);
  }
}
void Heartbeater::startHeartbeater() {
  if (_execute.load(std::memory_order_acquire)) {
    stopHeartbeater();
  }

  _execute.store(true, std::memory_order_release);

  _thread = std::thread([this]() {
    while (_execute.load(std::memory_order_acquire)) {
      sendAll();
      std::this_thread::sleep_for(IntervalBetweenHeartbeatsInSeconds);
    }
  });
}
void Heartbeater::stopHeartbeater() {
  _execute.store(false, std::memory_order_release);

  if (_thread.joinable()) {
    _thread.join();
  }
}
Heartbeater::~Heartbeater() {
  if (_execute.load(std::memory_order_acquire)) {
    stopHeartbeater();
  }
}

} // namespace Heartbeater
