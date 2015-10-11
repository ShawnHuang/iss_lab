#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <cmath>
using namespace cv;
int main(int argc, char *argv[])
{
  VideoCapture cap(0); // open the default camera
  Mat video;
  if(cap.isOpened())
  {
  	//std::cout << cap.get(CV_CAP_PROP_FPS) << std::endl;
  }
  
  VideoWriter output("test.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, Size(640, 480));

  while(cap.isOpened())
  {
    cap >> video;
    output << video;
    imshow("video", video);
    if (cvWaitKey(20) == 27)
    {
      break;
    }
  }
  return 0;
}
