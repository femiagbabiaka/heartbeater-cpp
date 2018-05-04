#include "heartbeater.h"

int main() {
  Heartbeater::Heartbeater myHeartbeater("localhost", "localhost/beat",
                                         std::chrono::seconds(5),
                                         std::chrono::milliseconds(20), 5);
  myHeartbeater.sendHeartbeat("test1");
  myHeartbeater.sendHeartbeat("test2");
  myHeartbeater.sendHeartbeat("test3");
  myHeartbeater.sendSecondsBehind("test4", 5);
  myHeartbeater.dumpHeartbeats();
  return 0;
}
