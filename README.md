# DIP
数字图像处理实验
#### 更新说明
- 4.12 完成实验1 更新代码与实验结果
- 4.13 完成实验2 更新代码与实验结果
- 4.13 完成实验3 更新代码与实验结果
- 4.13 基本完成实验4 还需要调整参数，提高识别效果（目前代码使用笔记本摄像头测试，调参需要使用ZED摄像头）
- 4.14 使用ZED摄像头测试代码

#### 实验要求
- 实验一：直方图均衡化
- 实验二：空域高斯滤波、膨胀、腐蚀
- 实验三：边缘检测、线检测、圆检测
- 实验四：色度空间转换、颜色分割、目标颜色检测、底盘控制
- 课程设计：实现自动取药送药小车

#### 文件说明
- code 参考代码
- dip_ws 实验工作空间
- graph 实验结果
- tutorials 实验指导书

#### 使用说明
- `ls /dev/video*`查看摄像头对应video
- 修改代码中的open(0)打开ZED/笔记本摄像头
- 运行

#### 实验效果图
- 实验一：直方图均衡化

![直方图均衡化](https://github.com/OriTwil/DIP/blob/main/graph/exp1(3).png)

- 实验二：输入、空域高斯滤波、灰度图、二值图、膨胀、腐蚀

- 输入图像![输入图像](https://github.com/OriTwil/DIP/blob/main/graph/exp2(in).png)
- 空域高斯滤波![空域高斯滤波](https://github.com/OriTwil/DIP/blob/main/graph/exp2(filtered).png)
- 灰度图![灰度图](https://github.com/OriTwil/DIP/blob/main/graph/exp2(gray).png)
- 二值图![二值图](https://github.com/OriTwil/DIP/blob/main/graph/exp2(binary).png)
- 腐蚀![腐蚀](https://github.com/OriTwil/DIP/blob/main/graph/exp2(eroded).png)
- 膨胀![膨胀](https://github.com/OriTwil/DIP/blob/main/graph/exp2(dilated).png)

- 实验三：边缘检测、线检测
- 边缘检测![边缘检测](https://github.com/OriTwil/DIP/blob/main/graph/exp3(edge).png)

- 实验四：输入、色度空间转换、颜色分割、目标检测、目标颜色检测、直方图
- 输入![输入](https://github.com/OriTwil/DIP/blob/main/graph/exp4(10).png)
- 色度空间转换![色度空间转换](https://github.com/OriTwil/DIP/blob/main/graph/exp4(8)png.png)
- 颜色分割![颜色分割](https://github.com/OriTwil/DIP/blob/main/graph/exp4(9).png)
- 目标检测![目标检测、目标颜色检测](https://github.com/OriTwil/DIP/blob/main/graph/expr4(7).png)
- 直方图![直方图](https://github.com/OriTwil/DIP/blob/main/graph/exp4(hist).png)
