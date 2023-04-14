#include <ros/ros.h>
#include <iostream>
#include "geometry_msgs/PointStamped.h"
#include "tf/transform_listener.h"
#include "apriltag_ros/AprilTagDetectionArray.h"


geometry_msgs::PointStamped cameral_point, target_to_dobot_point;
geometry_msgs::PointStamped release_tag_point, release_tag_to_dobot_point;

tf::TransformListener* listener=NULL;
ros::Publisher target_to_dobot_point_pub,release_tag_to_dobot_point_pub;

void cameralPointCallback(const apriltag_ros::AprilTagDetectionArray msg)
{
    //std::cout<<"msg.detections size="<<msg.detections.size()<<std::endl;
#if 0
    ROS_WARN("detections size =%d",msg.detections.size());
        if(msg.detections.size() ==0)
    {
        ROS_WARN("Not get cameral point");
        return ;
    }
    else
    {
        ROS_WARN("detections[0] point =(%f,%f)",msg.detections[0].pose.pose.pose.position.x,\
                                     msg.detections[0].pose.pose.pose.position.y);
    }
#endif
    
    if(msg.detections.size() ==0)
    {
        //ROS_WARN("Not get cameral point");
        target_to_dobot_point.header.frame_id="cameral_base";
        target_to_dobot_point.point.x=0;
        target_to_dobot_point.point.y=0;
        target_to_dobot_point.point.z=0;
        target_to_dobot_point_pub.publish(target_to_dobot_point);
        
        release_tag_point.header.frame_id="cameral_base";
        release_tag_point.point.x=0;
        release_tag_point.point.y=0;
        release_tag_point.point.z=0;
        return ;
    }
    else if(msg.detections.size() ==1 && msg.detections[0].id[0]==6)
    {
        target_to_dobot_point.header.frame_id="cameral_base";
        target_to_dobot_point.point.x=0;
        target_to_dobot_point.point.y=0;
        target_to_dobot_point.point.z=0;
        target_to_dobot_point_pub.publish(target_to_dobot_point);
        
        //ROS_WARN("id is 6, start pub release point");
        release_tag_point.header.frame_id="cameral_base";
        
        release_tag_point.point.x=msg.detections[0].pose.pose.pose.position.x;
        release_tag_point.point.y=msg.detections[0].pose.pose.pose.position.y;
        
        release_tag_point.point.x=release_tag_point.point.x/1.4265;
        release_tag_point.point.y=release_tag_point.point.y/1.0000;
        
        listener->transformPoint("dobot_base",release_tag_point,release_tag_to_dobot_point);
        release_tag_to_dobot_point_pub.publish(release_tag_to_dobot_point);
        //std::cout<<"after transform to dobot_base,release_point="<<"("<<release_tag_to_dobot_point.point.x
    //<<","<<release_tag_to_dobot_point.point.y<<")"<<std::endl;

    }
    else
    {
#if 1    
        for(int i=0;i<msg.detections.size();i++)
        {
            //ROS_WARN("id size =%d",msg.detections[i].id.size());

                //ROS_WARN("detections[%d] id is %d",i,msg.detections[i].id[0]);
                if(msg.detections[i].id[0]==6) //释放二维码的位置
                {
                    //ROS_WARN("id is 6, start pub release point");
                    release_tag_point.header.frame_id="cameral_base";
                    
                    release_tag_point.point.x=msg.detections[i].pose.pose.pose.position.x;
                    release_tag_point.point.y=msg.detections[i].pose.pose.pose.position.y;
                    
                    release_tag_point.point.x=release_tag_point.point.x/1.4265;
                    release_tag_point.point.y=release_tag_point.point.y/1.0000;

                    
                    listener->transformPoint("dobot_base",release_tag_point,release_tag_to_dobot_point);
                    release_tag_to_dobot_point_pub.publish(release_tag_to_dobot_point);
                    //std::cout<<"after transform to dobot_base,release_point="<<"("<<release_tag_to_dobot_point.point.x
                //<<","<<release_tag_to_dobot_point.point.y<<")"<<std::endl;
                    //break;
                }
                else //其他要抓取的目标点位置
                {
                    //ROS_WARN("id is 2, start pub target point");
                    cameral_point.header.frame_id="cameral_base";
                    
                    cameral_point.point.x=msg.detections[i].pose.pose.pose.position.x;
                    cameral_point.point.y=msg.detections[i].pose.pose.pose.position.y;
                    
                    //识别出来存在误差,x方向误差系数为1.4265
                    //std::cout<<"before incorrect point= "<<"("<<cameral_point.point.x<<","<<cameral_point.point.y<<")"<<std::endl;
                    cameral_point.point.x=cameral_point.point.x/1.4265;
                    cameral_point.point.y=cameral_point.point.y/1.0000;
                    
                    //std::cout<<"after incorrect point= "<<"("<<cameral_point.point.x<<","<<cameral_point.point.y<<")"<<std::endl;
                    listener->transformPoint("dobot_base",cameral_point,target_to_dobot_point);
                    //std::cout<<"origin point= "<<"("<<cameral_point.point.x<<","<<cameral_point.point.y<<")"<<std::endl;
                    
                   // std::cout<<"after transform to dobot_base,target_point="<<"("<<target_to_dobot_point.point.x
                //<<","<<target_to_dobot_point.point.y<<")"<<std::endl;
                    
                    target_to_dobot_point_pub.publish(target_to_dobot_point);
                }
            

        }
#endif

    #if 0    
        cameral_point.header.frame_id="cameral_base";
        
        cameral_point.point.x=msg.detections[0].pose.pose.pose.position.x;
        cameral_point.point.y=msg.detections[0].pose.pose.pose.position.y;
        
        //识别出来存在误差
        //std::cout<<"before incorrect point= "<<"("<<cameral_point.point.x<<","<<cameral_point.point.y<<")"<<std::endl;
        cameral_point.point.x=cameral_point.point.x/1.4265;
        cameral_point.point.y=cameral_point.point.y/1.0000;
        
        //std::cout<<"after incorrect point= "<<"("<<cameral_point.point.x<<","<<cameral_point.point.y<<")"<<std::endl;
        listener->transformPoint("dobot_base",cameral_point,target_to_dobot_point);
        //std::cout<<"origin point= "<<"("<<cameral_point.point.x<<","<<cameral_point.point.y<<")"<<std::endl;
        /*
        std::cout<<"after transform to dobot_base,target_point="<<"("<<target_to_dobot_point.point.x
    <<","<<target_to_dobot_point.point.y<<")"<<std::endl;
        */
        target_to_dobot_point_pub.publish(target_to_dobot_point);
    #endif
    }
}

int main(int argc, char** argv)
{
    ros::init(argc,argv,"apriltag_ros_static_cameral_point_to_dobot");
    ros::NodeHandle n_;
    ros::Rate loop(3);
    
    listener=new (tf::TransformListener);
	listener->waitForTransform("dobot_base","cameral_base",ros::Time(0),ros::Duration(10.0));
	
	target_to_dobot_point_pub=n_.advertise<geometry_msgs::PointStamped>("/target_to_dobot_point",5);
	release_tag_to_dobot_point_pub=n_.advertise<geometry_msgs::PointStamped>("/release_tag_to_dobot_point",5);
	ros::Subscriber cameral_point_sub=n_.subscribe("/tag_detections",1,cameralPointCallback);
	loop.sleep();
	//ros::spin();
	ros::AsyncSpinner spinner(boost::thread::hardware_concurrency());
    spinner.start();
    ros::waitForShutdown();


}
