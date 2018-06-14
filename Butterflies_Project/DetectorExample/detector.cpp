/**
 * @file objectDetection2.cpp
 * @author A. Huaman ( based in the classic facedetect.cpp in samples/c )
 * @brief A simplified version of facedetect.cpp, show how to load a cascade classifier and how to find objects (Face + eyes) in a video stream - Using LBP here
 */
 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include <boost/program_options.hpp>
 
 #include <iostream>
 #include <stdio.h>
 
 // Namespaces
 namespace po = boost::program_options;
 using namespace std;
 using namespace cv;
 
 /** Function Headers */
 void detectAndDisplay( Mat frame );
 
 /** Global variables */
 String cascade_name = "training/data/cascade.xml";
 CascadeClassifier cascade;
 string window_name = "Butterfly detection";
 
 RNG rng(12345);

 // A helper function to simplify the main part.
 template<class T>
 ostream& operator<<(ostream& os, const vector<T>& v)
 {
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
    return os;
 }
 
 /**
  * @function main
  */
 int main( int ac, const char* av[] )
 {

  try {
    
      /*
          Getting the command parameters
      */
      po::options_description desc("Allowed options");
      desc.add_options()
          ("help", "produce help message")
          ("input-file", po::value< vector<string> >(), "input file")
      ;

      po::positional_options_description p;
      p.add("input-file", -1);

      po::variables_map vm;
      po::store(po::command_line_parser(ac, av).
                options(desc).positional(p).run(), vm);
      po::notify(vm);

      /*
          Help message
      */
      if (vm.count("help")) {
          cout << "Usage: ./imageLoader picture1 picture2 [...]\n";
          cout << desc;
          return 0;
      }
      /*
          Showing the images given by the parameters
      */
      if (vm.count("input-file"))
      {

          /*
              Executing the image showing
          */       
          vector<string> path = vm["input-file"].as< vector<string> >();

          //-- 1. Load the cascade
          if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
          //-- 2. Load the pictures
          for(int i = 0; i < path.size(); i++)
          {
              // Load each image
              Mat image;
              // Read the file
              image = imread(path[i], CV_LOAD_IMAGE_COLOR);   
              // Check for invalid input
              if(! image.data )                              
              {
                  cout <<  "Could not open or find the image" << "\n" ;
                  return -1;
              } 
              //-- 3. Apply detector
              detectAndDisplay( image );               
          }
          waitKey(0);   // Wait for a keystroke in the window
      }
      else
      {
          cout << "Error: there aren't pictures to load" << "\n";
      }

  }
  // Error Handling
  catch(std::exception& e)
  {
      cout << e.what() << "\n";
      return 1;
  }
  return 0;

 }
 
 /**
  * @function detectAndDisplay
  */
 void detectAndDisplay( Mat frame )
 {
    std::vector<Rect> faces;
    Mat frame_gray;
 
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
 
    //-- Detect faces
    cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(80, 80) );
 
    for( int i = 0; i < faces.size(); i++ )
     {
       Mat faceROI = frame_gray( faces[i] );
       //std::vector<Rect> eyes;
        
       //-- Draw the face
       Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
       ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );
       
       /* NOT NEEDED
            //-- In each face, detect eyes
            eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            if( eyes.size() == 2)
            {
                //-- Draw the face
                Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
                ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );
        
                for( int j = 0; j < eyes.size(); j++ )
                { //-- Draw the eyes
                    Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 ); 
                    int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                    circle( frame, center, radius, Scalar( 255, 0, 255 ), 3, 8, 0 );
                }
                }
        */
     } 
    //-- Show what you got
    imshow( window_name, frame );
 }
 