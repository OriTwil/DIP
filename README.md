# DIP
Digital Image Processing Lab
[中文说明](https://github.com/OriTwil/DIP/blob/main/%E4%B8%AD%E6%96%87%E8%AF%B4%E6%98%8E.md)
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
- code： Reference code
- dip_ws： Experiment workspace
- graph：Experiment results
- tutorials：Experiment guide

## Instructions

- Use ls /dev/video* to check which video device corresponds to the camera.
- Modify the code to open ZED/Laptop camera with open(0).
- Run the code.

## Experimental Results
- Experiment 1: Histogram Equalization
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp1(3).png" width="400" height="300">
- Experiment 2: Input, Spatial Gaussian Filtering, Grayscale Image, Binary 
- Input Image
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(in).png" width="400" height="300">
- Spatial Gaussian Filtering
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(filtered).png" width="400" height="300">
- Grayscale Image
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(gray).png" width="400" height="300">
- Binary Image
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(binary).png" width="400" height="300">
- Erosion
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(eroded).png" width="400" height="300">
- Dilation
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp2(dilated).png" width="400" height="300">

- Experiment 3: Edge Detection, Line Detection
- 边缘检测
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp3(edge).png" width="400" height="300">

- Experiment 4: Input, Color Space Conversion, Color Segmentation, Object Detection, Object Color Detection, and Histogram
- Input
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp4(10).png" width="400" height="300">
- Color Space Conversion
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp4(8)png.png" width="400" height="300">
- Color Segmentation
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp4(9).png" width="400" height="300">
- Object Detection
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/expr4(7).png" width="400" height="300">
- Hist
- <img src="https://github.com/OriTwil/DIP/blob/main/graph/exp4(hist).png" width="400" height="300">
