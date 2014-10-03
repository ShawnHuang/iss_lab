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
  _image = this->hsv(image_rgb);
  cv::split(_image, _imagehsv);
}

Split2hsv::~Split2hsv(){
  
}

cv::Mat Split2hsv::hsv(cv::Mat image_src){
  cv::Mat image_dst;
  //image_dst = image_src;
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
  cv::Mat img2;
  temp.convertTo(img2,CV_32FC1);
  for(int i=0;i<temp.rows;i++){
    for(int j=0;j<temp.cols;j++)  {
      img2.at<float>(i, j) = 32*log(img2.at<float>(i, j)+1)/log(2);
    }
  }
  img2.convertTo(temp,CV_8UC1);
  return temp;
}
cv::Mat Split2hsv::getInvLogTrans(cv::Mat image_src){
  cv::Mat temp = image_src.clone();
  cv::Mat img3;
  temp.convertTo(img3,CV_32FC1);
  for(int i=0;i<temp.rows;i++){
    for(int j=0;j<temp.cols;j++)  {
	  float tempf = img3.at<float>(i, j)/32;
      img3.at<float>(i, j) = (float) pow(2,tempf)-1;
    }
  }
  img3.convertTo(temp,CV_8UC1);
  return temp;
}
