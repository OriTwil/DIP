#include "ros/ros.h"
#include "geometry_msgs/Twist.h"


int main(int argc, char **argv)
{

  ros::init(argc, argv, "my_teleop");

  ros::NodeHandle n;


  ros::Publisher teleop_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {

    geometry_msgs::Twist msg;
    msg.linear.x=0.3; //设置机器人线速度为0.3 
    msg.angular.z=0.3; //设置机器人角速度为0.3 

    teleop_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}

