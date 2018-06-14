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
 
 /** Function Headers */
 void detectAndDisplay( cv::Mat frame );
 
 /** Global variables */
 cv::String cascade_name = "../training/data/cascade.xml";
 cv::CascadeClassifier cascade;
 std::string window_name = "Butterfly detection";
 
 cv::RNG rng(12345);

 // A helper function to simplify the main part.
 template<class T>
 std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
 {
    copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
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
            ("help,h", "produce help message")
            ("cascade,c", po::value<std::string>()->default_value(cascade_name), "It is the cascade.xml file")
            ("input-file,i", po::value< std::vector<std::string> >(), "input file Usage: ./tarea4 -i picture1 picture2 [...]")
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
            std::cout << "Usage: ./detector -c cascade.xml - i picture1 picture2 [...]\n";
            std::cout << "       ./detector picture1 picture2 [...]\n";
            std::cout << "       ./detector -i picture1 picture2 [...]\n";
            std::cout << desc;
            return 0;
        }

        if(vm.count("input-file"))
        {

            // Load images
            std::vector<std::string> path = vm["input-file"].as< std::vector<std::string> >();
            // Load cascade
            cascade_name = vm["cascade"].as<cv::String>();

            //-- 1. Load the cascade
            if( !cascade.load( cascade_name ) ){ printf("Error: error loading cascade file\n"); return -1; };
            //-- 2. Load the pictures
            if(0 < path.size())
            {
                // Load each one
                for(int i = 0; i < path.size(); i++)
                {
                    // Load each image
                    cv::Mat image;
                    // Read the file
                    image = cv::imread(path[i], CV_LOAD_IMAGE_COLOR);   
                    // Check for invalid input
                    if(! image.data )                              
                    {
                        std::cout <<  "Error: Could not open or find the image" << "\n" ;
                        return -1;
                    } 
                    //-- 3. Apply detector
                    detectAndDisplay( image );               
                }

            }
            else
            {
                // Not images loaded
                std::cout <<  "Error: Could not open or find the image" << "\n" ;
            }
        }
        else
        {
            // Not images loaded
            std::cout <<  "Error: No images were selected" << "\n" ;
        }
   

  }
  // Error Handling
  catch(std::exception& e)
  {
      std::cout << e.what() << "\n";
      return 1;
  }
  return 0;

 }
 
 /**
  * @function detectAndDisplay
  */
 void detectAndDisplay( cv::Mat frame )
 {
    cv::vector<cv::Rect> faces;
    cv::Mat frame_gray;
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    cv::equalizeHist( frame_gray, frame_gray );
 
    //-- Detect faces
    cascade.detectMultiScale( frame_gray, faces, 1.1, 150, 0, cv::Size(100, 100) );
 
    for( int i = 0; i < faces.size(); i++ )
     {
       cv::Mat faceROI = frame_gray( faces[i] );
       //std::vector<Rect> eyes;
        
       // Compute vertexes
       cv::Point vertex1(faces[i].x, faces[i].y);
       cv::Point vertex2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

       // Draw rectangle
       cv::rectangle(frame, vertex1, vertex2, cv::Scalar( 0, 0, 255 ), 2, 8, 0);
       
     } 
    //-- Show what you got
    cv::imshow( window_name , frame );
    cv::waitKey(0);   // Wait for a keystroke in the window
 }
 