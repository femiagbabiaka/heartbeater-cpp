cc_library(
  name = "heartbeater",
  srcs = ["heartbeater.cc"],
  hdrs = ["heartbeater.h"],
  deps = [
    "@com_google_absl//absl/synchronization",
  ],
)

cc_binary(
  name = "test",
  srcs = ["test.cc"],
  deps = [
      ":heartbeater",
  ],
)
