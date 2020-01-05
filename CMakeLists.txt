cmake_minimum_required(VERSION 2.8.3)
project(tf_trajectory_visualizer)

find_package(catkin REQUIRED COMPONENTS 
    geometry_msgs
    roscpp
    tf2
    tf2_ros
    visualization_msgs
)
set(ROS_COMPILE_FLAGS "-W -Wall -Wextra -pedantic")

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)
link_directories(
    ${catkin_LIBRARY_DIRS}
    ${PCL_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIRS}
)
add_definitions(${PCL_DEFINITIONS})

catkin_package()

add_executable(tf_trajectory_visualizer 
    src/tf_trajectory_visualizer.cpp
)
target_link_libraries(tf_trajectory_visualizer
    ${catkin_LIBRARIES}
)
