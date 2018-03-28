#include "heartbeater.h"

namespace Heartbeater {
Heartbeat::Heartbeat(std::string ServiceName, std::string Hostname,
                     int SecondsBehind) {
  this->ServiceName = ServiceName;
  this->Hostname = Hostname;
  this->SecondsBehind = SecondsBehind;
}

HeartbeaterContainer::HeartbeaterContainer(void) {
  std::unordered_map<std::string, Heartbeat> *heartbeaterMap{};
  this->heartbeaterMap = heartbeaterMap;
}

void HeartbeaterContainer::lockContainer() { 
  absl::MutexLock l(this->mapMutex);
}

void HeartbeaterContainer::addHeartbeat(std::string key, Heartbeat *value) {
  this->heartbeaterMap->emplace(key, *value);
}

Heartbeater::Heartbeater(
    std::string Hostname, std::string HeartbeaterEndpoint,
    std::chrono::seconds IntervalBetweenHeartbeatsInSeconds,
    std::chrono::milliseconds RequestTimeoutInMilliseconds, int Retries) {
  this->Hostname = Hostname;
  this->HeartbeaterEndpoint = HeartbeaterEndpoint;
  this->IntervalBetweenHeartbeatsInSeconds = IntervalBetweenHeartbeatsInSeconds;
  this->RequestTimeoutInMilliseconds = RequestTimeoutInMilliseconds;
  this->Retries = Retries;
  this->heartbeatMap = new HeartbeaterContainer::HeartbeaterContainer();
}

void Heartbeater::sendHeartbeat(std::string ServiceName) {
  this->heartbeatMap->lockContainer();
  Heartbeat *heartbeatToSend = new Heartbeat(ServiceName, this->Hostname, 0);
  this->heartbeatMap->addHeartbeat(ServiceName, heartbeatToSend);
}

void Heartbeater::dumpHeartbeats() {
  for (auto item : *this->heartbeatMap->heartbeaterMap) {
    std::cout << " " << item.first << " " << std::endl;
  }
}

} // namespace Heartbeater
