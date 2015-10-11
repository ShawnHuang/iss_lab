#include <iostream>
#include <vector>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/objdetect/objdetect.hpp>
 
using namespace std;
using namespace cv;
    
Mat image1 = imread("SRC1.jpg");
Mat image2 = imread("SRC2.jpg");
Mat image3 = imread("SRC3.jpg");
Mat image4 = imread("SRC4.jpg");
VideoCapture cap("SRC.mp4"); 


CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";

Mat detect( Mat frame );

int main(int argc, const char * argv[]){

    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

    if(!cap.isOpened()) return -1;
    VideoWriter output("SRC.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)),true);
    Mat video;
    
    const clock_t begin = clock();
    
    imwrite("SRC1f.jpg",detect(image1));
    imwrite("SRC2f.jpg",detect(image2));
    imwrite("SRC3f.jpg",detect(image3));
    imwrite("SRC4f.jpg",detect(image4));

    for (;;)
    {
      cap >> video; // get a new frame from camera
      if (video.empty())
      {
        break;
      }
      output<<detect(video);
      if(waitKey(30) >= 0) break;
    }
    
    const clock_t mid = clock();
    cout << float(mid-begin)/CLOCKS_PER_SEC << endl;
    std::system("ffmpeg -y -i SRC.avi SRC2.mp4 -v 0");
    
    return 0;
   }

Mat detect( Mat frame )
{
  Mat frame_gray;
  std::vector<Rect> faces;

  cvtColor( frame, frame_gray, CV_BGR2GRAY);
  equalizeHist( frame_gray, frame_gray);//equalize

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, Size(40, 40) );//CV_HAAR_DO_CANNY_PRUNING
  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;

    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, Size(15, 15) );//0 |CV_HAAR_SCALE_IMAGE

    for( size_t j = 0; j < eyes.size(); j++ )
     {
       Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
     }
  }
  //-- Show what you got
  //imshow( window_name, frame );
  return frame;
}
