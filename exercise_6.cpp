#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/video/background_segm.hpp>
#include <iostream>
using namespace cv;
Mat video;
Mat edges;
Ptr<BackgroundSubtractor> pMOG2;
Mat fgMaskMOG2;
Mat kernel1(1,1,CV_8U,Scalar(1));
Mat kernel2(2,2,CV_8U,Scalar(1));
Mat kernel3(3,3,CV_8U,Scalar(1));
Mat kernel5(5,5,CV_8U,Scalar(1));
int main(int argc, char *argv[])
{
    VideoCapture cap("walk.avi"); // open the default camera 'M', 'J', 'P', 'G'
    VideoWriter output("outputw.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)),false);
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    //namedWindow("GMM",1);

    pMOG2 = new BackgroundSubtractorMOG2(10,16,true);
    Mat conjunct = Mat::zeros(240, 320, CV_8UC1);
    Mat old_result = Mat::zeros(240, 320, CV_8UC1);
    int i =1;
    for(;;)
    {
        cap >> video; // get a new frame from camera
        if (video.empty())
        {
          break;
        }
        pMOG2->operator()(video, fgMaskMOG2, 0.00125);
        threshold( fgMaskMOG2, fgMaskMOG2, 150, 255, 0);
        //GaussianBlur( fgMaskMOG2_t, fgMaskMOG2_t, Size(3,3), 0, 0 );
        erode(fgMaskMOG2, fgMaskMOG2, kernel1);
        dilate(fgMaskMOG2, fgMaskMOG2, kernel1);
        dilate(fgMaskMOG2, fgMaskMOG2, kernel3);
        erode(fgMaskMOG2, fgMaskMOG2, kernel3);
        dilate(fgMaskMOG2, fgMaskMOG2, kernel5);
        erode(fgMaskMOG2, fgMaskMOG2, kernel5);

        std::vector<std::vector<Point>> contours;
        std::vector<Point> contour;
        findContours(fgMaskMOG2.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        //drawContours(video,contours,-1,Scalar(0,0,255),3);
        double largest_size=0;
        Rect rect;
        int k;
        for (k = 0; k < contours.size(); k++)
        {
            double size = contourArea(contours[k], false);
            if(largest_size<size) {
              largest_size = size;
              rect = boundingRect(contours[k]);
              contour = contours[k];
            }
        }
        std::vector<Point> head;
        int j;
        for (j = 0; j < contour.size(); j++)
        {
           //if (contour[j].y<=rect.y)//&&
           if(contour[j].y<=(rect.y+rect.height*0.125))
           {
              //std::cout<<contour[j].y<<","<<(rect.y)<<","<<(rect.height)<<","<<rect.y<<std::endl;
              head.push_back(contour[j]);
           }  
        }
        Point2f center;
        float radius;
        if(head.size()>0) 
        {
          //rect = boundingRect(head);
          //rectangle( video, rect, Scalar(255,0,0));
          approxPolyDP(Mat(head),head,3,true);
          minEnclosingCircle((Mat)head,center,radius);
          //circle(video,center,radius,Scalar(255,0,0));
        }
        //center
        //rect
        //std::cout<<rect.x<<","<<rect.y<<","<<rect.width<<","<<rect.height<<std::endl;
        //std::cout<<video.rows<<","<<video.cols<<std::endl;
        Mat result=Mat::zeros(240,320,CV_8UC1);
        Mat roi(fgMaskMOG2,Rect(rect.x,rect.y,rect.width,rect.height));
        //std::cout<<(160-center.x+rect.x+roi.cols)<<std::endl;
        Mat in_result(result,Rect(160-center.x+rect.x,0,roi.cols,roi.rows));
        roi.copyTo(in_result);
        //Mat c3 = Mat::zeros(300,300,CV_8U);
        //result.convertTo(c3,CV_8U);
        //std::cout<<test.rows<<","<<test.cols<<std::endl;
        if(i>56&&i<82) 
        {
          //std::cout<<CV_8UC3<<result.type()<<std::endl;
          Mat diffresult = Mat::zeros(240,320,CV_8UC1);
          if(i>57&&i<82) {
            absdiff( old_result , result , diffresult);
            add(diffresult, conjunct, conjunct);
          }
          old_result = result;
          output<<result;
          imwrite("outputw.jpg",conjunct);
        }

        //std::cout<<rect.x<<","<<rect.y<<","<<rect.width<<","<<rect.height<<std::endl;
        //printf("%d\n", index);
        //drawContours( video, contours, index, Scalar(0,0,255),3);
        //video.copyTo(output2, fgMaskMOG2_e);
        //imshow("FG Mask MOG", fgMaskMOG2_t);
        //imshow("FG Mask with Morphology", fgMaskMOG2_d);
        i++;
        //std::cout<<"frame"<<i<<std::endl;
        
        if(waitKey(30) >= 0) break;
    }
  std::system("ffmpeg -y -i outputw.avi outputw.mp4 -v 0");
  return 0;
}
