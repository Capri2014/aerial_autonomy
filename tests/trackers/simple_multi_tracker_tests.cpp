#include <gtest/gtest.h>

#include "aerial_autonomy/trackers/simple_multi_tracker.h"

TEST(SimpleMultiTrackerTests, Constructor) {
  SimpleMultiTracker simple_tracker(MultiTracker::CLOSEST);
}

TEST(SimpleTrackerTests, SetTrackingVectors) {
  SimpleMultiTracker simple_tracker(MultiTracker::CLOSEST);

  std::vector<std::tuple<uint32_t, Position>> tracking_vectors;
  tracking_vectors.push_back(std::make_tuple(0, Position(1, 1, 1)));
  tracking_vectors.push_back(std::make_tuple(1, Position(1, 1, 3)));
  simple_tracker.setTrackingVectors(tracking_vectors);

  std::vector<std::tuple<uint32_t, Position>> tracking_vectors_get;
  simple_tracker.getTrackingVectors(tracking_vectors_get);

  ASSERT_EQ(tracking_vectors_get.size(), tracking_vectors.size());

  for (unsigned int i = 0; i < tracking_vectors_get.size(); i++) {
    ASSERT_EQ(tracking_vectors_get[i], tracking_vectors[i]);
  }
}

TEST(SimpleTrackerTests, GetTrackingVectorClosest) {
  SimpleMultiTracker simple_tracker(MultiTracker::CLOSEST);

  std::vector<std::tuple<uint32_t, Position>> tracking_vectors;
  tracking_vectors.push_back(std::make_tuple(0, Position(1, 1, 1)));
  tracking_vectors.push_back(std::make_tuple(1, Position(1, 1, 3)));
  simple_tracker.setTrackingVectors(tracking_vectors);

  Position tracking_vector;
  ASSERT_TRUE(simple_tracker.getTrackingVector(tracking_vector));
  ASSERT_EQ(tracking_vector, std::get<1>(tracking_vectors[0]));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
