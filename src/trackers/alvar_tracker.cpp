#include "aerial_autonomy/trackers/alvar_tracker.h"

#include <glog/logging.h>

bool AlvarTracker::getTrackingVectors(
    std::vector<std::tuple<uint32_t, Position>> &pos) {
  if (!trackingIsValid()) {
    return false;
  }
  pos = object_positions_;
  return true;
}

bool AlvarTracker::trackingIsValid() {
  // \todo Matt: make timeout configurable
  bool valid = (ros::Time::now() - last_update_time_).toSec() < 0.5;
  if (!valid)
    VLOG(2) << "Alvar has not been updated for 0.5 seconds";
  return valid;
}

void AlvarTracker::markerCallback(
    const ar_track_alvar::AlvarMarkers &marker_msg) {
  last_update_time_ = ros::Time::now();
  std::vector<std::tuple<uint32_t, Position>> object_positions;
  for (unsigned int i = 0; i < marker_msg.markers.size(); i++) {
    auto marker_pose = marker_msg.markers[i].pose.pose;
    Position p(marker_pose.position.x, marker_pose.position.y,
               marker_pose.position.z);
    object_positions.push_back(std::make_tuple(marker_msg.markers[i].id, p));
  }
  object_positions_ = object_positions;
}
