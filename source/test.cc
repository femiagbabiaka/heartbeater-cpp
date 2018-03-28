#include "heartbeater.h"

int main() {
  Heartbeater::Heartbeater myHeartbeater("localhost", "localhost/beat",
                                         std::chrono::seconds(5),
                                         std::chrono::milliseconds(20), 5);
  myHeartbeater.sendHeartbeat("test1");
  return 0;
}
