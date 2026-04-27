
#include "eventer.hpp"

#define EN_PLANNING 1

#define DEBUG(var) \
    do{ \
       RCLCPP_INFO_STREAM(this->get_logger(), #var << " = " << var); \
    }while(0)

namespace glue_events_routines
{

enum JOYSTICK_TYPE
{
    JOYSTICK_XBOX360_WIRED = 1,
    JOYSTICK_XBOX360_WIRELESS = 2,
    JOYSTICK_SPACEMOUSE_WIRELESS = 3
};

enum XBOX360_WIRED_CONTROLLER_AXIS
{
    XBOX360_WIRED_LEFT_STICK_LR = 0,
    XBOX360_WIRED_LEFT_STICK_FB = 1,
    XBOX360_WIRED_LEFT_TRIGGER = 2,
    XBOX360_WIRED_RIGHT_STICK_LR = 3,
    XBOX360_WIRED_RIGHT_STICK_FB = 4,
    XBOX360_WIRED_RIGHT_TRIGGER = 5,
    XBOX360_WIRED_CROSS_KEY_LR = 6,
    XBOX360_WIRED_CROSS_KEY_FB = 7
};

enum XBOX360_WIRELESS_CONTROLLER_AXIS
{
    XBOX360_WIRELESS_LEFT_STICK_LR = 0,
    XBOX360_WIRELESS_LEFT_STICK_FB = 1,
    XBOX360_WIRELESS_RIGHT_STICK_LR = 2,
    XBOX360_WIRELESS_RIGHT_STICK_FB = 3,
    XBOX360_WIRELESS_LEFT_TRIGGER = 4,
    XBOX360_WIRELESS_RIGHT_TRIGGER = 5,
    XBOX360_WIRELESS_CROSS_KEY_LR = 6,
    XBOX360_WIRELESS_CROSS_KEY_FB = 7
};

enum XBOX360_CONTROLLER_BUTTON
{
    XBOX360_BTN_A = 0,
    XBOX360_BTN_B = 1,
    XBOX360_BTN_X = 2,
    XBOX360_BTN_Y = 3,
    XBOX360_BTN_LB = 4,
    XBOX360_BTN_RB = 5,
    XBOX360_BTN_BACK = 6,
    REDRAGON_BTN_SELECT = 6,
    XBOX360_BTN_START = 7,
    XBOX360_BTN_POWER = 8,
    XBOX360_BTN_STICK_LEFT = 9,
    XBOX360_BTN_STICK_RIGHT = 10
};

Eventer::Eventer(const rclcpp::NodeOptions& options)
  :
    Node("eventer", options), 
    dof_(7), ros_queue_size_(10), joystick_type_(JOYSTICK_XBOX360_WIRED),
    joy_topic_("/joy"),
    toggling_proceed(true),
    proceed_toggle_state(false)
{
    // init parameter from node
    _declare_or_get_param<int>(dof_, "dof", dof_);
    _declare_or_get_param<int>(ros_queue_size_, "ros_queue_size", ros_queue_size_);
    _declare_or_get_param<int>(joystick_type_, "joystick_type", joystick_type_);
    _declare_or_get_param<std::string>(joy_topic_, "joy_topic", joy_topic_);
    _declare_or_get_param<bool>(toggling_proceed, "toggling_proceed", toggling_proceed);


    // Setup pub/sub
    joy_sub_ = this->create_subscription<sensor_msgs::msg::Joy>(joy_topic_, ros_queue_size_, std::bind(&Eventer::_joy_callback, this, std::placeholders::_1));

    routine_run__pub = this->create_publisher<std_msgs::msg::String>("/routine_run", ros_queue_size_);
    proceed_pub = this->create_publisher<std_msgs::msg::Bool>("/proceed", ros_queue_size_);

    snapshoot_pub = this->create_publisher<std_msgs::msg::Bool>("/snapshoot", ros_queue_size_);
    reselect_best__pub = this->create_publisher<std_msgs::msg::Bool>("/reselect_best", ros_queue_size_);

    // Done in xarm_moveit_servo/JoyToServoPub
#if 0
    // Create a service client to start the ServoServer
    servo_start_client_ = this->create_client<std_srvs::srv::Trigger>("/servo_server/start_servo");
    servo_start_client_->wait_for_service(std::chrono::seconds(1));
    servo_start_client_->async_send_request(std::make_shared<std_srvs::srv::Trigger::Request>());
#endif
}

template <typename T>
void Eventer::_declare_or_get_param(T& output_value, const std::string& param_name, const T default_value)
{
    try
    {
        if (this->has_parameter(param_name))
        {
            this->get_parameter<T>(param_name, output_value);
        }
        else
        {
            output_value = this->declare_parameter<T>(param_name, default_value);
        }
    }
    catch (const rclcpp::exceptions::InvalidParameterTypeException& e)
    {
        RCLCPP_WARN_STREAM(this->get_logger(), "InvalidParameterTypeException(" << param_name << "): " << e.what());
        RCLCPP_ERROR_STREAM(this->get_logger(), "Error getting parameter \'" << param_name << "\', check parameter type in YAML file");
        throw e;
    }

    RCLCPP_INFO_STREAM(this->get_logger(), "Found parameter - " << param_name << ": " << output_value);
}




void Eventer::_joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
{

#if EN_PLANNING
    if(prev_pb.empty()){
       prev_pb.resize(msg->buttons.size());
       pb_re.resize(msg->buttons.size()); 
       pb_fe.resize(msg->buttons.size());
    }else{
        // RE and FE calc.
        for(uint i = 0; i < prev_pb.size(); i++){
            pb_re[i] = !prev_pb[i] && msg->buttons[i];
            pb_fe[i] = prev_pb[i] && !msg->buttons[i];
        }

        // Redragon: 1
        if(pb_re[XBOX360_BTN_Y]){
            std_msgs::msg::String routine_msg;
            routine_msg.data = "home";
            routine_run__pub->publish(routine_msg);
        }

        // Redragon: 2
        if(pb_re[XBOX360_BTN_B]){
            std_msgs::msg::String routine_msg;
            routine_msg.data = "pick_it_up_and_drop_it_to_bucket";
            routine_run__pub->publish(routine_msg);
        }

        // Redragon: 3
        if(toggling_proceed){
            if(pb_re[XBOX360_BTN_A]){
                proceed_toggle_state = !proceed_toggle_state;
                std_msgs::msg::Bool proceed_msg;
                proceed_msg.data = proceed_toggle_state;
                proceed_pub->publish(proceed_msg);
            }
        }else{
            if(pb_re[XBOX360_BTN_A]){
                std_msgs::msg::Bool proceed_msg;
                proceed_msg.data = true;
                proceed_pub->publish(proceed_msg);
            }
            if(pb_fe[XBOX360_BTN_A]){
                std_msgs::msg::Bool proceed_msg;
                proceed_msg.data = false;
                proceed_pub->publish(proceed_msg);
            }
        }
        // Redragon: 4
        if(pb_re[XBOX360_BTN_X]){
            std_msgs::msg::Bool snapshoot_msg;
            snapshoot_msg.data = true;
            snapshoot_pub->publish(snapshoot_msg);
        }

        // Redragon: select
        if(pb_re[REDRAGON_BTN_SELECT]){
            std_msgs::msg::Bool msg;
            msg.data = true;
            reselect_best__pub->publish(msg);
        }

    }
    for(uint i = 0; i < prev_pb.size(); i++){
        prev_pb[i] = msg->buttons[i];
    }

#endif
}

}

// Register the component with class_loader
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(glue_events_routines::Eventer)
