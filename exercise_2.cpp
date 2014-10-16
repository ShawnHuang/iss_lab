#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
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
int main(int argc, char *argv[])
{
    VideoCapture cap("video.avi"); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    namedWindow("GMM",1);

    pMOG = new BackgroundSubtractorMOG(10,5,0.5);
    pMOG2 = new BackgroundSubtractorMOG2(10,16,true);
    for(;;)
    {
        Mat output;
        Mat output2;
        cap >> video; // get a new frame from camera
        pMOG->operator()(video, fgMaskMOG, 0.01);
        pMOG2->operator()(video, fgMaskMOG2, 0.01);
        threshold( fgMaskMOG, fgMaskMOG_t, 128, 255, 0);
        erode(fgMaskMOG_t, fgMaskMOG_e, kernel);
        erode(fgMaskMOG2, fgMaskMOG2_e, kernel);
        dilate(fgMaskMOG_e, fgMaskMOG_d, kernel2);
        dilate(fgMaskMOG2_e, fgMaskMOG2_d, kernel2);
        video.copyTo(output, fgMaskMOG_e);
        video.copyTo(output2, fgMaskMOG2_e);
        imshow("GMM", video);
        //imshow("FG Mask MOG", output2);
        imshow("FG Mask MOG", fgMaskMOG_t);
        imshow("FG Mask with Morphology", fgMaskMOG_d);
        imshow("FG Mask MOG output", output);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
