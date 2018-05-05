#include "heartbeater.h"

int main() {
  Heartbeater::Heartbeater myHeartbeater("localhost", "http://postb.in/rge2figr",
                                         std::chrono::seconds(5),
                                         2000, 5);
  myHeartbeater.sendHeartbeat("test1");
  myHeartbeater.sendHeartbeat("test2");
  myHeartbeater.sendHeartbeat("test3");
  myHeartbeater.sendSecondsBehind("test4", 5);
  std::this_thread::sleep_for(std::chrono::seconds(10));
  return 0;
}
