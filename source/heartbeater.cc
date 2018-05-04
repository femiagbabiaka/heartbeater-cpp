#include "heartbeater.h"

namespace Heartbeater {
Heartbeat::Heartbeat(std::string ServiceName, std::string Hostname,
                     int SecondsBehind) {
  this->ServiceName = ServiceName;
  this->Hostname = Hostname;
  this->SecondsBehind = SecondsBehind;
}

HeartbeaterContainer::HeartbeaterContainer(void) {
  this->heartbeaterMap = std::make_shared<std::unordered_map<std::string, Heartbeat>>();
}

void HeartbeaterContainer::addHeartbeat(std::string const key, Heartbeat value) {
  std::lock_guard<std::mutex> guard(_mutex);
  this->heartbeaterMap->emplace(key, value);
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
  this->heartbeatMap = std::unique_ptr<HeartbeaterContainer>( new HeartbeaterContainer() );
}

void Heartbeater::sendHeartbeat(std::string ServiceName) {
  Heartbeat heartbeatToSend = Heartbeat(ServiceName, this->Hostname, 0);
  this->heartbeatMap->addHeartbeat(ServiceName, heartbeatToSend);
}

void Heartbeater::dumpHeartbeats() noexcept {
  for (auto item : *this->heartbeatMap->heartbeaterMap) {
    std::cout << " " << item.first << " " << std::endl;
  }
}
void Heartbeater::sendSecondsBehind(std::string ServiceName, int SecondsBehind) {
  Heartbeat heartbeatToSend = Heartbeat(ServiceName, this->Hostname, SecondsBehind);
  this->heartbeatMap->addHeartbeat(ServiceName, heartbeatToSend);
}

} // namespace Heartbeater
