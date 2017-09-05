#pragma once

#include "aerial_autonomy/types/position_yaw.h"
#include <tf_conversions/tf_eigen.h>

namespace conversions {

/**
 * @brief Convert an Eigen::Matrix4d to a tf::Transform
 * @param e Input Eigen::Matrix4d
 * @param tf Output tf::Transform
 */
void transformMatrix4dToTf(const Eigen::Matrix4d &e, tf::Transform &tf);

/**
 * @brief Convert roll, pitch, yaw Euler angles to a tf::Transform
 * @param r Roll
 * @param p Pitch
 * @param y Yaw
 * @param tf Output tf::Transform
 */
void transformRPYToTf(double r, double p, double y, tf::Transform &tf);

/**
 * @brief Convert PositionYaw to tf::Transform
 * @param p PositionYaw to convert
 * @param tf The equivalent tf::Transform
 */
void positionYawToTf(const PositionYaw &p, tf::Transform &tf);
}
