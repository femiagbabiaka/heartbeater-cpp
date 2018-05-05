#include "heartbeater.h"

namespace Heartbeater {
Heartbeat::Heartbeat(std::string ServiceName, std::string Hostname,
                     int SecondsBehind) {
  this->ServiceName = std::move(ServiceName);
  this->Hostname = std::move(Hostname);
  this->SecondsBehind = SecondsBehind;
}

HeartbeaterContainer::HeartbeaterContainer() {
  this->heartbeaterMap = std::make_shared<std::unordered_map<std::string, Heartbeat>>();
}

void HeartbeaterContainer::addHeartbeat(std::string const key, Heartbeat value) {
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
    std::string Hostname, std::string HeartbeaterEndpoint,
    std::chrono::seconds IntervalBetweenHeartbeatsInSeconds,
    int RequestTimeoutInMilliseconds, int Retries) {
  this->Hostname = std::move(Hostname);
  this->HeartbeaterEndpoint = std::move(HeartbeaterEndpoint);
  this->IntervalBetweenHeartbeatsInSeconds = IntervalBetweenHeartbeatsInSeconds;
  this->RequestTimeoutInMilliseconds = RequestTimeoutInMilliseconds;
  this->Retries = Retries;
  this->heartbeatMap = std::unique_ptr<HeartbeaterContainer>(new HeartbeaterContainer());

  startHeartbeater();
}

void Heartbeater::sendHeartbeat(std::string ServiceName) {
  Heartbeat heartbeatToSend = Heartbeat(ServiceName, Hostname, 0);
  heartbeatMap->addHeartbeat(ServiceName, heartbeatToSend);
}

void Heartbeater::dumpHeartbeats() noexcept {
  for (auto item : *heartbeatMap->heartbeaterMap) {
    std::cout << " " << item.first << " " << std::endl;
  }
}
void Heartbeater::sendSecondsBehind(std::string ServiceName, int SecondsBehind) {
  Heartbeat heartbeatToSend = Heartbeat(ServiceName, Hostname, SecondsBehind);
  heartbeatMap->addHeartbeat(ServiceName, heartbeatToSend);
}
void Heartbeater::doSend(Heartbeat beat) {

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
