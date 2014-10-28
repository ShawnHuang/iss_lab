#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <cmath>
using namespace cv;
Mat video;
Mat edges;
Ptr<BackgroundSubtractor> pMOG;
Ptr<BackgroundSubtractor> pMOG2;
Mat fgMaskMOG;
Mat fgMaskMOG2;
Mat fgMaskMOG_t;
Mat fgMaskMOG_d;
Mat fgMaskMOG_e;
Mat fgMaskMOG2_d;
Mat fgMaskMOG2_e;
Mat kernel(3,3,CV_8U,Scalar(1));
Mat kernel2(5,5,CV_8U,Scalar(1));
void select_bgr_inrange(string str)
{
  cv::Mat image_rgb;
  cv::Mat image_mask;
  image_rgb = cv::imread(str, CV_LOAD_IMAGE_COLOR);
  //imshow(str,image_rgb);
  //cv::inRange(image_rgb, cv::Scalar(197, 230, 247), cv::Scalar(197,230,247), image_mask);
  cv::inRange(image_rgb, cv::Scalar(192, 224, 240), cv::Scalar(192,224,240), image_mask);
  //cv::inRange(image_rgb, cv::Scalar(179,209,245), cv::Scalar(184,216,251), image_mask);
  //imshow(str,image_mask);
  //cv::cvtColor(image_src, image_dst, CV_BGR2HSV);
}
void select_Ycrcb_inrange(Mat src, Mat& dst)
{
  cv::Mat image_rgb;
  cv::Mat image_ycrcb;
  cv::Mat image_mask;
  //image_rgb = cv::imread(str, CV_LOAD_IMAGE_COLOR);
  image_rgb = src;
  cv::cvtColor(image_rgb, image_ycrcb, CV_BGR2YCrCb);
  //cv::inRange(image_ycrcb, cv::Scalar(80, 135, 85), cv::Scalar(255,180,135), image_mask);
  cv::inRange(image_ycrcb, cv::Scalar(0, 133, 98), cv::Scalar(255,177,122), image_mask);
  //imshow("test",image_mask);
  dst = image_mask;
}
void select_bgr_loop(string str)
{
  cv::Mat image_rgb;
  image_rgb = cv::imread(str, CV_LOAD_IMAGE_COLOR);
  cv::Mat image_mask(image_rgb.rows,image_rgb.cols,CV_8UC1,Scalar(0));
  for (int i = 0; i < image_rgb.rows; i++)
  {
    cv::Vec3b* row = image_rgb.ptr<cv::Vec3b>(i);

    int j,index;
    for (j = 0,index = 0; j < image_rgb.cols; j++,index+=3)
    {
      cv::Vec3b col = row[j];
      uchar blue = col[0];
      uchar green = col[1];
      uchar red = col[2];
      //detect baby face 
      //if((int) blue ==197 &&(int)green == 230 && (int) red ==247) 
      //{
      //  image_mask.ptr<uchar>(i)[j] = (uchar) '\xff';
      //}
      if ((int) red>95 &&
          (int) green>40 && 
          (int) blue>20 && 
          (int) red>(int) green && 
          (int) red>(int) blue && 
          std::abs((int) red - (int) green)>15 &&
          std::max(std::max((int) red, (int) green),(int) blue) - std::min(std::min((int) red, (int) green),(int) blue)>15
        )
      {
        image_mask.ptr<uchar>(i)[j] = (uchar) '\xff';
      }
    }
  }
  //imshow(str,image_mask);
}
void videoDetect(string str)
{
  Mat frame;
  Mat gframe;
  Mat selcolor;
  Mat canny;
  Mat kernel(20,20,CV_8U,Scalar(1));
  Mat kernel2(30,30,CV_8U,Scalar(1));
  VideoCapture cap(str);
  //int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));
  //CV_FOURCC('M', 'J', 'P', 'G')
  VideoWriter output("output.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)));
  //VideoWriter output("output.avi", cap.get(CV_CAP_PROP_FOURCC), cap.get(CV_CAP_PROP_FPS), Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)));
  while(cap.isOpened()) {
    Mat frame_m;
    cap>>frame;
    if(frame.empty()) break;
    GaussianBlur(frame,gframe,Size(5,5),2,0);
    select_Ycrcb_inrange(gframe, selcolor);
    //erode(selcolor, selcolor, kernel);
    //dilate(selcolor, selcolor, kernel2);
    //canny
    //Canny(selcolor, selcolor, 50, 150, 3);
    std::vector<std::vector<Point>> contours;
    findContours(selcolor,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    //vector<vector<Point>>::iterator itr;
    //itr = contours.begin();
    //while(itr != contours.end())
    //{
    for(int i=0;i<contours.size();i++)
    {
      if(contours[i].size()<1000){
        drawContours(selcolor,contours,i,Scalar(0),CV_FILLED);
        continue;
      }
    }
    frame.copyTo(frame_m,selcolor);
    for(int i=0;i<contours.size();i++)
    {
      if(contours[i].size()<1000){
        continue;
      }
      drawContours(frame_m,contours,i,Scalar(0,0,255));
      Rect rect = boundingRect(Mat(contours[i]));
      putText( frame_m, "pixels: " + std::to_string((int)contourArea(contours[i])) , Point(rect.x,rect.y), FONT_HERSHEY_COMPLEX, 1, Scalar(0,0,255));
      rectangle(frame_m,rect,Scalar(255,0,0));
    }
    //  ++itr;
    //}
    //
    //vector<Point> points;
    //for (int r = 0; r < selcolor.rows; r++)
    //	for (int c = 0; c < selcolor.cols; c++)
	  //  	if (selcolor.ptr<uchar>(r)[c] > 127)
	  //  		points.push_back(Point(c,r));
    //Rect test = boundingRect(points);
    //rectangle(frame,test,Scalar(255,0,0));
    //std::cout<<canny.at<int>(2,3)<<std::endl;
    //imshow(str, selcolor);
    output<<frame_m;
  }
  std::system("ffmpeg -i output.avi output.mp4 &>/dev/null");
}
void roi()
{
  Mat img = imread("./picture.jpg", CV_LOAD_IMAGE_COLOR);
  int x = 20, y = 20, width = 50, height = 50;
  Rect roi_rect = Rect(x,y,width,height);
  Mat roi = img(roi_rect);
  Mat complement;
  bitwise_not(roi,complement);
  //imshow("image",roi);
}
int main(int argc, char *argv[])
{
    //select_bgr_inrange("./baby01.bmp");
    //select_bgr_inrange("./picture.jpg");
    //select_Ycrcb_inrange("./picture1.jpg");
    //select_bgr_loop("./picture.jpg");
    videoDetect("./input.mp4");
    //roi();
    //waitKey(0);

//    VideoCapture cap("video.avi"); // open the default camera
//    if(!cap.isOpened())  // check if we succeeded
//        return -1;
//    //namedWindow("GMM",1);
//
//    pMOG = new BackgroundSubtractorMOG(10,5,0.5);
//    pMOG2 = new BackgroundSubtractorMOG2(10,16,true);
//    int i =1;
//    for(;;)
//    {
//        Mat output;
//        Mat output2;
//        cap >> video; // get a new frame from camera
//        pMOG->operator()(video, fgMaskMOG, 0.01);
//        pMOG2->operator()(video, fgMaskMOG2, 0.01);
//        threshold( fgMaskMOG, fgMaskMOG_t, 128, 255, 0);
//        erode(fgMaskMOG_t, fgMaskMOG_e, kernel);
//        erode(fgMaskMOG2, fgMaskMOG2_e, kernel);
//        dilate(fgMaskMOG_e, fgMaskMOG_d, kernel2);
//        dilate(fgMaskMOG2_e, fgMaskMOG2_d, kernel2);
//        video.copyTo(output, fgMaskMOG_e);
//        video.copyTo(output2, fgMaskMOG2_e);
//        //imshow("GMM", video);
//        //imshow("FG Mask MOG", output2);
//        //imshow("FG Mask MOG", fgMaskMOG_t);
//        //imshow("FG Mask with Morphology", fgMaskMOG_d);
//        //imshow("FG Mask MOG output", output);
//        i++;
//        if (i==150)
//        {
//          vector<int> compression_params;
//          compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
//          compression_params.push_back(9); 
//          imwrite("150th.png",output,compression_params);
//        }
//        if (i==200)
//        {
//          vector<int> compression_params;
//          compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
//          compression_params.push_back(9); 
//          imwrite("200th.png",output,compression_params);
//        }
//        std::cout<<"frame"<<i<<std::endl;
//        if(waitKey(30) >= 0) break;
//    }
    // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
