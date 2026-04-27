
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    robot_ip = LaunchConfiguration('robot_ip', default='192.168.1.163')
    report_type = LaunchConfiguration('report_type', default='dev')
    prefix = LaunchConfiguration('prefix', default='')
    hw_ns = LaunchConfiguration('hw_ns', default='ufactory')
    limited = LaunchConfiguration('limited', default=True)
    effort_control = LaunchConfiguration('effort_control', default=False)
    velocity_control = LaunchConfiguration('velocity_control', default=False)
    add_gripper = LaunchConfiguration('add_gripper', default=True)
    add_vacuum_gripper = LaunchConfiguration('add_vacuum_gripper', default=False)
    baud_checkset = LaunchConfiguration('baud_checkset', default=True)
    default_gripper_baud = LaunchConfiguration('default_gripper_baud', default=2000000)
    # 1: xbox360 wired
    # 2: xbox360 wireless
    # 3: spacemouse wireless
    joystick_type = LaunchConfiguration('joystick_type', default=1)

    en_vision = LaunchConfiguration('en_vision', default = False)
    reduced_speed = LaunchConfiguration('reduced_speed', default = 0.5) # Set to 0.1 to test it.
    toggling_proceed = LaunchConfiguration('toggling_proceed', default = True)
    sim = LaunchConfiguration('sim', default = False)


    # robot moveit servo launch
    # glue_events_routines/launch/_robot_moveit_servo.launch.py
    common_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(PathJoinSubstitution([FindPackageShare('glue_events_routines'), 'launch', '_common.launch.py'])),
        launch_arguments={
            'robot_ip': robot_ip,
            'report_type': report_type,
            'baud_checkset': baud_checkset,
            'default_gripper_baud': default_gripper_baud,
            'dof': '6',
            'prefix': prefix,
            'hw_ns': hw_ns,
            'limited': limited,
            'effort_control': effort_control,
            'velocity_control': velocity_control,
            'add_gripper': add_gripper,
            'add_vacuum_gripper': add_vacuum_gripper,
            'robot_type': 'lite',
            'ros2_control_plugin': 'uf_robot_hardware/UFRobotSystemHardware',
            'joystick_type' : joystick_type,
            'en_vision' : en_vision,
            'reduced_speed' : reduced_speed,
            'toggling_proceed' : toggling_proceed,
            'sim' : sim,
        }.items(),
    )

    return LaunchDescription([
        common_launch,
        # robot_driver_launch,
    ])