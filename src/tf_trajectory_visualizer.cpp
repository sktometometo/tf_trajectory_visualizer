/*
 * MIT License
 * 
 * Copyright (c) 2020 Koki Shinjo
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <ros/ros.h>

#include <geometry_msgs/TransformStamped.h>
#include <visualization_msgs/Marker.h>

#include <tf2_ros/transform_listener.h>

int main( int argc, char** argv )
{
    ros::init(argc, argv, "tf_trajectory_visualizer");
    ros::NodeHandle nh;

    std::string frame_id_fixed;
    std::string frame_id_moved;
    std::string topic_trajectory;

    nh.param<std::string>("frame_id_fixed", frame_id_fixed, "odom");
    nh.param<std::string>("frame_id_moved", frame_id_moved, "base_link");
    nh.param<std::string>("topic_trajectory", topic_trajectory, "/tf_trajectory");

    ros::Publisher trajectory_pub = nh.advertise<visualization_msgs::Marker>(topic_trajectory,10);

    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);

    visualization_msgs::Marker trajectory;
    trajectory.header.frame_id = frame_id_fixed;
    trajectory.ns = "tf_trajectory";
    trajectory.id = 1001;
    trajectory.type = visualization_msgs::Marker::LINE_STRIP;
    trajectory.action = visualization_msgs::Marker::ADD;

    trajectory.pose.position.x = 0.0;
    trajectory.pose.position.y = 0.0;
    trajectory.pose.position.z = 0.0;
    trajectory.pose.orientation.x = 0.0;
    trajectory.pose.orientation.y = 0.0;
    trajectory.pose.orientation.z = 0.0;
    trajectory.pose.orientation.w = 1.0;

    trajectory.scale.x = 0.05;
    trajectory.scale.y = 0.0;
    trajectory.scale.z = 0.0;

    trajectory.color.r = 0.0f;
    trajectory.color.g = 1.0f;
    trajectory.color.b = 0.0f;
    trajectory.color.a = 1.0;

    geometry_msgs::Point point;
    geometry_msgs::Point prepoint;

    ros::Rate rate(10.0);
    bool firstloop=true;
    while(nh.ok()) {
        geometry_msgs::TransformStamped tfStamped;

        try{
            tfStamped = tfBuffer.lookupTransform( frame_id_fixed, frame_id_moved, ros::Time(0) );
        } catch (tf2::TransformException &ex) {
            ROS_WARN("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }

        point.x = tfStamped.transform.translation.x;
        point.y = tfStamped.transform.translation.y;
        point.z = tfStamped.transform.translation.z;

        if ( not firstloop ) {
            trajectory.points.push_back(prepoint);
            trajectory.points.push_back(point);
            trajectory_pub.publish(trajectory);
        } else {
            firstloop=false;
        }

        prepoint = point;

        rate.sleep();
    }
}
