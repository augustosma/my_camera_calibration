#!/bin/bash

rosrun my_camera_calibration my_camera_calibration_node &
rosbag play /catkin_ws/src/eth/cam_checkboard.bag -s 8

 
