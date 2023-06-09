#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
// #include <cv.h>
// #include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"

#include <geometry_msgs/Twist.h>
#include "sensor_msgs/Image.h"

#define LINEAR_X 0

using namespace cv;

// 空域高斯滤波器函数
void Gaussian(cv::Mat& input, cv::Mat& output, cv::Mat sigma)
{
    // 检查sigma是否合法
    if (sigma.cols != 2 || sigma.rows != 2 || sigma.type() != CV_64F) {
        std::cout << "Invalid sigma parameter" << std::endl;
        return;
    }
    double sigmaX = sigma.at<double>(0);
    double sigmaY = sigma.at<double>(1);
    int ksizeX = cvRound(3 * sigmaX) * 2 + 1; // 计算高斯核大小
    int ksizeY = cvRound(3 * sigmaY) * 2 + 1;
    ksizeX = std::max(1, ksizeX); // 最小为1
    ksizeY = std::max(1, ksizeY);
    if (ksizeX % 2 == 0) ksizeX++; // 确保为奇数
    if (ksizeY % 2 == 0) ksizeY++;
    // 调用GaussianBlur函数
    cv::GaussianBlur(input, output, cv::Size(ksizeX, ksizeY), sigmaX, sigmaY);
}


// 腐蚀函数
void Dilate(cv::Mat& src, cv::Mat& tem, cv::Mat& dst)
{
    Mat element = getStructuringElement(MORPH_RECT, tem.size(), Point(tem.cols/2, tem.rows/2));
    dilate(src, dst, element);
}

// 膨胀函数
void Erode(cv::Mat& src, cv::Mat& tem, cv::Mat& dst)
{
    Mat element = getStructuringElement(MORPH_RECT, tem.size(), Point(tem.cols/2, tem.rows/2));
    erode(src, dst, element);
}

int main(int argc,char **argv)
{
    VideoCapture capture;
    capture.open(0);

	ROS_WARN("------START------");
	ros::init(argc, argv, "trafficLaneTrack"); //初始化ROS节点
	ros::NodeHandle n;

    ros::Rate loop_rate(10); // 速度发布频率
	// ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/smoother_cmd_vel", 5); //定义机器人的速度发布器

	if (!capture.isOpened())
	{
		printf("摄像头没有正常打开\n");
		return 0;
	}
    waitKey(1000);
    Mat frame;// 当前帧图片
    int nFrames = 0;// 图片帧数
    int frameWidth = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    int frameHeight = capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	while (ros::ok())
	{
		capture.read(frame);
		if (frame.empty())
		{
			break;
		}

        // Mat frIn = frame.clone();// 笔记本
        Mat frIn = frame(cv::Rect(0,0,frame.cols/2,frame.rows)); // ZED左目图片
        imshow("In",frIn);

        Mat frFiltered, frDilated, frEroded;
        // 空域高斯滤波
        Mat sigma = (cv::Mat_<double>(2, 2) << 10, 0, 0, 10);
        Gaussian(frIn, frFiltered, sigma);
        imshow("Filtered",frFiltered);

        // 将frIn转换为灰度图
        Mat frGray;
        cvtColor(frIn, frGray, COLOR_BGR2GRAY);
        imshow("Gray", frGray);

        // 将灰度图转换成二值图像
        Mat frBinary;
        threshold(frGray, frBinary, 100, 255, THRESH_BINARY);
        imshow("Binary", frBinary);

        // 对二值图像进行腐蚀
        Mat tem = Mat::zeros(13,13,CV_8UC1);
        Dilate(frBinary, tem, frDilated);
        imshow("Dilated",frDilated);

        // 对二值图像进行膨胀
        Erode(frBinary, tem, frEroded);
        imshow("Eroded",frEroded);

        ros::spinOnce();
        waitKey(5);
    }
    return 0;
}

