#!/bin/bash

exit 0

# Clone.
bootstrap.sh


source /opt/ros/jazzy/setup.bash

# Build.
colcon build --symlink-install

# Precise build.
colcon build --symlink-install --packages-select glue_events_routines


# http://192.168.1.163:18333/

# Run.
source install/setup.sh


#############################
# Connect to robot

# Test hand.
ros2 run glue_events_routines hand_test

#############################

#T1
ros2 launch xarm_moveit_config lite6_moveit_realmove.launch.py robot_ip:=192.168.1.163 add_gripper:=true


########
# Controlling digital pins

function set_dig_pin() {
    ros2 service call \
/ufactory/set_cgpio_digital \
xarm_msgs/srv/SetDigitalIO \
"
ionum: $1
value: $2
"
}

# Activate
set_dig_pin 0 1

# Up
set_dig_pin 1 0

# Down
set_dig_pin 1 1

# Stop
set_dig_pin 0 0


#######
#TODO RS-485

#######
# Some random helper

ros2 interface show xarm_msgs/srv/GetInt16
ros2 service call /ufactory/get_reduced_mode xarm_msgs/srv/GetInt16

ros2 interface show xarm_msgs/srv/SetInt16

ros2 service call /ufactory/set_reduced_mode xarm_msgs/srv/SetInt16 "{data: 1}"
ros2 service call /ufactory/set_reduced_max_joint_speed xarm_msgs/srv/SetFloat32 "{data: 0.1}"
ros2 service call /ufactory/get_reduced_mode xarm_msgs/srv/GetInt16