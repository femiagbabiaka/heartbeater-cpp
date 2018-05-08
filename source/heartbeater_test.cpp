//
// Created by Femi Agbabiaka on 5/8/18.
//

#include <gtest/gtest.h>
#include <heartbeater.h>

TEST(CreateHeartbeater, CreateHeartbeater_CanCreateHeartbeater_Test) {
  auto myHeartbeater = std::unique_ptr<Heartbeater::Heartbeater>(new Heartbeater::Heartbeater(
      "localhost",
      "https://my.domain/heartbeater/",
      std::chrono::seconds(2),
      1000,
      2
  ));
}

TEST(AddHeartbeats, AddHeartbeats_CanAddHeartbeat_Test) {
  auto myHeartbeater = std::unique_ptr<Heartbeater::Heartbeater>(new Heartbeater::Heartbeater(
      "localhost",
      "https://my.domain/heartbeater/",
      std::chrono::seconds(0),
      1000,
      1
  ));

  myHeartbeater->sendHeartbeat("test_service");

  ASSERT_EQ(myHeartbeater->heartbeatMap->heartbeaterMap->count("test_service"), 1);
}
