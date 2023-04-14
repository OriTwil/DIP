#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "serial/serial.h"
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>

using namespace std;

//定义串口变量
serial::Serial ser;
//定义中心线速度，角速度，左轮线速度，右轮线速度
float vel_x,vel_w,left_vel_x,right_vel_x;
//定义轮子直径
float wheel_diameter=0.162;
//定义两轮子间距
float wheel_track=0.33;
//定义轮子转动一圈码盘数
float encoder_resolution=610;
//左右轮周期内转动码盘数
int16_t left_pwm,right_pwm;
//控制周期
float period=1/30.0;

/*
*连接底盘STM32串口，波特率为115200
*/
bool connectRobot()
{
    try
    {        
        ser.setPort("/dev/port1");
        ser.setBaudrate(115200);
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
*将从STM32串口中获取的二进制字符流转换成16进制字符
*/
string string_to_hex(const std::string& input)
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

/*
*接收STM32 返回的消息，并提取出有效数据recVaildData[i]
*/
void RecData(uint8_t* recVaildData)
{
  int out=0,len=0;

  string head0 = ser.read(1);
  string head0_hex=string_to_hex(head0);
  if(head0_hex=="FF")
  {
      string head1 = ser.read(1);
      string head1_hex=string_to_hex(head1);
      if(head1_hex=="AA")
      {
          string dataLen=ser.read(1);
          string dataLen_hex=string_to_hex(dataLen);
          sscanf(dataLen_hex.c_str(),"%02x",&len);

          string dataRes=ser.read(1);
          string dataRes_hex=string_to_hex(dataRes);
          for(int i=0;i<len-1;i++)
          {
            string vaildData=ser.read(1);
            string vaildData_hex=string_to_hex(vaildData);
            sscanf(vaildData_hex.c_str(),"%02x",&recVaildData[i]);
          }

          string check_sum=ser.read(1);
          string check_sum_hex=string_to_hex(check_sum);
      }
  }
}

/*
*往STM32 中发送左右轮的速度，单位为周期内轮子要转动的码盘数
*/
void driverRobot(int16_t& left_pwm,int16_t& right_pwm)
{
  //cout<<"left_pwm="<<left_pwm<<",right_pwm="<<right_pwm<<endl;
  uint8_t left_g=left_pwm>>8;
  //printf("%02x\n",left_g);
  uint8_t left_d=left_pwm&0xFF;
  //printf("%02x\n",left_d);

  uint8_t right_g=right_pwm>>8;
  uint8_t right_d=right_pwm&0xFF;

  uint8_t check_sum=((uint8_t)0x05+(uint8_t)0x04+left_d+left_g+right_d+right_g)%255;

  char data[20]={(uint8_t)0xff,(uint8_t)0xaa,(uint8_t)0x05,(uint8_t)0x04,left_d,left_g,right_d,right_g,check_sum};
  ser.write((const uint8_t*)data,9);

  uint8_t responseData[20]={0};
  RecData(responseData);
}
/*
*订阅到速度主题/cmd_vel的Twist后的回调处理函数
*作用: 将ROS的速度转换成左右轮电机的速度
*/
void controlCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    cout<<"in controlCallback"<<endl;
     //从ROS速度消息中获取出中心线速度和角速度
    vel_x=msg->linear.x;
    vel_w=msg->angular.z;
    
    
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

int main(int argc, char **argv)
{
    ros::init(argc,argv,"driver_test");
    //定义ros 节点
    ros::NodeHandle nh;
	
    if(connectRobot())
    {
        cout<<"connect robot ok"<<endl;
    }
    else
    {
        cout<<"connect robot fail"<<endl;
        return -1;
    }
	
    ros::Subscriber sub=nh.subscribe("/cmd_vel",1,controlCallback);
	
    ros::spin();
    return 0;
}
