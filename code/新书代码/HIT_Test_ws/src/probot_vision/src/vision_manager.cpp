/***********************************************************************
Copyright 2019 Wuhan PS-Micro Technology Co., Itd.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***********************************************************************/

#include "probot_vision/vision_manager.h"
#include "geometry_msgs/PointStamped.h"
#include "tf/transform_listener.h"

static const std::string OPENCV_WINDOW = "Image window";
tf::TransformListener* listener=NULL;


VisionManager::VisionManager(ros::NodeHandle n_, float length, float breadth) : it_(n_)
{
	this->table_length = length;
	this->table_breadth = breadth;

  	// Subscribe to input video feed and publish object location
  	//image_sub_  = it_.subscribe("/probot_anno/camera/image_raw", 1, &VisionManager::imageCb, this);
  	
  	image_sub_  = it_.subscribe("/usb_cam/image_raw", 1, &VisionManager::imageCb, this);
	image1_pub_ = it_.advertise("/table_detect", 1);
	image2_pub_ = it_.advertise("/object_detect", 1);
	
	cv::namedWindow(OPENCV_WINDOW);
}

void VisionManager::get2DLocation(const sensor_msgs::ImageConstPtr &msg, float &x, float &y)
{
	cv::Rect tablePos;
	detectTable(msg, tablePos);

	detect2DObject(msg, x, y, tablePos);
	convertToMM(x, y);
}

void VisionManager::detectTable(const sensor_msgs::ImageConstPtr &msg, cv::Rect &tablePos)
{
	// Extract Table from the image and assign values to pixel_per_mm fields
	cv::Mat BGR[3];
    
	try
    {
      cv_ptr_ = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception &e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
	cv::Mat &image = cv_ptr_->image;

	split(image, BGR);
	cv::Mat gray_image_red = BGR[2];
	cv::Mat gray_image_green = BGR[1];
	cv::Mat denoiseImage;
	cv::medianBlur(gray_image_red, denoiseImage, 3);
    //cv::imshow("src",image);
    //cv::imshow("gray_image_red",gray_image_red);
    //cv::imshow("gray_image_green",gray_image_green);
    cv::imshow("denoiseImage",denoiseImage);
	// Threshold the Image
	cv::Mat binaryImage = denoiseImage;
	for (int i = 0; i < binaryImage.rows; i++)
	{
		for (int j = 0; j < binaryImage.cols; j++)
		{
			int editValue = binaryImage.at<uchar>(i, j);
			int editValue2 = gray_image_green.at<uchar>(i, j);

			if ((editValue >= 175) && (editValue < 255) && (editValue2 >= 175) && (editValue2 < 255))
			{ // check whether value is within range.
				binaryImage.at<uchar>(i, j) = 255;
			}
			else
			{
				binaryImage.at<uchar>(i, j) = 0;
			}
		}
	}
	cv::imshow("binaryImage_table000",binaryImage);
	cv::Mat ele = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
	cv::morphologyEx(binaryImage,binaryImage,cv::MORPH_OPEN,ele);
	cv::imshow("binaryImage_table111",binaryImage);
//	dilate(binaryImage, binaryImage, cv::Mat());
	erode(binaryImage, binaryImage, cv::Mat());
    cv::imshow("binaryImage_table",binaryImage);
	// Get the centroid of the of the blob
	std::vector<cv::Point> nonZeroPoints;
	cv::findNonZero(binaryImage, nonZeroPoints);
	cv::Rect bbox = cv::boundingRect(nonZeroPoints);
	
	std::vector<std::vector<cv::Point> > contours_box;
    std::vector<cv::Vec4i> hierarchy_box;

    // 查找图像的轮廓 查找的轮廓为白色区域的轮廓
    findContours(binaryImage, contours_box, hierarchy_box, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // 定义外接矩形
    std::vector<cv::Rect> boundRect(contours_box.size());
    
    float max_size = -1;
    int idx = 0;

    for (int i =0;i<contours_box.size();i++)
    {
        // 获取最小外接矩形
      //  boundRect[i] = boundingRect(contours[i]);

        // 在原图像上绘制最小外接矩形
       // rectangle(src, boundRect[i], Scalar(0, 255, 0));
       if(max_size < cv::contourArea(contours_box[i])){
        idx = i;
        max_size = cv::contourArea(contours_box[i]);
       }
    }
   // rectangle(src, boundRect[idx], Scalar(0, 255, 0));
	 bbox = cv::boundingRect(contours_box[idx]);
	
	cv::Point pt;
	pt.x = bbox.x + bbox.width / 2;
	pt.y = bbox.y + bbox.height / 2;
	//红点是桌面矩形中心
	cv::circle(image, pt, 4, cv::Scalar(0, 0, 255), -1, 8);
#if 0
	// Update pixels_per_mm fields
	pixels_permm_y = bbox.height / table_length;
	pixels_permm_x = bbox.width  / table_breadth;
#endif	
	// Update pixels_per_mm fields
	pixels_permm_y = 0.32417/1000.0;
	pixels_permm_x = 0.32417/1000.0;
    tablePos = bbox;

	// Test the conversion values
	std::cout << "Pixels in y" << pixels_permm_y << std::endl;
	std::cout << "Pixels in x" << pixels_permm_x << std::endl;

	// Draw Contours - For Debugging
	
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(binaryImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	for (int i = 0; i < contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(255, 0, 0);
		cv::drawContours(image, contours, i, color, 3, 8, hierarchy, 0, cv::Point());
	}
	
	
       // Update GUI Window
    cv::imshow("table", cv_ptr_->image);
    cv::waitKey(3);
	// Output modified video stream
 	image1_pub_.publish(cv_ptr_->toImageMsg());
}

void VisionManager::detect2DObject(const sensor_msgs::ImageConstPtr &msg, float &pixel_x, float &pixel_y, cv::Rect &tablePos)
{
	// Implement Color Thresholding and contour findings to get the location of object to be grasped in 2D
	cv::Mat gray_image_green;
	cv::Mat BGR[3];
    
	try
    {
      cv_ptr_ = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception &e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

	cv::Mat &image = cv_ptr_->image;

	cv::split(image, BGR);

	gray_image_green = BGR[1];

	// Denoise the Image
	cv::Mat denoiseImage;
	cv::medianBlur(gray_image_green, denoiseImage, 3);

	// Threshold the Image
	cv::Mat binaryImage = denoiseImage;
	cv::imshow("denoiseImage000",denoiseImage);
	for (int i = 0; i < binaryImage.rows; i++)
	{
		for (int j = 0; j < binaryImage.cols; j++)
		{
			if((j<tablePos.x+3) || j>(tablePos.x+tablePos.width-3) || (i<tablePos.y+3) || i>(tablePos.y + tablePos.height-3))
			{
				binaryImage.at<uchar>(i, j) = 0;
			}
			else
			{
				int editValue = binaryImage.at<uchar>(i, j);

				if ((editValue > 120) && (editValue <= 255))
				{ // check whether value is within range.
					binaryImage.at<uchar>(i, j) = 255;
//					binaryImage.at<uchar>(i, j) = 100;
				}
				else
				{
					binaryImage.at<uchar>(i, j) = 0;
				}
			}
		}
	}
	cv::imshow("binaryImage000",binaryImage);
	//cv::waitKey(0);
	dilate(binaryImage, binaryImage, cv::Mat());

	// Get the centroid of the of the blob
	std::vector<cv::Point> nonZeroPoints;
	cv::findNonZero(binaryImage, nonZeroPoints);
	cv::Rect bbox = cv::boundingRect(nonZeroPoints);
	cv::Point pt;
	pixel_x = bbox.x + bbox.width / 2;
	pixel_y = bbox.y + bbox.height / 2;

	// Test the conversion values
	std::cout << "pixel_x" << pixel_x << std::endl;
	std::cout << "pixel_y" << pixel_y << std::endl;

	// For Drawing
	pt.x = bbox.x + bbox.width / 2;
	pt.y = bbox.y + bbox.height / 2;
	cv::circle(image, pt, 4, cv::Scalar(0, 0, 255), -1, 8);

    cv::circle(image, cv::Point(320,240), 4, cv::Scalar(0, 255, 255), -1, 8);
	// Draw Contours
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(binaryImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	int center = 0;
	float max_size = -FLT_MAX;
	int center_sec = -1;
	std::cout  << "contours size = " << contours.size() << std::endl;
	for (int i = 0; i < contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(255, 0, 0);
		cv::drawContours(image, contours, i, color, 3, 8, hierarchy, 0, cv::Point());
		if(hierarchy[i][0,3] != -1){
		    center = i;
		}
		
		/*
       if(max_size < cv::contourArea(contours[i])){
        center_sec = center;
         center = i;
         max_size = cv::contourArea(contours[i]);
       }*/
	}
	cv::drawContours(image, contours, center, cv::Scalar(255, 255, 0), 3, 8, hierarchy, 0, cv::Point());
	printf("contoursize = %d\n",(int)contours.size());
    cv::imshow("image_obj",image);
    
    if(center_sec == -1)
        center_sec = center;
    bbox = cv::boundingRect(contours[center]);
    pixel_x = bbox.x + bbox.width / 2;
	pixel_y = bbox.y + bbox.height / 2;
	
	
	cv::circle(image, cv::Point(pixel_x,pixel_y), 4, cv::Scalar(255, 255, 255), -1, 8);
	
	
	std::cout << "pixel_x_test=" << pixel_x << std::endl;
	std::cout << "pixel_y_test=" << pixel_y << std::endl;
    
	// Output modified video stream
 	image2_pub_.publish(cv_ptr_->toImageMsg());
}

void VisionManager::convertToMM(float &x, float &y)
{
	img_centre_x_ = 640 / 2;
	img_centre_y_ = 480 / 2;
    
    
    //std::cout << "===================x=" << x << std::endl;
	//std::cout << "============================y=" << y << std::endl;
	
	//std::cout << "(x - img_centre_x_)=" << (x - img_centre_x_) << std::endl;
	//std::cout << "(y - img_centre_y_)" << (y - img_centre_y_) << std::endl;
	
	// Convert from pixel to world co-ordinates in the camera frame
	pixels_permm_x=0.32417/1000.0;
	pixels_permm_y=0.32417/1000.0;
	x = (x - img_centre_x_) * pixels_permm_x;
	y = (y - img_centre_y_) * pixels_permm_y;	
	
    //std::cout << "------------------x=" << x << std::endl;
	//std::cout << "--------------------------y=" << y << std::endl;
	
	
}


void VisionManager::imageCb(const sensor_msgs::ImageConstPtr &msg)
{
    ROS_INFO_STREAM("Processing the Image to locate the Object...");

    // ROS_INFO("Image Message Received");
    float obj_x, obj_y;
    get2DLocation(msg, obj_x, obj_y);

    // Temporary Debugging
    std::cout<< " X-Co-ordinate in Camera Frame :" << obj_x << std::endl;
    std::cout<< " Y-Co-ordinate in Camera Frame :" << obj_y << std::endl;
    
    geometry_msgs::PointStamped cameral_point, target_to_dobot_point;
    cameral_point.header.frame_id="cameral_base";
    cameral_point.point.x=obj_x;
    cameral_point.point.y=obj_y;
    listener->transformPoint("dobot_base",cameral_point,target_to_dobot_point);
    std::cout<<"origin point= "<<"("<<obj_x<<","<<obj_y<<")"<<std::endl;
    std::cout<<"after transform to dobot_base,target_point="<<"("<<target_to_dobot_point.point.x
    <<","<<target_to_dobot_point.point.y<<")"<<std::endl;
    
}

VisionManager::~VisionManager()
{
    cv::destroyWindow(OPENCV_WINDOW);
}


// Temporary Main Function for testing- This should go away later
int main(int argc, char** argv ) 
{
  	ros::init(argc, argv, "simple_grasping_vision_detection");
  	ros::NodeHandle n_;

  	ROS_INFO_STREAM("Waiting for two seconds..");
  	ros::WallDuration(2.0).sleep();

	//float length = 0.3;
	//float breadth = 0.3;
	
	listener=new (tf::TransformListener);
	listener->waitForTransform("dobot_base","cameral_base",ros::Time(0),ros::Duration(10.0));
	
	float length = 0.37;
	float breadth = 0.22;

	VisionManager vm(n_, length, breadth);

	while (ros::ok())
	{
		// Process image callback
		ros::spinOnce();

		ros::WallDuration(2.0).sleep();
	}
	return 0;
}
