#include <cmath>
#include <vector>
#include <iostream>
#include <string>

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>

#define CV_LOAD_IMAGE_COLOR 1

class Split2hsv
{
public:
  Split2hsv (std::string s);
  virtual ~Split2hsv ();

  cv::Mat hsv(cv::Mat image_src);
  cv::Mat getImageH();
  cv::Mat getImageS();
  cv::Mat getImageV();
  cv::Mat getLogTrans(cv::Mat image_src);
  cv::Mat getInvLogTrans(cv::Mat image_src);

private:
  cv::Mat _image;
  //std::vector<cv::Mat> _imagehsv;
  cv::Mat _imagehsv[3];
};
