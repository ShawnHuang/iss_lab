#include <opencv2/core/core.hpp>
//#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp> 
//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>

using namespace cv;

//void function_name();
int main(int argc, char *argv[])
{
  Mat img_scene = imread("scene.jpg");
  Mat img_obj = imread("obj.jpg");
  Mat gray_img_scene = imread("scene.jpg", CV_LOAD_IMAGE_GRAYSCALE); //if flag >0 three channels, flag=0 one channel, flag<0 original
  Mat gray_img_obj = imread("obj.jpg", CV_LOAD_IMAGE_GRAYSCALE); //if flag >0 three channels, flag=0 one channel, flag<0 original
  Mat desc_scene;
  Mat desc_obj;
  std::vector<KeyPoint> keypoints_scene;
  std::vector<KeyPoint> keypoints_obj;

  SIFT sift(4000, 3, 0.04, 10, 1.6f); // nfeatures=0 nOctaveLayers=3 contrastThreshold=0.04 edgeThreshold=10 sigma=1.6
  sift.operator()(gray_img_scene, noArray(), keypoints_scene, desc_scene, 0);
  sift.operator()(gray_img_obj, noArray(), keypoints_obj, desc_obj, 0);

  //SURF surf(400);//double hessianThreshold, int nOctaves=4, int nOctaveLayers=2, bool extended=true, bool upright=false );
  //surf(gray_img_obj, noArray(), keypoints_obj, desc_obj, 0);
  //surf(gray_img_scene, noArray(), keypoints_scene, desc_scene, 0);

  //SurfFeatureDetector detector(400);
  //detector.detect(gray_img_obj, keypoints_obj);
  //detector.detect(gray_img_scene, keypoints_scene);

  //SurfDescriptorExtractor extractor;
  //extractor.compute(gray_img_obj, keypoints_obj, desc_obj);
  //extractor.compute(gray_img_scene, keypoints_scene, desc_scene);

  //initModule_nonfree();
  //Ptr<FeatureDetector> detector = FeatureDetector::create( "SURF" );  
  //Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create( "SURF" );
  //detector->detect(gray_img_obj, keypoints_obj);
  //detector->detect(gray_img_scene, keypoints_scene);
  //extractor->compute(gray_img_obj, keypoints_obj, desc_obj);
  //(*extractor).compute(gray_img_scene, keypoints_scene, desc_scene);

  std::cout<<desc_obj.size()<<std::endl;
  //FlannBasedMatcher matcher;
  //std::vector< DMatch > matches;
  //matcher.match( desc_scene, desc_obj, matches);
 
  BruteForceMatcher< L2<float> > matcher;  
  std::vector< DMatch > matches;  
  matcher.match( desc_scene, desc_obj, matches);  

  std::vector<Point2f> scene;
  std::vector<Point2f> obj;
  std::vector<KeyPoint> k_scene;
  std::vector<KeyPoint> k_obj;

  double max_dist = 0; double min_dist = 100;
  for( int i = 0; i < desc_scene.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  std::vector< DMatch > good_matches;

  for( int i = 0; i < desc_scene.rows; i++ )
  { 
    if( matches[i].distance < 3*min_dist )
    { 
     good_matches.push_back( matches[i]); 
    }
  }

  for( int i = 0; i < good_matches.size(); i++ )
  {
    scene.push_back( keypoints_scene[ good_matches[i].queryIdx ].pt );
    obj.push_back( keypoints_obj[ good_matches[i].trainIdx ].pt );
    k_scene.push_back( keypoints_scene[ good_matches[i].queryIdx ] );
    k_obj.push_back( keypoints_obj[ good_matches[i].trainIdx ] );
  }
  if(scene.size()<4) return -1;
  Mat H = findHomography( obj, scene, CV_RANSAC );
 
  Mat result;
  warpPerspective(img_scene, result, H.inv(), Size(img_scene.cols+img_obj.cols,img_scene.rows));
  Mat half(result,Rect(0,0,img_obj.cols,img_obj.rows));
  img_obj.copyTo(half);
  //drawKeypoints(img_scene, k_scene, img_scene, Scalar(0, 0, 255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  //drawKeypoints(img_obj, k_obj, img_obj, Scalar(0, 0, 255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  imwrite( "Result.jpg", result );
/*
    Mat img_obj = imread("scene.jpg");
    Mat img_scene = imread("01.jpg");
    
    Mat gray_img_obj;
    Mat gray_img_scene;
    
    cvtColor(img_obj, gray_img_obj, CV_RGB2GRAY);
    cvtColor(img_scene, gray_img_scene, CV_RGB2GRAY);
    
    int minHessian = 400;
    SurfFeatureDetector detector(minHessian);
    
    vector<KeyPoint>keypoints_obj, keypoints_scene;
    detector.detect(gray_img_obj, keypoints_obj);
    detector.detect(gray_img_scene, keypoints_scene);
    
    SurfDescriptorExtractor extractor;
    
    Mat desc_obj, desc_scene;
    
    extractor.compute(gray_img_obj, keypoints_obj, desc_obj);
    extractor.compute(gray_img_scene, keypoints_scene, desc_scene);
*/


  //Mat img_matches;
  //drawMatches( img_obj, keypoints_obj, img_scene, keypoints_scene,
  //             matches, img_matches, Scalar::all(-1), Scalar::all(-1),
  //             vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
  //drawKeypoints(image, keypoints, image, Scalar(0, 0, 255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  //imwrite("Result.jpg", img_matches);



  //-- Get the corners from the image_1 ( the object to be "detected" )
  //std::vector<Point2f> corners1(4);
  //corners1[0] = cvPoint(0,0); 
  //corners1[1] = cvPoint( img_obj.cols, 0 );
  //corners1[2] = cvPoint( img_obj.cols, img_obj.rows );
  //corners1[3] = cvPoint( 0, img_obj.rows );

  //std::vector<Point2f> corners2(4);

  //perspectiveTransform( corners1, corners2, H);

  //for(int i = 0; i < 4; ++i)
  //      line( img_matches, corners2[i] + Point2f( img_obj.cols, 0), corners2[(i + 1) % 4] + Point2f( img_obj.cols, 0), Scalar(0, 255, 0), 4 );

  //for(int i = 0; i < 4; ++i)
  //      line( img_matches, corners1[i] , corners1[(i + 1) % 4], Scalar(0, 255, 0), 4 );
  //imwrite( "Result.jpg", img_matches );


  return 0;
}














//void function_name()
//{
//  namedWindow("face detection",1);
//  Mat frame;
//  VideoCapture cap(0);
//  while(cap.isOpened()) {
//    cap>>frame;
//    if (frame.empty())
//    {
//      break;
//    }
//    imshow("video",frame);
//    if(waitKey(30)>=0) break;
//  }
//  cap.release();
//}
//void sift_test()
//{
//    const Mat input = imread("lena.jpg", 0); //Load as grayscale
//
//    SiftFeatureDetector detector;
//    vector<KeyPoint> keypoints;
//    detector.detect(input, keypoints);
//
//    Mat output;
//    drawKeypoints(input, keypoints, output);
//    imshow("sift_result.jpg", output);
//    waitKey(0);
//  
//}
