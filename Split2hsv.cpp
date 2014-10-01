#include <cmath>
#include <vector>
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include "Split2hsv.hpp"

Split2hsv::Split2hsv(std::string s){
  cv::Mat image_rgb;
  image_rgb = cv::imread(s, CV_LOAD_IMAGE_COLOR);
  _image = image_rgb;
  _image = this->hsv(_image);
  cv::split(_image, _imagehsv);
}

Split2hsv::~Split2hsv(){
  
}

cv::Mat Split2hsv::hsv(cv::Mat image_src){
  cv::Mat image_dst; 
  cv::cvtColor(image_src, image_dst, CV_BGR2HSV);
  return image_dst;
}

cv::Mat Split2hsv::getImageH(){
  return _imagehsv[0];
}

cv::Mat Split2hsv::getImageS(){
  return _imagehsv[1];
}

cv::Mat Split2hsv::getImageV(){
  return _imagehsv[2];
}

cv::Mat Split2hsv::getLogTrans(cv::Mat image_src){
  cv::Mat temp = image_src.clone();
  for(int i=0;i<temp.rows;i++){
    for(int j=0;j<temp.cols;j++)  {
      temp.at<uchar>(i, j) = 32*log10((float)temp.at<uchar>(i, j)+1);
    }
  }
  return temp;
}
cv::Mat Split2hsv::getInvLogTrans(cv::Mat image_src){
  cv::Mat temp = image_src.clone();
  for(int i=0;i<temp.rows;i++){
    for(int j=0;j<temp.cols;j++)  {
      float tempf = (float)temp.at<uchar>(i, j)/32;
      temp.at<uchar>(i, j) = pow(2,tempf)-1;
    }
  }
  return temp;
}
