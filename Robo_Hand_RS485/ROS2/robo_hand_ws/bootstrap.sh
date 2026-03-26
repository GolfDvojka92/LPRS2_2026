#!/bin/bash

source /opt/ros/jazzy/setup.bash

pushd src/
# 0c3eabf56711b5b95e9edaba5600b86d99fe4229
git clone https://github.com/xArm-Developer/xarm_ros2.git --recursive -b ${ROS_DISTRO}
popd

patch -p1 -d src/xarm_ros2/ < patches/xarm_ros2__${ROS_DISTRO}.patch
