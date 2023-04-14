#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;

static const std::string OPENCV_WINDOW = "Image window";

class ImageCorrect
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  
  

public:
    Mat cameraMatrix;
    Mat distCoeffs;

  ImageCorrect()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1, &ImageCorrect::imageCb, this);
    image_pub_ = it_.advertise("/usb_cam/image_correct", 1);
#if 0
    std::vector<double>cm;
    std::vector<double>dc;
    if (nh_.getParam("/usb_cam/camera_matrix/data", cm))
    {
      ROS_WARN("Got param: cameraMatrix.data=[%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf]",\
               cm[0],cm[1],cm[2],cm[3],cm[4],cm[5],cm[6],cm[7],cm[8]);
    }
    else
    {
      ROS_ERROR("Failed to get param '/usb_cam/camera_matrix/data'");
    }
    
    if (nh_.getParam("/usb_cam/distortion_coefficients/data", dc))
    {
      ROS_WARN("Got param: distortion_coefficients.data=[%lf,%lf,%lf,%lf,%lf]",\
               dc[0],dc[1],dc[2],dc[3],dc[4]);
    }
    else
    {
      ROS_ERROR("/usb_cam/distortion_coefficients/data'");
    }
    
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    cameraMatrix.at<double>(0, 0) = cm[0];
    cameraMatrix.at<double>(0, 1) = cm[1];
    cameraMatrix.at<double>(0, 2) = cm[2];
    cameraMatrix.at<double>(1, 0) = cm[3];
    cameraMatrix.at<double>(1, 1) = cm[4];
    cameraMatrix.at<double>(1, 2) = cm[5];
    cameraMatrix.at<double>(2, 0) = cm[6];
    cameraMatrix.at<double>(2, 1) = cm[7];
    cameraMatrix.at<double>(2, 2) = cm[8];
 
    distCoeffs = Mat::zeros(5, 1, CV_64F);
    distCoeffs.at<double>(0, 0) = dc[0];
    distCoeffs.at<double>(1, 0) = dc[1];
    distCoeffs.at<double>(2, 0) = dc[2];
    distCoeffs.at<double>(3, 0) = dc[3];
    distCoeffs.at<double>(4, 0) = dc[4]; 
#endif
#if 1
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    cameraMatrix.at<double>(0, 0) = 371.450355;
    cameraMatrix.at<double>(0, 1) = 0.000000;
    cameraMatrix.at<double>(0, 2) = 320.451676;
    cameraMatrix.at<double>(1, 0) = 0.000000;
    cameraMatrix.at<double>(1, 1) = 494.574368;
    cameraMatrix.at<double>(1, 2) = 234.028774;
    cameraMatrix.at<double>(2, 0) = 0.000000;
    cameraMatrix.at<double>(2, 1) = 0.000000;
    cameraMatrix.at<double>(2, 2) = 1.000000;
 
    distCoeffs = Mat::zeros(5, 1, CV_64F);
    distCoeffs.at<double>(0, 0) = -0.347383;
    distCoeffs.at<double>(1, 0) = 0.081498;
    distCoeffs.at<double>(2, 0) = 0.004733;
    distCoeffs.at<double>(3, 0) = -0.001698;
    distCoeffs.at<double>(4, 0) = 0;    
#endif

    
    //cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageCorrect()
  {
    //cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    Mat  map1, map2,frameCalibration;
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
        getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, Size(640,480), 1, Size(640,480), 0),
        Size(640,480), CV_16SC2, map1, map2);
        
     //remap(cv_ptr->image, frameCalibration, map1, map2, INTER_LINEAR);
    remap(cv_ptr->image, cv_ptr->image, map1, map2, INTER_LINEAR);
    
    // Draw an example circle on the video stream
    //if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
    //  cv::circle(cv_ptr->image, cv::Point(50, 50), 30, CV_RGB(255,0,0), -1);
    
    //if (frameCalibration.rows > 60 && frameCalibration.cols > 60)
      //cv::circle(frameCalibration, cv::Point(50, 50), 30, CV_RGB(255,0,0), -1);
      
    // Update GUI Window
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    //cv::imshow(OPENCV_WINDOW, frameCalibration);
    //cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_correct");
  ImageCorrect ic;
  ros::spin();
  return 0;
}
