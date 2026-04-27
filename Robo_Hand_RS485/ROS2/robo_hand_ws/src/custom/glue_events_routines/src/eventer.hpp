
#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <control_msgs/msg/joint_jog.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <moveit_msgs/msg/planning_scene.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/bool.hpp>


namespace glue_events_routines {

class Eventer : public rclcpp::Node {
public:
    Eventer(const rclcpp::NodeOptions& options);
private:
    template <typename T>
    void _declare_or_get_param(T& output_value, const std::string& param_name, const T default_value = T{});

    void _joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg);

    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_sub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr routine_run__pub;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr proceed_pub;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr snapshoot_pub;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr reselect_best__pub;

    rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr servo_start_client_;
    


    int dof_;
    int ros_queue_size_;
    int joystick_type_;
    bool toggling_proceed;

    std::string joy_topic_;

    std::vector<bool> prev_pb;
    std::vector<bool> pb_re;
    std::vector<bool> pb_fe;

    bool proceed_toggle_state;
};
}


