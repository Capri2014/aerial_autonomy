#include "aerial_autonomy/controller_hardware_connectors/rpyt_relative_pose_visual_servoing_connector.h"
#include "aerial_autonomy/log/log.h"

bool RPYTRelativePoseVisualServoingConnector::extractSensorData(
    std::tuple<tf::Transform, tf::Transform, VelocityYawRate> &sensor_data) {
  parsernode::common::quaddata quad_data;
  drone_hardware_.getquaddata(quad_data);
  tf::Transform tracking_pose;
  if (!getTrackingTransformRotationCompensatedQuadFrame(tracking_pose)) {
    VLOG(1) << "Invalid tracking vector";
    return false;
  }
  VelocityYawRate current_velocity_yawrate(
      quad_data.linvel.x, quad_data.linvel.y, quad_data.linvel.z,
      quad_data.omega.z);
  DATA_LOG("rpyt_relative_pose_visual_servoing_connector")
      << quad_data.linvel.x << quad_data.linvel.y << quad_data.linvel.z
      << quad_data.rpydata.x << quad_data.rpydata.y << quad_data.rpydata.z
      << quad_data.omega.x << quad_data.omega.y << quad_data.omega.z
      << DataStream::endl;
  // giving transform in rotation-compensated quad frame
  sensor_data = std::make_tuple(getBodyFrameRotation(), tracking_pose,
                                current_velocity_yawrate);
  return true;
}

void RPYTRelativePoseVisualServoingConnector::sendHardwareCommands(
    RollPitchYawRateThrust controls) {
  geometry_msgs::Quaternion rpyt_msg;
  rpyt_msg.x = controls.r;
  rpyt_msg.y = controls.p;
  rpyt_msg.z = controls.y;
  rpyt_msg.w = controls.t;
  drone_hardware_.cmdrpyawratethrust(rpyt_msg);
}