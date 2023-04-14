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

// 边缘检测函数
void EdgeDetector(cv::Mat& input,cv::Mat& output)
{
    cvtColor(input, output, COLOR_BGR2GRAY);
    GaussianBlur(output, output, Size(3,3), 0, 0);
    Canny(output, output, 50, 200, 3);
}

// 霍夫线变换
void Hough_line(cv::Mat& output, cv::Mat& line_img)
{
    std::vector<cv::Vec2f> lines;
    HoughLines(output, lines, 1, CV_PI/180, 100, 0, 0);

    cv::Point pt1, pt2;
    float theta_sum = 0;
    int theta_count = 0;
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        theta_sum += theta;
        theta_count += 1;
    }
    if (theta_count > 0)
    {
        float theta_mean = theta_sum / theta_count;
        float rho_mean = output.rows * sin(theta_mean) + output.cols * cos(theta_mean);
        double a = cos(theta_mean), b = sin(theta_mean);
        double x0 = a*rho_mean, y0 = b*rho_mean;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( line_img, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }
}

// 霍夫圆变换
void Hough_Circle(cv::Mat& output, cv::Mat& circle_img)
{
    std::vector<cv::Vec3f> circles;
    HoughCircles(output, circles, HOUGH_GRADIENT, 1, output.rows/8, 200, 100, 0, 0);
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle( circle_img, center, radius, Scalar(0,0,255), 3, LINE_AA);
    }
}

int main(int argc,char **argv)
{
    VideoCapture capture;
    capture.open(0);// 0为笔记本摄像头 1为zed相机

	ROS_INFO("------START------");
	ros::init(argc, argv, "trafficLaneTrack"); //初始化ROS节点
	ros::NodeHandle n;

    ros::Rate loop_rate(10); // 速度发布频率
	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/smoother_cmd_vel", 5); //定义机器人的速度发布器

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

        // 灰度图转换
        cv::Mat gray_img, edge_img;
        cvtColor(frIn, gray_img, COLOR_BGR2GRAY);

        // 边缘检测函数
        EdgeDetector(frIn, edge_img);
        imshow("edge",edge_img);

        // 线检测
        cv::Mat line_img = frIn.clone();
        Hough_line(edge_img, line_img);
        imshow("line",line_img);

        // 圆检测
        cv::Mat circle_img = frIn.clone();
        Hough_Circle(edge_img, circle_img);
        imshow("circle",circle_img);
        // // 线检测
        // cv::Mat line_img = edge_img.clone();
        // std::vector<cv::Vec2f> lines;
        // HoughLines(edge_img, lines, 5, CV_PI/180, 150, 0, 0);

        // for( size_t i = 0; i < lines.size(); i++ )
        // {
        //     float rho = lines[i][0], theta = lines[i][1];
        //     cv::Point pt1, pt2;
        //     double a = cos(theta), b = sin(theta);
        //     double x0 = a*rho, y0 = b*rho;
        //     pt1.x = cvRound(x0 + 1000*(-b));
        //     pt1.y = cvRound(y0 + 1000*(a));
        //     pt2.x = cvRound(x0 - 1000*(-b));
        //     pt2.y = cvRound(y0 - 1000*(a));
        //     line( line_img, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
        // }
        // imshow("Line detection", line_img);

        // // 圆检测
        // cv::Mat circle_img = edge_img.clone();
        // std::vector<cv::Vec3f> circles;
        // HoughCircles(edge_img, circles, HOUGH_GRADIENT, 1, edge_img.rows/8, 200, 100, 0, 0);
        // // imshow("circles",circles);
        // for( size_t i = 0; i < circles.size(); i++ )
        // {
        //     cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        //     int radius = cvRound(circles[i][2]);
        //     circle( circle_img, center, radius, Scalar(0,0,255), 3, LINE_AA);
        // }
        // imshow("Circle detection", circle_img);

        ros::spinOnce();
        waitKey(5);
    }
    return 0;
}