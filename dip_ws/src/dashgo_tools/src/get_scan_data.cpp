#include "ros/ros.h"
#include <math.h>
#include "sensor_msgs/PointCloud.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point32.h"
#include "sensor_msgs/LaserScan.h"
#include <tf/transform_listener.h>
#include <geometry_msgs/PointStamped.h>

#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/static_transform_broadcaster.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2/convert.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "tf2/LinearMath/Transform.h"
#include "tf2/LinearMath/Vector3.h"
#include "tf2/LinearMath/Transform.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/transform_datatypes.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

ros::Publisher scan_cloud_pub;
int laser_count=0,throttle_scans=1;
tf2_ros::Buffer tfBuffer;
tf2_ros::TransformListener* listener=NULL;

void scanCallback(const sensor_msgs::LaserScan msg)
{
    laser_count++;
    if((laser_count%throttle_scans)!=0)
    {
        return;
    }
    //tf2_ros::TransformListener listener;
    //listener.waitForTransform("/map", "laser_frame", ros::Time(0), ros::Duration(10.0));
    sensor_msgs::PointCloud android_scan_data;
    android_scan_data.header.frame_id="/map";
    float current_point_angle,current_point_distance;
    geometry_msgs::PointStamped origin_scan_point, scan_toMap_point;
    origin_scan_point.header.frame_id="laser_frame";

    geometry_msgs::Point32 target_point;

    int origin_scan_len=msg.ranges.size();
    for(int i=0;i<origin_scan_len;i++)
    {
        current_point_angle=msg.angle_min+i*msg.angle_increment;
        current_point_distance=msg.ranges[i];
        
        origin_scan_point.point.x=current_point_distance*cos(current_point_angle);
        origin_scan_point.point.y=current_point_distance*sin(current_point_angle);
        //listener.transformPoint("/map",origin_scan_point,scan_toMap_point);
        // listener->transformPoint("/map",origin_scan_point,scan_toMap_point);
        tf2::doTransform(origin_scan_point, scan_toMap_point, tfBuffer.lookupTransform("map", "laser_frame", ros::Time(0)));

        target_point.x=scan_toMap_point.point.x;
        target_point.y=scan_toMap_point.point.y;
        android_scan_data.points.push_back(target_point);
    }
    scan_cloud_pub.publish(android_scan_data);
}

int main(int argc,char** argv)
{
    ros::init(argc, argv, "get_scan_data");
    ros::NodeHandle n;
    // listener=new (tf::TransformListener);
    // listener->waitForTransform("/map", "laser_frame", ros::Time(0), ros::Duration(10.0));    
    tf2_ros::TransformListener tfListener(tfBuffer);
    listener=&tfListener;
    ros::Duration(1.0).sleep(); // 等待tf缓存数据准备好
    
    scan_cloud_pub=n.advertise<sensor_msgs::PointCloud>("/android_scan", 5);
    ros::Subscriber origin_scan_data = n.subscribe("/scan", 1, scanCallback);

    ros::Rate loop_rate(10);
    ros::spin();
    return 0;
}
