#include "heartbeater.h"

namespace Heartbeater {
Heartbeat::Heartbeat(std::string ServiceName, std::string Hostname,
                     int SecondsBehind) {
  this->ServiceName = ServiceName;
  this->Hostname = Hostname;
  this->SecondsBehind = SecondsBehind;
}

HeartbeaterContainer::HeartbeaterContainer(void) {
  this->heartbeaterMap = std::make_shared<std::unordered_map<std::string, std::shared_ptr<Heartbeat>>>();
}

void HeartbeaterContainer::addHeartbeat(std::string const key, std::shared_ptr<Heartbeat> value) {
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
  this->heartbeatMap = std::make_shared<HeartbeaterContainer>();
}

void Heartbeater::sendHeartbeat(std::string ServiceName) {
  std::shared_ptr<Heartbeat> heartbeatToSend = std::make_shared<Heartbeat>(ServiceName, this->Hostname, 0);
  this->heartbeatMap->addHeartbeat(ServiceName, heartbeatToSend);
}

void Heartbeater::dumpHeartbeats() {
  for (auto item : *this->heartbeatMap->heartbeaterMap) {
    std::cout << " " << item.first << " " << std::endl;
    std::cout << " " << item.second << " " << std::endl;
  }
}

} // namespace Heartbeater
