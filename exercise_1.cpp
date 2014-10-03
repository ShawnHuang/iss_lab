#include <cmath>
#include <vector>
#include <iostream>

#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include "Split2hsv.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  Split2hsv hsv = Split2hsv("lena.jpg");
  cv::Mat h = hsv.getImageH();
  cv::Mat s = hsv.getImageS();
  cv::Mat v = hsv.getImageV();
  cv::Mat lh = hsv.getLogTrans(h);
  cv::Mat ls = hsv.getLogTrans(s);
  cv::Mat lv = hsv.getLogTrans(v);
  cv::Mat ilh = hsv.getInvLogTrans(h);
  cv::Mat ils = hsv.getInvLogTrans(s);
  cv::Mat ilv = hsv.getInvLogTrans(v);
  
  
  cv::namedWindow("exercise_1", cv::WINDOW_AUTOSIZE);
 
  cv::imshow("exercise_1", h);
  cv::imshow("exercise_2", s);
  cv::imshow("exercise_3", v);
  
  cv::imshow("exercise_4", lh);
  cv::imshow("exercise_5", ls);
  cv::imshow("exercise_6", lv);

  cv::imshow("exercise_7", ilh);
  cv::imshow("exercise_8", ils);
  cv::imshow("exercise_9", ilv);
  
  cv::Mat aa(ilv, cv::Rect(0,0,100,100));

  //cv::Mat tile = img(cv::Range(r, min(r + N, img.rows)), cv::Range(c, min(c + N, img.cols)));
  resize(h, h, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  resize(s, s, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  resize(v, v, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  resize(lh, lh, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  resize(ls, ls, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  resize(lv, lv, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  resize(ilh, ilh, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  resize(ils, ils, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  resize(ilv, ilv, cv::Size(), 0.33, 0.33, cv::INTER_CUBIC);
  //cv::Mat tile(cv::Range(0,h.rows),cv::Range(0,h.cols));
  //tile(cv::Range(0,h.rows),cv::Range(0,h.cols))=h;
  //cv::imshow("exercise_10", aa);
  cv::waitKey(0);
  return 0;
}
