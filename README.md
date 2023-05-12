# DIP
This is the repository for the Digital Image Processing Lab. For introduction in Chinese , please refer to the [中文说明](https://github.com/OriTwil/DIP/blob/main/%E4%B8%AD%E6%96%87%E8%AF%B4%E6%98%8E.md)
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/%E5%BE%AE%E4%BF%A1%E5%9B%BE%E7%89%87_20230416170210.jpg" width="200" height="250">
## Update Notes

- 4/12: Completed Experiment 1, updated code and results.
- 4/13: Completed Experiment 2, updated code and results.
- 4/13: Completed Experiment 3, updated code and results.
- 4/13: Experiment 4 is almost done, but parameter adjustments are needed to improve recognition accuracy. (Currently using a laptop camera for testing, ZED camera is required for parameter tuning)
- 4/14: Tested the code using ZED camera.

## Experiment Requirements

- Experiment 1: Histogram Equalization
- Experiment 2: Spatial Gaussian Filtering, Dilation, and Erosion
- Experiment 3: Edge Detection, Line Detection, and Circle Detection
- Experiment 4: Color Space Conversion, Color Segmentation, Object Color Detection, and Chassis Control
Course Project: Implementing an Automated Medicine Delivery Car
## File Description
- `code`： Reference code
- `dip_ws`： Experiment workspace
- `graph`：Experiment results
- `tutorials`：Experiment guide

## Instructions

- Use ls /dev/video* to check which video device corresponds to the camera.
- Modify the code to open ZED/Laptop camera with open(0).
- Run the code.

## Experimental Results
- Experiment 1: Histogram Equalization
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp1(3).png" width="200" height="150"> 
- Experiment 2: Input Image & Spatial Gaussian Filtering & Grayscale Image & Binary Image & Erosion & Dilation
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(in).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(filtered).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(gray).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(binary).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(eroded).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(dilated).png" width="200" height="150">

- Experiment 3: Edge Detection, Line Detection
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp3(edge).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/dip_ws/src/img/edge_detection.png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp3(edge_img).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/dip_ws/src/img/line_detecton.png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp3(hough_line).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp3(hough_circle).png" width="200" height="150">
- Experiment 4: Input, Color Space Conversion, Color Segmentation, Object Detection, Object Color Detection, and Histogram
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp4(10).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp4(8)png.png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp4(9).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/expr4(7).png" width="200" height="150"> <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp4(hist).png" width="200" height="150">
