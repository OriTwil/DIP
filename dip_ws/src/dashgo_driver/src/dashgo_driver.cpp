#include <iostream>
#include "serial/serial.h"
#include "ros/ros.h"
#include <string>
#include <sstream>
#include <vector>
#include "geometry_msgs/Twist.h"
#include <math.h>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Quaternion.h"
#include <stdlib.h>
#include "tf/transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int16.h"
#include "sensor_msgs/Range.h"
#include "sensor_msgs/PointCloud.h"
#include <sstream>
#include <math.h>

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


using namespace std;

const double ODOM_POSE_COVARIANCE[] = { 1e-3, 0, 0, 0, 0, 0,
                                0, 1e-3, 0, 0, 0, 0,
                                0, 0, 1e6, 0, 0, 0,
                                0, 0, 0, 1e6, 0, 0,
                                0, 0, 0, 0, 1e6, 0,
                                0, 0, 0, 0, 0, 1e3
                              };
const double ODOM_TWIST_COVARIANCE[] ={ 1e-3, 0, 0, 0, 0, 0,
                                0, 1e-3, 0, 0, 0, 0,
                                0, 0, 1e6, 0, 0, 0,
                                0, 0, 0, 1e6, 0, 0,
                                0, 0, 0, 0, 1e6, 0,
                                0, 0, 0, 0, 0, 1e3
                              };

class RobotStm32Driver
{
public:
    RobotStm32Driver();
    ~RobotStm32Driver();
    //连接底盘STM32串口
    bool connectRobot();
    //获取底盘STM32传感器的数据
    void getRobotData(const ros::TimerEvent&);
    //将从STM32串口中获取的二进制字符流转成16进制字符
    string string_to_hex(const std::string& input);
    //获取底盘STM32的串口波特率，用于确认导航模块与STM32是否通讯正常
    int get_baud();
    //获取底盘左右轮编码器数据
    void get_encoder(uint16_t& leftEncode,uint16_t& rightEncode);
    //获取陀螺仪imu数据
    void get_imu(float& imu_angle,float& yaw_vel);
    //获取超声波数据
    void get_sonars(float* sonar_array);
    //速度主题回调函数，将线速度和角速度转换成左右电机的速度
    void controlCallback(const geometry_msgs::Twist::ConstPtr& msg);
    //往STM32中发左右电机速度
    void driverRobot(int16_t& left_pwm,int16_t& right_pwm);
    //将STM32中获取的消息解析出来
    void RecData(uint8_t* recVaildData);
    //重置底盘编码器，置0
    void set_encode();
    //重置陀螺仪Imu，角度置0
    void set_Imu();

    void isPassedCallback(const std_msgs::Int16::ConstPtr& msg);
    
private:
    //定义ros 节点
    ros::NodeHandle g_nh,nh;
    //定义串口变量
    serial::Serial ser;
    //定义串口名，默认为"/dev/port1"
    std::string serialPort;
    //定义串口波特率
    int serial_baud;
    bool useImu;
    //定义左右轮码盘数变脸
    uint16_t leftEncode,rightEncode;
    //定义陀螺仪角度，角速度
    float imu_angle,yaw_vel;
    //陀螺仪的线加速度，角加速度变量
    int16_t x_accel,y_accel,z_accel;
    //定义中心线速度，角速度，左轮线速度，右轮线速度
    float vel_x,vel_w,left_vel_x,right_vel_x;
    //定义两轮子间距，轮子直径，轮子转动一圈码盘数，动力系数
    float wheel_track,wheel_diameter,encoder_resolution,gear_reduction;
    //左右轮周期内转动码盘数
    int16_t left_pwm,right_pwm;

    int16_t sonar0,sonar1,sonar2,sonar3,sonar4,sonar5,sonar6;
    //定义码盘最小值，最大值，后退溢出的临界值，前进溢出的临界值
    int encoder_min,encoder_max,encoder_low_wrap,encoder_high_wrap;
    //左右轮转动的圈数
    int l_wheel_mult,r_wheel_mult;
    //上一时刻的左右轮编码器值
    int last_leftEncode,last_rightEncode;
    float dleft,dright,dxy_center,dw_center,vxy_center,vw_center;
    float current_dx,current_dy,current_dw;
    float period;

    nav_msgs::Odometry odom;
    geometry_msgs::Quaternion odom_quaternion,imu_tf_quaternion;
    ros::Timer get_robot_data_timer1;
    tf2_ros::TransformBroadcaster odomBroadcaster;
    tf2_ros::TransformBroadcaster imuBroadcaster;
    geometry_msgs::TransformStamped  odom_transform;
    ros::Publisher odomPub;

    geometry_msgs::TransformStamped  imu_transform;
    ros::Publisher imuPub,imuAnglePub;
    ros::Publisher sonarPub[6],sonarCloudPointPub;    

    bool isPassed;
    struct sonar_location
    {
      float sonar_offset_yaw;
      float sonar_offset_x;
      float sonar_offset_y;
    }sonar_location[6];

};

RobotStm32Driver::RobotStm32Driver():g_nh("~")
{
    //cout<<"in RobotStm32Driver"<<endl;
    leftEncode=rightEncode=0;
    imu_angle=yaw_vel=0.0;
    x_accel=x_accel=z_accel=0;
    left_pwm=right_pwm=0;
    //wheel_diameter=0.162;
    //wheel_track=0.33;
    //encoder_resolution=610;
    g_nh.param<float>("wheel_diameter",wheel_diameter,0.162);
    g_nh.param<float>("wheel_track",wheel_track,0.33);
    g_nh.param<float>("encoder_resolution",encoder_resolution,610);

    encoder_min=0;
    encoder_max=65535;
    l_wheel_mult=0;
    r_wheel_mult=0;
    encoder_low_wrap=(encoder_max-encoder_min)*0.3+encoder_min;
    encoder_high_wrap=(encoder_max-encoder_min)*0.7+encoder_min;
    //cout<<"encoder_low_wrap="<<encoder_low_wrap<<",encoder_high_wrap="
    //    <<encoder_high_wrap<<endl;

    dleft=dright=dxy_center=dw_center=vxy_center=vw_center=0.0;
    current_dx=current_dy=current_dw=0.0;
    last_leftEncode=last_rightEncode=0;
    period=1/30.0;

    g_nh.param<std::string>("port",serialPort,"/dev/ttyUSB0");
    g_nh.param<int>("baud",serial_baud,115200);
    g_nh.param<bool>("useImu",useImu,false);
    
    isPassed=true;

    g_nh.param<float>("sonar0_offset_yaw",sonar_location[0].sonar_offset_yaw,0.0);
    g_nh.param<float>("sonar0_offset_x",sonar_location[0].sonar_offset_x,0.0);
    g_nh.param<float>("sonar0_offset_y",sonar_location[0].sonar_offset_y,0.0);

    g_nh.param<float>("sonar1_offset_yaw",sonar_location[1].sonar_offset_yaw,0.0);
    g_nh.param<float>("sonar1_offset_x",sonar_location[1].sonar_offset_x,0.0);
    g_nh.param<float>("sonar1_offset_y",sonar_location[1].sonar_offset_y,0.0);

    g_nh.param<float>("sonar2_offset_yaw",sonar_location[2].sonar_offset_yaw,0.0);
    g_nh.param<float>("sonar2_offset_x",sonar_location[2].sonar_offset_x,0.0);
    g_nh.param<float>("sonar2_offset_y",sonar_location[2].sonar_offset_y,0.0);

    g_nh.param<float>("sonar3_offset_yaw",sonar_location[3].sonar_offset_yaw,0.0);
    g_nh.param<float>("sonar3_offset_x",sonar_location[3].sonar_offset_x,0.0);
    g_nh.param<float>("sonar3_offset_y",sonar_location[3].sonar_offset_y,0.0);

    g_nh.param<float>("sonar4_offset_yaw",sonar_location[4].sonar_offset_yaw,0.0);
    g_nh.param<float>("sonar4_offset_x",sonar_location[4].sonar_offset_x,0.0);
    g_nh.param<float>("sonar4_offset_y",sonar_location[4].sonar_offset_y,0.0);

    g_nh.param<float>("sonar5_offset_yaw",sonar_location[5].sonar_offset_yaw,0.0);
    g_nh.param<float>("sonar5_offset_x",sonar_location[5].sonar_offset_x,0.0);
    g_nh.param<float>("sonar5_offset_y",sonar_location[5].sonar_offset_y,0.0);

    if(connectRobot())
    {
        cout<<"connect robot ok"<<endl;
    }
    else
    {
        cout<<"connect robot fail"<<endl;
        return ;
    }

    set_encode();
    set_Imu();
    
    //里程计odom消息发布器
    odomPub=nh.advertise<nav_msgs::Odometry>("odom",5);
    //陀螺仪Imu消息发布器
    imuPub=nh.advertise<sensor_msgs::Imu>("imu",5);
    imuAnglePub=nh.advertise<std_msgs::Float32>("imu_angle",5);

    for(int i=0;i<6;i++)
    {
      stringstream topic_name;
      topic_name<<"/sonar"<<i;
      sonarPub[i]=nh.advertise<sensor_msgs::Range>(topic_name.str(),5);
    }
    sonarCloudPointPub=nh.advertise<sensor_msgs::PointCloud>("/sonar_cloudpoint",5);
    //get_robot_data_timer1=g_nh.createTimer(ros::Duration(period),boost::bind(&RobotStm32Driver::getRobotData,this,_1));
    get_robot_data_timer1=g_nh.createTimer(ros::Duration(0.05),boost::bind(&RobotStm32Driver::getRobotData,this,_1));
    
    //ros::Subscriber sub=nh.subscribe<geometry_msgs::Twist>("/cmd_vel",1,boost::bind(&RobotStm32Driver::controlCallback,this,_1));
    ros::Subscriber sub=nh.subscribe<geometry_msgs::Twist>("smoother_cmd_vel",1,boost::bind(&RobotStm32Driver::controlCallback,this,_1));
    
    ros::Subscriber laserFilter=nh.subscribe<std_msgs::Int16>("is_passed",1,boost::bind(&RobotStm32Driver::isPassedCallback,this,_1));
    ros::spin();
}
/*
*接收STM32 返回的消息，并提取出有效数据recVaildData[i]
*/
void RobotStm32Driver::RecData(uint8_t* recVaildData)
{
  int out=0,len=0;
  //uint8_t recVaildData[20]={0};

  string head0 = ser.read(1);
  string head0_hex=string_to_hex(head0);
  //cout<<"head0_hex="<<head0_hex<<endl;
  if(head0_hex=="FF")
  {
      string head1 = ser.read(1);
      string head1_hex=string_to_hex(head1);
      //cout<<"head1_hex="<<head1_hex<<endl;
      if(head1_hex=="AA")
      {
          string dataLen=ser.read(1);
          string dataLen_hex=string_to_hex(dataLen);
          //cout<<"dataLen_hex="<<dataLen_hex<<endl;
          sscanf(dataLen_hex.c_str(),"%02x",&len);
          //cout<<"len ="<<len<<endl;

          string dataRes=ser.read(1);
          string dataRes_hex=string_to_hex(dataRes);
          //cout<<"dataRes_hex="<<dataRes_hex<<endl;
          for(int i=0;i<len-1;i++)
          {
            string vaildData=ser.read(1);
            string vaildData_hex=string_to_hex(vaildData);
            //cout<<"get encode vaildData_hex="<<vaildData_hex<<endl;

            sscanf(vaildData_hex.c_str(),"%02x",&recVaildData[i]);
          }
          //leftEncode=recVaildData[0]+(recVaildData[1]<<8);
          //rightEncode=recVaildData[2]+(recVaildData[3]<<8);
          //cout<<"***leftEncode="<<leftEncode<<" ,rightEncode="<<
           //   rightEncode<<endl;

          string check_sum=ser.read(1);
          string check_sum_hex=string_to_hex(check_sum);
          //cout<<"check_sum_hex="<<check_sum_hex<<endl;
      }
  }
}
/*
* 给STM32 下发重置编码器的指令，清0
*/
void RobotStm32Driver::set_encode()
{
    int8_t data[20]={(int8_t)0xff,(int8_t)0xaa,(int8_t)0x01,(int8_t)0x03,(int8_t)0x04};
    ser.write((const uint8_t*)data,5);

    uint8_t responseData[20]={0};
    RecData(responseData);
}
/*
* 给STM32 下发重置陀螺仪Imu的指令，角度值清0
*/
void RobotStm32Driver::set_Imu()
{
    int8_t data[20]={(int8_t)0xff,(int8_t)0xaa,(int8_t)0x01,(int8_t)0x14,(int8_t)0x15};
    ser.write((const uint8_t*)data,5);

    uint8_t responseData[20]={0};
    RecData(responseData);
}

/*
*连接底盘STM32串口，波特率为115200
*/
bool RobotStm32Driver::connectRobot()
{
    try
    {        
        ser.setPort(serialPort);
        cout<<"connect serial port is "<<serialPort<<endl;
        ser.setBaudrate(serial_baud);
        serial::Timeout to=serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
        return true;

    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("unable to open serial port "<<ser.getPort()
                         <<",fail.");
        ros::Duration(1).sleep();
        return false;
    }
}

/*
*将获取到的传感器数据，转换成相应ROS的消息，并发布出来
*该函数是定时器回调函数，每0.1s执行一次 
*/
void RobotStm32Driver::getRobotData(const ros::TimerEvent&)
{
    //cout<<"start get robot data"<<endl;
    //获取波特率值，确保导航模块与STM32串口通讯正常
    if(ser.isOpen())
    {
        int baud=get_baud();
        //cout<<"baud= "<<baud<<endl;
    }
    //获取左右轮编码器值
    get_encoder(leftEncode,rightEncode);
    //cout<<"left_enc="<<leftEncode<<" ,right_enc= "<<rightEncode<<endl;
    //计算左轮在这周期内移动的距离
    //后退超过0-》65535,65534-1=65533,真实应为-4个脉冲
    if(leftEncode-last_leftEncode>encoder_max/2)
    {
        dleft=1.0*(leftEncode-last_leftEncode-encoder_max)/(encoder_resolution/(M_PI*wheel_diameter));
    } //前进 65535-》0,1-65533=-65532,真实应为4个脉冲
    else if(leftEncode-last_leftEncode< -encoder_max/2)
    {
        dleft=1.0*(leftEncode-last_leftEncode+encoder_max)/(encoder_resolution/(M_PI*wheel_diameter));
    }
    else
    {
        dleft=1.0*(leftEncode-last_leftEncode)/(encoder_resolution/(M_PI*wheel_diameter));
    }
    //计算右轮子轮在这周期内移动的距离
    if(rightEncode-last_rightEncode>encoder_max/2)
    {
        dright=1.0*(rightEncode-last_rightEncode-encoder_max)/(encoder_resolution/(M_PI*wheel_diameter));
    }
    else if(rightEncode-last_rightEncode< -encoder_max/2)
    {
        dright=1.0*(rightEncode-last_rightEncode+encoder_max)/(encoder_resolution/(M_PI*wheel_diameter));
    }
    else
    {
        dright=1.0*(rightEncode-last_rightEncode)/(encoder_resolution/(M_PI*wheel_diameter));
    }
    last_leftEncode=leftEncode;
    last_rightEncode=rightEncode;
    //cout<<"dleft= "<<dleft<<",dright="<<dright<<endl;
    //计算机器人中心行走距离
    dxy_center=(dleft+dright)/2.0; 
    //计算机器人中心转动的弧度  
    dw_center=(dright-dleft)/wheel_track;
    //cout<<"***dxy_center="<<dxy_center<<", dw_center="<<dw_center<<endl;
    //计算出机器人中心的线速度和角速度
    vxy_center=dxy_center/0.1;
    vw_center=dw_center/0.1;

    if(dxy_center !=0)
    {   //分别计算出机器人在x轴和y轴在这周期内距离增量，并累加
        current_dx +=cos(current_dw+dw_center/2.0)*dxy_center;
        current_dy +=sin(current_dw+dw_center/2.0)*dxy_center;
        //cout<<"current_dx="<<current_dx<<",current_dy="\
         //   <<current_dy<<endl; 
    }
    if(dw_center !=0)
    {   //角度增量累加
        current_dw +=dw_center;
    }
    //指定里程计消息坐标系为odom，它的子坐标系为机器人坐标系base_footprint
    //对ROS odom消息进行赋值
    odom.header.frame_id="odom";
    odom.child_frame_id="base_footprint";
    odom.header.stamp=ros::Time::now();
    odom.pose.pose.position.x=current_dx;
    odom.pose.pose.position.y=current_dy;
    odom.pose.pose.position.z=0;
    //将转动的角度current_dw，变换成四元数表示
    //odom_quaternion=tf::createQuaternionFromYaw(current_dw);
    odom_quaternion.x=0;
    odom_quaternion.y=0;
    odom_quaternion.z=sin(current_dw/2.0);//由欧拉角转四元数公式可以得到
    odom_quaternion.w=cos(current_dw/2.0);

    odom.pose.pose.orientation=odom_quaternion;
    odom.twist.twist.linear.x=vxy_center;
    odom.twist.twist.linear.y=0;
    odom.twist.twist.angular.z=vw_center;

    //协防差矩阵，如果在此不初始化，有可能导致robot_pose_ekf 卡尔曼滤波时会出错
    for(int i=0;i<36;i++)
    {
        odom.pose.covariance[i]=ODOM_POSE_COVARIANCE[i];
    }
    for(int i=0;i<36;i++)
    {
        odom.twist.covariance[i]=ODOM_TWIST_COVARIANCE[i];
    }
    //如果不打算融合陀螺仪，则需要在此发布tf，否则由robot_pose_ekf发布里程计的tf
    if(useImu == false)
    {
        //cout<<"not use imu"<<endl;
    //发布odom 与base_footprint 的tf转换关系
        odom_transform.header.stamp=ros::Time::now();
        odom_transform.header.frame_id="odom";
        odom_transform.child_frame_id ="base_footprint";
        odom_transform.transform.translation.x=current_dx;
        odom_transform.transform.translation.y=current_dy;
        odom_transform.transform.translation.z=0.0;
        odom_transform.transform.rotation=odom_quaternion;    
        odomBroadcaster.sendTransform(odom_transform);
    }


    odomPub.publish(odom);

    //定义ROS Imu消息变量和四元数变量
    sensor_msgs::Imu imu_data;
    geometry_msgs::Quaternion imu_quaternion;
    //获取陀螺仪角度值和角速度
    get_imu(imu_angle,yaw_vel);
    //cout<<"imu_angle="<<imu_angle<<",yaw_vel="<<yaw_vel<<endl;
    //指定Imu的tf坐标系为imu_frame_id
    imu_data.header.stamp=ros::Time::now();
    imu_data.header.frame_id="imu_base";
    //设置协方差
    imu_data.orientation_covariance[0]=1000000;
    imu_data.orientation_covariance[1]=0;
    imu_data.orientation_covariance[2]=0;
    imu_data.orientation_covariance[3]=0;
    imu_data.orientation_covariance[4]=1000000;
    imu_data.orientation_covariance[5]=0;
    imu_data.orientation_covariance[6]=0;
    imu_data.orientation_covariance[7]=0;
    imu_data.orientation_covariance[8]=1e-6; //1000000;
    //将陀螺仪的角度值转换成四元数表示
    // imu_quaternion=tf::createQuaternionMsgFromYaw(-1*imu_angle*3.1416/180);
    tf2::Quaternion quat;
    quat.setRPY(0, 0, -1*imu_angle*3.1416/180);
    imu_quaternion = tf2::toMsg(quat);
    //imu_quaternion.x=0.0;
    //imu_quaternion.y=0.0;
    //imu_quaternion.z=sin(-1*imu_angle*3.1416/(180*2.0));
    //imu_quaternion.w=cos(-1*imu_angle*3.1416/(180*2.0));
    imu_data.orientation=imu_quaternion;
    //float imu_yaw=tf2::getYaw(imu_quaternion);
    //cout<<"imu_yaw= "<<imu_yaw<<endl;

    imu_data.linear_acceleration_covariance[0]=1000000;
    imu_data.linear_acceleration_covariance[1]=0;
    imu_data.linear_acceleration_covariance[2]=0;
    imu_data.linear_acceleration_covariance[3]=0;
    imu_data.linear_acceleration_covariance[4]=1000000;
    imu_data.linear_acceleration_covariance[5]=0;
    imu_data.linear_acceleration_covariance[6]=0;
    imu_data.linear_acceleration_covariance[7]=0;
    imu_data.linear_acceleration_covariance[8]=1e-6;//1000000;
    
    imu_data.angular_velocity_covariance[0]=1000000;
    imu_data.angular_velocity_covariance[0]=0;
    imu_data.angular_velocity_covariance[0]=0;
    imu_data.angular_velocity_covariance[0]=0;
    imu_data.angular_velocity_covariance[0]=1000000;
    imu_data.angular_velocity_covariance[0]=0;
    imu_data.angular_velocity_covariance[0]=0;
    imu_data.angular_velocity_covariance[0]=0;
    imu_data.angular_velocity_covariance[0]=1e-6;//1000000;
    
    imu_data.linear_acceleration.x=-y_accel/100.0 + 0.13;
    imu_data.linear_acceleration.y=x_accel/100.0 + 0.06;
    imu_data.linear_acceleration.z=z_accel/100.0;
    
    imu_data.angular_velocity.x=0.0;
    imu_data.angular_velocity.y=0.0;
    imu_data.angular_velocity.z=1*yaw_vel*3.1416/(180*100);
#if 0  //动态发布imu 与 base_footprint 的关系 
    imu_transform.header.stamp=ros::Time::now();
    imu_transform.header.frame_id="imu_base";
    imu_transform.child_frame_id ="base_footprint";
    imu_transform.transform.translation.x=0;
    imu_transform.transform.translation.y=0;
    imu_transform.transform.translation.z=0.0;
    
    imu_tf_quaternion.x=0;
    imu_tf_quaternion.y=0;
    imu_tf_quaternion.z=0;//由欧拉角转四元数公式可以得到
    imu_tf_quaternion.w=1; //表示没有旋转    
    imu_transform.transform.rotation=imu_tf_quaternion;    
    imuBroadcaster.sendTransform(imu_transform);
#endif     
    //发不陀螺消息
    imuPub.publish(imu_data);
    //发布陀螺仪消息，方便调试
    std_msgs::Float32 msg;
    msg.data=-1*imu_angle;
    imuAnglePub.publish(msg);

    //获取6组超声波值
    float sonar_array[6]={0.0};
    get_sonars(sonar_array);
    //cout<<"sonar0-5= "<<"("<<sonar_array[0]<<","<<sonar_array[1]
    //    <<","<<sonar_array[2]<<","<<sonar_array[3]<<","<<sonar_array[4]
    //   <<","<<sonar_array[5]<<endl;

    sensor_msgs::Range sonar_range_array[6];
    for(int i=0;i<6;i++)
    {
      sonar_range_array[i].header.stamp=ros::Time::now();
      stringstream ss;
      ss<<"/sonar"<<i;
     // cout<<"frame_id="<<ss.str()<<endl;
      sonar_range_array[i].header.frame_id=ss.str();
      sonar_range_array[i].radiation_type=sonar_range_array[i].ULTRASOUND;
      sonar_range_array[i].field_of_view=0.3;
      sonar_range_array[i].min_range=0.04;
      sonar_range_array[i].max_range=0.8;
      sonar_range_array[i].range=sonar_array[i];

      if(sonar_range_array[i].range==0.0)
      {
        sonar_range_array[i].range=1.0;
      }
      else if(sonar_range_array[i].range>=sonar_range_array[i].max_range)
      {
        sonar_range_array[i].range=sonar_range_array[i].max_range;
      }
      sonarPub[i].publish(sonar_range_array[i]);
    }

    sensor_msgs::PointCloud sonar_point_cloud;
    float sonar_maxval = 3.5;
    sonar_point_cloud.header.frame_id="/base_footprint";
    geometry_msgs::Point32 sonar_point[6];

    for(int i=0;i<6;i++)
    {
      sonar_point[i].x=sonar_location[i].sonar_offset_x+sonar_range_array[i].range*cos(sonar_location[i].sonar_offset_yaw);
      sonar_point[i].y=sonar_location[i].sonar_offset_y+sonar_range_array[i].range*sin(sonar_location[i].sonar_offset_yaw);
      sonar_point[i].z=0.15;
      sonar_point_cloud.points.push_back(sonar_point[i]);
    }
    sonarCloudPointPub.publish(sonar_point_cloud);


    //driverRobot(left_pwm,right_pwm);
    //left_pwm=right_pwm=0;
}
/*
*获取STM32 串口的波特率，用于确认导航模块与STM32的通讯正常
*/
int RobotStm32Driver::get_baud()
{
    int out=0;
    uint8_t responseData[20]={0};

    char data[20]={(char)0xff,(char)0xaa,(char)0x01,(char)0x00,(char)0x01};
    ser.write((const uint8_t*)data,5);

    RecData(responseData);
    out=responseData[0]+(responseData[1]<<8)+(responseData[2]<<16)+(responseData[3]<<24);
    return out;
}
/*
*从STM32 中获取左右轮编码器值
*/
void RobotStm32Driver::get_encoder(uint16_t& leftEncode,uint16_t& rightEncode)
{
  uint8_t responseData[20]={0};
  int8_t data[20]={(int8_t)0xff,(int8_t)0xaa,(int8_t)0x01,(int8_t)0x02,(int8_t)0x03};
  ser.write((const uint8_t*)data,5);

  RecData(responseData);
  leftEncode=responseData[0]+(responseData[1]<<8);
  rightEncode=responseData[2]+(responseData[3]<<8);
  //cout<<"***leftEncode="<<leftEncode<<" ,rightEncode="<<rightEncode<<endl;
}
/*
*从STM32 中获取陀螺仪值
*/
void RobotStm32Driver::get_imu(float& imu_angle,float& yaw_vel)
{
  char data[20]={(char)0xff,(char)0xaa,(char)0x01,(char)0x05,(char)0x06};
  ser.write((const uint8_t*)data,5);

  uint8_t responseData[20]={0};
  RecData(responseData);
  imu_angle=((int16_t)(responseData[0]+(responseData[1]<<8)))/100.0;
  yaw_vel=((int16_t)(responseData[2]+(responseData[3]<<8)))/100.0;
  x_accel=(responseData[4]+(responseData[5]<<8));
  y_accel=(responseData[6]+(responseData[7]<<8));
  z_accel=(responseData[8]+(responseData[9]<<8));

  //cout<<"yaw_angle= "<<yaw_angle<<endl;
  //return yaw_angle;
}
/*
*从STM32 中获取超声波的值
*/
void RobotStm32Driver::get_sonars(float* sonar_array)
{
    char data[20]={(char)0xff,(char)0xaa,(char)0x01,(char)0x0D,(char)0x0E};
    ser.write((const uint8_t*)data,5);

    uint8_t responseData[20]={0};
    RecData(responseData);
    sonar0=(int16_t)((responseData[0]+(responseData[1]<<8)));
    sonar1=(int16_t)((responseData[2]+(responseData[3]<<8)));
    sonar2=(int16_t)((responseData[4]+(responseData[5]<<8)));
    sonar3=(int16_t)((responseData[6]+(responseData[7]<<8)));
    sonar4=(int16_t)((responseData[8]+(responseData[9]<<8)));
    sonar5=(int16_t)((responseData[10]+(responseData[11]<<8)));

    //cout<<"sonar0= "<<sonar0<<",sonar1= "<<sonar1<<",sonar2= "<<sonar2
    //   <<",sonar3= "<<sonar3<<",sonar4= "<<sonar4<<",sonar5= "<<sonar5<<endl;
    sonar_array[0]=sonar0/100.0;
    sonar_array[1]=sonar1/100.0;
    sonar_array[2]=sonar2/100.0;
    sonar_array[3]=sonar3/100.0;
    sonar_array[4]=sonar4/100.0;
    sonar_array[5]=sonar5/100.0;
}
/*
*往STM32 中发送左右轮的速度，单位为周期内轮子要转动的码盘数
*/
void RobotStm32Driver::driverRobot(int16_t& left_pwm,int16_t& right_pwm)
{
  //cout<<"left_pwm="<<left_pwm<<",right_pwm="<<right_pwm<<endl;
  uint8_t left_g=left_pwm>>8;
  //printf("%02x\n",left_g);
  uint8_t left_d=left_pwm&0xFF;
  //printf("%02x\n",left_d);

  uint8_t right_g=right_pwm>>8;
  uint8_t right_d=right_pwm&0xFF;

  uint8_t check_sum=((uint8_t)0x05+(uint8_t)0x04+left_d+left_g+right_d+right_g)%255;

//   char data[20]={(uint8_t)0xff,(uint8_t)0xaa,(uint8_t)0x05,(uint8_t)0x04,left_d,left_g,right_d,right_g,check_sum};
  char data[20] = {
        static_cast<char>(0xff), 
        static_cast<char>(0xaa), 
        static_cast<char>(0x05), 
        static_cast<char>(0x04), 
        static_cast<char>(left_d), 
        static_cast<char>(left_g), 
        static_cast<char>(right_d), 
        static_cast<char>(right_g), 
        static_cast<char>(check_sum)
    };

  ser.write((const uint8_t*)data,9);

  uint8_t responseData[20]={0};
  RecData(responseData);
}

/*
*订阅到速度主题/cmd_vel的Twist后的回调处理函数
*作用: 将ROS的速度转换成左右轮电机的速度
*/
void RobotStm32Driver::controlCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    //cout<<"in controlCallback"<<endl;
     //从ROS速度消息中获取出中心线速度和角速度
    vel_x=msg->linear.x;
    vel_w=msg->angular.z;
    
    if((isPassed==false)&&(vel_x>0))
    {
        vel_x=0;
    }
    
     //将中心线速度和角速度 分解成左右轮的线速度，角速度
    right_vel_x=vel_x+vel_w*(wheel_track/2.0);
    left_vel_x=vel_x-vel_w*(wheel_track/2.0);
    //cout<<"vel_x="<<vel_x<<", vel_w="<<vel_w<<endl;
    //cout<<"left_vel_x="<<left_vel_x<<",right_vel_x="<<right_vel_x<<endl;
     //分别计算出左右轮周期内需要转的码盘数
    //float period=30.0;
    left_pwm=(int16_t)((left_vel_x*period)*(encoder_resolution/(M_PI*wheel_diameter)));
    right_pwm=(int16_t)((right_vel_x*period)*(encoder_resolution/(M_PI*wheel_diameter)));
    //cout<<"left_pwm="<<left_pwm<<",right_pwm="<<right_pwm<<endl;
    //往STM32 中发速度控制指令
    driverRobot(left_pwm,right_pwm);
}

void RobotStm32Driver::isPassedCallback(const std_msgs::Int16::ConstPtr& msg)
{
    if(msg->data>2)
    {
        isPassed=false;
    }
    else
    {
        isPassed=true;
    }
}

/*
*将从STM32串口中获取的二进制字符流转换成16进制字符
*/
string RobotStm32Driver::string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

RobotStm32Driver::~RobotStm32Driver()
{

}

int main(int argc, char** argv)
{
    ros::init(argc,argv,"robot_stm32_driver");
    RobotStm32Driver rd;
    //rd.connectRobot();
    //ros::spin();
    //cout << "Hello World!" << endl;
    return 0;
}
