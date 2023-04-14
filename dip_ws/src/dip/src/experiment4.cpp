#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
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

    // 创建阈值分割的滑动条
    int iLowH = 0;
    int iHighH = 10;
    int iLowS = 43;
    int iHighS = 255;
    int iLowV = 46;
    int iHighV = 255;

    namedWindow("Control", WINDOW_AUTOSIZE); // 新建一个窗口

    // 利用createTrackbar()函数建立滑动条，对颜色空间转换后的各通道进行阈值分割
    createTrackbar("LowH", "Control", &iLowH, 179); // 创建Hue滑动条
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); // 创建Saturation滑动条
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255); // 创建Value滑动条
    createTrackbar("HighV", "Control", &iHighV, 255);

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

        // 色度空间转换
        // 1.空域高斯滤波去噪
        Mat frFiltered; 
        Mat sigma = (cv::Mat_<double>(2, 2) << 10, 0, 0, 10);
        Gaussian(frIn, frFiltered, sigma);
        imshow("Filtered",frFiltered);
        // 2.遍历原图的RGB三通道像素
        Mat frHsv;
        cvtColor(frFiltered, frHsv, COLOR_BGR2HSV); // 转换为HSV色彩空间
        // 3.imshow显示结果图
        imshow("HSV", frHsv);

        // 颜色分割
        cv::Mat frThresholded;
        inRange(frHsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), frThresholded); // 对HSV图像进行阈值分割
        // imshow显示结果图
        imshow("Thresholded",frThresholded);

        // 目标颜色检测
        cv::Mat frEdge; // 轮廓图像

        // 1. 轮廓检测
        std::vector<std::vector<Point> > contours; // 定义轮廓
        std::vector<Vec4i> hierarchy;
        findContours(frThresholded, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); // 轮廓检测

        // 2. 选择矩形描述轮廓
        std::vector<Rect> boundRect; // 定义矩形向量
        for (int i = 0; i < contours.size(); i++)
        {
            Rect rect = boundingRect(contours[i]); // 根据轮廓获取外接矩形
            float aspectRatio = (float)rect.width / (float)rect.height; // 计算宽高比
            int area = rect.width * rect.height; // 计算面积
            if (aspectRatio >= 0 && aspectRatio <= 0.5 && area > 8000) { // 仅选择宽高比在1到2之间且面积大于200像素的矩形
                boundRect.push_back(rect);
                rectangle(frame, rect.tl(), rect.br(), Scalar(0, 255, 0), 2, 8, 0); // 画矩形框
            }
        }
        imshow("Result", frame);

        // 3. 获取多边形区域后。从原图中截取该区域图像
        // 获取多边形区域
        std::vector<std::vector<Point>> polygons(boundRect.size());
        for (int i = 0; i < boundRect.size(); i++)
        {
            std::vector<Point> poly;
            poly.push_back(boundRect[i].tl());
            poly.push_back(Point(boundRect[i].tl().x, boundRect[i].br().y));
            poly.push_back(boundRect[i].br());
            poly.push_back(Point(boundRect[i].br().x, boundRect[i].tl().y));
            approxPolyDP(Mat(poly), polygons[i], 3, true); // 多边形拟合
        }

        // 从原图中截取该区域图像
        Mat frROI = Mat::zeros(frHsv.size(), CV_8UC1); // 创建一个和原图大小相同的黑色图像
        for (int i = 0; i < polygons.size(); i++)
        {
            fillConvexPoly(frROI, polygons[i], Scalar(255)); // 在黑色图像上填充多边形区域
        }
        Mat frTarget; // 存储目标图像
        frIn.copyTo(frTarget, frROI); // 从原图中截取该区域图像


        // 4. 对多边形区域的图像进行颜色分割，对分割结果进行统计，判断图像的颜色并输出分类结果
        //（1）给红，橙，黄，绿，青，蓝，紫设定对应阈值，统计轮廓内对应每种颜色阈值范围内的像素个数。
        Mat frSeg; // 分割结果图像
        cvtColor(frFiltered, frHsv, COLOR_BGR2HSV); // BGR->HSV色彩空间转换

        Mat frMask; // 分割后的掩膜
        inRange(frHsv, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), frMask); // 利用滑动条的参数进行阈值分割
        imshow("Mask",frMask);

        // 统计各颜色阈值内的像素数目
        int nRed = countNonZero(frMask(cv::Rect(0, 0, frMask.cols, frMask.rows / 3))); // 统计红色像素个数
        int nOrange = countNonZero(frMask(cv::Rect(0, frMask.rows / 3, frMask.cols, frMask.rows / 3))); // 统计橙色像素个数
        int nYellow = countNonZero(frMask(cv::Rect(0, frMask.rows / 3 * 2, frMask.cols, frMask.rows / 3)));// 统计黄色像素个数
        int nGreen = countNonZero(frMask(cv::Rect(0, frMask.rows / 3, frMask.cols, frMask.rows / 3)));// 统计绿色像素个数
        int nCyan = countNonZero(frMask(cv::Rect(0, frMask.rows / 3, frMask.cols, frMask.rows / 3)));// 统计青色像素个数
        int nBlue = countNonZero(frMask(cv::Rect(0, frMask.rows / 3, frMask.cols, frMask.rows / 3 * 2)));// 统计蓝色像素个数
        int nPurple = countNonZero(frMask(cv::Rect(0, frMask.rows / 3, frMask.cols, frMask.rows / 3)));// 统计紫色像素个数

        // 根据各阈值范围内的像素数目判断颜色并输出分类结果
        if (nRed > 1000) {
            std::cout << "The color is red" << std::endl;
        } else if (nOrange > 1000) {
            std::cout << "The color is orange" << std::endl;
        } else if (nYellow > 1000) {
            std::cout << "The color is yellow" << std::endl;
        } else if (nGreen > 1000) {
            std::cout << "The color is green" << std::endl;
        } else if (nCyan > 1000) {
            std::cout << "The color is cyan" << std::endl;
        } else if (nBlue > 1000) {
            std::cout << "The color is blue" << std::endl;
        } else if (nPurple > 1000) {
            std::cout << "The color is purple" << std::endl;
        } else {
            std::cout << "No color detected" << std::endl;
        }
        // // 绘制折线图
        // Mat hsvFrame;
        // cvtColor(frFiltered, hsvFrame, COLOR_BGR2HSV); // 转换为HSV空间

        // Mat hist;
        // int histSize[] = { 180 }; // 色调值的范围是0到179
        // float hranges[] = { 0, 180 };
        // const float* ranges[] = { hranges };
        // int channels[] = { 0 }; // 色调通道
        // calcHist(&hsvFrame, 1, channels, Mat(), hist, 1, histSize, ranges, true, false); // 计算直方图

        // // 显示折线图
        // int hist_w = 360, hist_h = 240;
        // int bin_w = cvRound((double)hist_w / histSize[0]);
        // Mat histImg(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
        // normalize(hist, hist, 0, histImg.rows, NORM_MINMAX, -1, Mat()); // 归一化

        // for (int i = 1; i < histSize[0]; i++)
        // {
        //     line(histImg, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
        //         Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
        //         Scalar(255, 255, 255), 2, 8, 0);
        // }
        // imshow("Hist", histImg);

        // 统计不同颜色在直方图中的比例，红橙黄绿青蓝紫
        float ranges[] = {0, 10, 20, 30, 60, 100, 135, 180};
        const float* histRange = {ranges};

        Mat hist;
        int histSize = 7;
        int channels[] = {0}; // 只对Hue通道进行直方图统计
        calcHist(&frHsv, 1, channels, Mat(), hist, 1, &histSize, &histRange, true, false);

        double maxVal = 0;
        minMaxLoc(hist, 0, &maxVal, 0, 0);

        int histHeight = 256;
        Mat histImg = Mat::zeros(histHeight, histSize * 30, CV_8UC3);

        for (int h = 0; h < histSize; h++)
        {
            float binVal = hist.at<float>(h);
            int intensity = cvRound(binVal * histHeight / maxVal);
            Scalar color;
            if (h == 0 || h == 1 || h == 6)
            {
                color = Scalar(255, 0, 0); // 红色
            }
            else if (h == 2 || h == 3)
            {
                color = Scalar(0, 128, 255); // 橙色
            }
            else if (h == 4)
            {
                color = Scalar(0, 255, 255); // 黄色
            }
            else if (h == 5)
            {
                color = Scalar(0, 255, 0); // 绿色
            }
            else if (h == 6)
            {
                color = Scalar(255, 255, 0); // 青色
            }
            else
            {
                color = Scalar(255, 0, 255); // 紫色
            }
            rectangle(histImg, Point(h * 30, histHeight - 1), Point((h + 1) * 30 - 1, histHeight - intensity), color, FILLED);
        }
        imshow("Histogram", histImg);

        ros::spinOnce();
        waitKey(5);
    }
    return 0;
}