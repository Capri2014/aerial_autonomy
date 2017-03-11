#include <aerial_autonomy/basic_events.h>
#include <aerial_autonomy/onboard_system_handler.h>
#include <aerial_autonomy/state_machines/basic_state_machine.h>
#include <gtest/gtest.h>
#include <ros/ros.h>

#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/String.h>

using namespace basic_events;

class OnboardSystemHandlerTests : public ::testing::Test {
public:
  OnboardSystemHandlerTests() : nh_(), nh_send_() {
    // Configure system
    OnboardSystemHandlerConfig onboard_system_config;
    onboard_system_config.set_uav_parser_type(
        "quad_simulator_parser/QuadSimParser");
    onboard_system_config.mutable_uav_system_config()
        ->set_minimum_takeoff_height(0.4);

    onboard_system_handler_.reset(
        new OnboardSystemHandler<LogicStateMachine,
                                 BasicEventManager<LogicStateMachine>>(
            nh_, onboard_system_config));
    event_pub_ = nh_send_.advertise<std_msgs::String>("event_manager", 1);
    pose_pub_ =
        nh_send_.advertise<geometry_msgs::PoseStamped>("goal_pose_command", 1);
    ros::spinOnce();
  }

  void publishEvent(std::string event) {
    std_msgs::String event_msg;
    event_msg.data = event;
    event_pub_.publish(event_msg);
    // wait for threads to process
    ros::spinOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    ros::spinOnce();
  }

  void publishPoseCommand(PositionYaw pose) {
    geometry_msgs::PoseStamped pose_msg;
    pose_msg.pose.position.x = pose.x;
    pose_msg.pose.position.y = pose.y;
    pose_msg.pose.position.z = pose.z;
    pose_msg.pose.orientation.w = 1; // TODO(matt): use yaw
    pose_pub_.publish(pose_msg);
    // wait for threads to process
    ros::spinOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    ros::spinOnce();
  }

private:
  ros::NodeHandle nh_;
  ros::NodeHandle nh_send_;

  ros::Publisher event_pub_;
  ros::Publisher pose_pub_;

public:
  std::unique_ptr<OnboardSystemHandler<LogicStateMachine,
                                       BasicEventManager<LogicStateMachine>>>
      onboard_system_handler_;
};

TEST_F(OnboardSystemHandlerTests, Constructor) {}

TEST_F(OnboardSystemHandlerTests, TestConnections) {
  while (!onboard_system_handler_->isConnected()) {
  }
  SUCCEED();
}

TEST_F(OnboardSystemHandlerTests, ProcessEvents) {
  while (!onboard_system_handler_->isConnected()) {
  }
  // Check takeoff works
  publishEvent("Takeoff");
  ASSERT_TRUE(onboard_system_handler_->getUAVData().armed);
  ASSERT_EQ(onboard_system_handler_->getUAVData().localpos.z, 0.5);
  // Check subsequent event works
  publishEvent("Land");
  ASSERT_FALSE(onboard_system_handler_->getUAVData().armed);
  ASSERT_EQ(onboard_system_handler_->getUAVData().localpos.z, 0.0);
}

TEST_F(OnboardSystemHandlerTests, ProcessPoseCommand) {
  while (!onboard_system_handler_->isConnected()) {
  }
  // Check pose command works
  publishEvent("Takeoff");
  ASSERT_TRUE(onboard_system_handler_->getUAVData().armed);
  ASSERT_EQ(onboard_system_handler_->getUAVData().localpos.z, 0.5);
  PositionYaw pose_command(1, 2, 3, 0);
  publishPoseCommand(pose_command);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  parsernode::common::quaddata quad_data =
      onboard_system_handler_->getUAVData();
  ASSERT_EQ(PositionYaw(quad_data.localpos.x, quad_data.localpos.y,
                        quad_data.localpos.z, quad_data.rpydata.z),
            pose_command);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  ros::init(argc, argv, "onboard_system_handler_tests");
  return RUN_ALL_TESTS();
}