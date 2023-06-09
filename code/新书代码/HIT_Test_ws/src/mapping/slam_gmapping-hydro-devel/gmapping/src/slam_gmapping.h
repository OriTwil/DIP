/*
 * slam_gmapping
 * Copyright (c) 2008, Willow Garage, Inc.
 *
 * THE WORK (AS DEFINED BELOW) IS PROVIDED UNDER THE TERMS OF THIS CREATIVE
 * COMMONS PUBLIC LICENSE ("CCPL" OR "LICENSE"). THE WORK IS PROTECTED BY
 * COPYRIGHT AND/OR OTHER APPLICABLE LAW. ANY USE OF THE WORK OTHER THAN AS
 * AUTHORIZED UNDER THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 * 
 * BY EXERCISING ANY RIGHTS TO THE WORK PROVIDED HERE, YOU ACCEPT AND AGREE TO
 * BE BOUND BY THE TERMS OF THIS LICENSE. THE LICENSOR GRANTS YOU THE RIGHTS
 * CONTAINED HERE IN CONSIDERATION OF YOUR ACCEPTANCE OF SUCH TERMS AND
 * CONDITIONS.
 *
 */

/* Author: Brian Gerkey */

#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/Float64.h"
#include "nav_msgs/GetMap.h"
#include "tf/transform_listener.h"
#include "tf/transform_broadcaster.h"
#include "message_filters/subscriber.h"
#include "tf/message_filter.h"

#include "gmapping/gridfastslam/gridslamprocessor.h"
#include "gmapping/sensor/sensor_base/sensor.h"

#include <boost/thread.hpp>

class SlamGMapping
{
  public:
    SlamGMapping();
    SlamGMapping(ros::NodeHandle& nh, ros::NodeHandle& pnh);
    SlamGMapping(unsigned long int seed, unsigned long int max_duration_buffer);
    ~SlamGMapping();

    void init();
    void startLiveSlam();
    void startReplay(const std::string & bag_fname, std::string scan_topic);
    void publishTransform();
  
    void laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
    bool mapCallback(nav_msgs::GetMap::Request  &req,
                     nav_msgs::GetMap::Response &res);
    void publishLoop(double transform_publish_period);

  private:
    ros::NodeHandle node_;
    ros::Publisher entropy_publisher_;
    ros::Publisher sst_;
    ros::Publisher sstm_;
    ros::ServiceServer ss_;
    tf::TransformListener tf_;
    message_filters::Subscriber<sensor_msgs::LaserScan>* scan_filter_sub_;
    //记录转换到odom坐标系后的激光雷达数据
    tf::MessageFilter<sensor_msgs::LaserScan>* scan_filter_;
    tf::TransformBroadcaster* tfB_;

    GMapping::GridSlamProcessor* gsp_;
    GMapping::RangeSensor* gsp_laser_;
    // The angles in the laser, going from -x to x (adjustment is made to get the laser between
    // symmetrical bounds as that's what gmapping expects)
    //激光的角度数组
    std::vector<double> laser_angles_;
    // The pose, in the original laser frame, of the corresponding centered laser with z facing up
    //在原始激光框架中，相应的z面朝上的中心激光的姿态
    tf::Stamped<tf::Pose> centered_laser_pose_;
    // Depending on the order of the elements in the scan and the orientation of the scan frame,
    // We might need to change the order of the scan
    bool do_reverse_range_;
    unsigned int gsp_laser_beam_count_;
    GMapping::OdometrySensor* gsp_odom_;

    bool got_first_scan_;

    bool got_map_;
    nav_msgs::GetMap::Response map_;

    ros::Duration map_update_interval_;
    //map 到odom 的tf转换信息
    tf::Transform map_to_odom_;
    boost::mutex map_to_odom_mutex_;
    boost::mutex map_mutex_;

    //激光限流作用,每隔throttle_scans_ （默认值 1）帧数据计算一次
    int laser_count_;   
    int throttle_scans_;

    boost::thread* transform_thread_;

    std::string base_frame_;
    std::string laser_frame_;
    std::string map_frame_;
    std::string odom_frame_;

    void updateMap(const sensor_msgs::LaserScan& scan);
    bool getOdomPose(GMapping::OrientedPoint& gmap_pose, const ros::Time& t);
    bool initMapper(const sensor_msgs::LaserScan& scan);
    bool addScan(const sensor_msgs::LaserScan& scan, GMapping::OrientedPoint& gmap_pose);
    double computePoseEntropy();
    
    // Parameters used by GMapping
    //激光雷达的最大距离和最大使用距离
    double maxRange_;
    double maxUrange_;

    double maxrange_;

    /*scan-matching结果接受的最小得分*/
    double minimum_score_;
    double sigma_;
    int kernelSize_;

    //scan-matching的过程中的初始的搜索步长和迭代次数
    double lstep_;
    double astep_;
    int iterations_;

    //计算在likelihoodandscore()函数中 计算likelihood使用用的方差(不是标准差)
    //likelihood是指高斯分布的指数项的系数，即e的指数
    double lsigma_;
    double ogain_;

    /*对于一帧激光雷达数据来说 只取每第(n+1)个激光束  这个是相对于scan-match来说的。
如果n等于0 则取每第1帧激光束
如果n等于1 则取每第2帧激光束 也就是说使用的激光束变成原来的1/2
如果n等于2 则取每第3帧激光束 也就是说使用的激光束变成原来的1/3*/
    int lskip_;
/*机器人的运动模型的相关的噪声参数*/
    double srr_;
    double srt_;
    double str_;
    double stt_;
/*进行滤波器更新的最小距离*/
    double linearUpdate_;
    double angularUpdate_;

    /*粒子滤波器的相关参数*/
    double temporalUpdate_;
    double resampleThreshold_;
    int particles_;
    /*初始时候地图的维度和分辨率*/
    double xmin_;
    double ymin_;
    double xmax_;
    double ymax_;
    double delta_;
    double occ_thresh_;
    double llsamplerange_;
    double llsamplestep_;
    double lasamplerange_;
    double lasamplestep_;
    
    ros::NodeHandle private_nh_;
    
    unsigned long int seed_;
    
    double transform_publish_period_;
    double tf_delay_;
};
