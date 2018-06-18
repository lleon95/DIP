
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>


/** Global variables */
cv::String cascade_name = "../training/data/cascade.xml";
cv::CascadeClassifier cascade;
std::string window_name = "Butterfly detection";
std::string resultsPath="./results/";
cv::RNG rng(12345);
int minNeighbors = 300;
int imageCounter=0;

void cropButterfly(const cv::Mat &frame, cv::Rect myRec,std::string imageName);
/**
 * @function detectAndDisplay
 */
void detectAndDisplay( cv::Mat frame )
{
  imageCounter++;
   cv::vector<cv::Rect> butterflies;
   cv::Mat frame_gray;
   cvtColor( frame, frame_gray, CV_BGR2GRAY );
   cv::equalizeHist( frame_gray, frame_gray );

   // Sensitivity
   cascade.detectMultiScale( frame_gray, butterflies, 1.1, minNeighbors, 0, cv::Size(100, 100) );
   int offsetRect=40;
   for( int i = 0; i < butterflies.size(); i++ )
    {
      cv::Mat faceROI = frame_gray( butterflies[i] );
      //std::vector<Rect> eyes;

      // Compute vertexes
      cv::Point vertex1(butterflies[i].x-offsetRect, butterflies[i].y-offsetRect);
      cv::Point vertex2(butterflies[i].x + butterflies[i].width, butterflies[i].y + butterflies[i].height);

      // Draw rectangle

      cv::Rect myRec(vertex1,vertex2);

      cv::rectangle(frame, myRec, cv::Scalar( 0, 0, 255 ));
      std::string imageName= "r" + static_cast<std::ostringstream*>( &(std::ostringstream() << i) )->str()
                              +  static_cast<std::ostringstream*>( &(std::ostringstream() << imageCounter) )->str()+".jpg";
      cropButterfly(frame,myRec,imageName);

    }
   //-- Show what you got
   cv::imshow( window_name , frame );
   cv::waitKey(0);   // Wait for a keystroke in the window
}


void cropButterfly(const cv::Mat &frame, cv::Rect myRec,std::string imageName){


  std::string resultName= resultsPath + imageName;

  cv::Mat croppedImage = frame(myRec);
  cv::imwrite(resultName, croppedImage);

}
