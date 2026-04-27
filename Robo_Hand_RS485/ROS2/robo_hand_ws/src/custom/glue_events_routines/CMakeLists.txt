cmake_minimum_required(VERSION 3.5)
project(glue_events_routines)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(control_msgs REQUIRED)
find_package(moveit_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(xarm_msgs REQUIRED)
find_package(xarm_planner REQUIRED)


set(dependencies "rclcpp" "std_msgs" "geometry_msgs" "moveit_msgs" "xarm_msgs" "std_srvs" "xarm_planner")

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

set(THIS_PACKAGE_INCLUDE_DEPENDS
  rclcpp
  rclcpp_components
  std_msgs
  std_srvs
  sensor_msgs
  control_msgs
  moveit_msgs
  geometry_msgs
  moveit_ros_planning_interface
  tf2_ros
  xarm_msgs
  xarm_planner
  xarm_sdk
)

include_directories(
  include
  ${ament_INCLUDE_DIRS}
)

add_library(eventer SHARED src/eventer.cpp)
ament_target_dependencies(eventer ${THIS_PACKAGE_INCLUDE_DEPENDS})
rclcpp_components_register_nodes(eventer "glue_events_routines::Eventer")


include_directories(
	../../../install/xarm_planner/include
)
link_directories(
	../../../install/xarm_planner/lib
)


add_library(delta_twist_cmds_mux SHARED src/delta_twist_cmds_mux.cpp)
ament_target_dependencies(delta_twist_cmds_mux ${THIS_PACKAGE_INCLUDE_DEPENDS})
rclcpp_components_register_nodes(delta_twist_cmds_mux "glue_events_routines::delta_twist_cmds_mux")


install(TARGETS
  eventer
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib/${PROJECT_NAME}
  INCLUDES DESTINATION include
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
