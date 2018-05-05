#include "heartbeater.h"

int main() {
  Heartbeater::Heartbeater myHeartbeater("localhost", "http://ptsv2.com/t/ql1tn-1525479915/post",
                                         std::chrono::seconds(5),
                                         2000, 5);
  myHeartbeater.sendHeartbeat("test1");
  myHeartbeater.sendHeartbeat("test2");
  myHeartbeater.sendHeartbeat("test3");
  myHeartbeater.sendSecondsBehind("test4", 5);
  return 0;
}
