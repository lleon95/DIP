#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <boost/program_options.hpp>

#define RED_COLOR  CV_RGB(255,0,0)
#define BLUE_COLOR CV_RGB(0,0,255)
#define GREEN_COLOR CV_RGB(0,255,0)


#define FILENAME "parameters.yml"


#define CALIBRATE_MODE 1
#define AXIS_MODE 2


using namespace cv;
namespace po = boost::program_options;

int numBoards = 1;
int numCornersHor;
int numCornersVer;

Size board_sz= Size(numCornersHor, numCornersVer);

Mat image;
Mat gray_image;

VideoCapture capture = VideoCapture(0);

Mat intrinsics;
Mat distortion;



int operationMode= CALIBRATE_MODE;


int calibrateCam(){

  printf("Enter number of boards to analize: ");
  scanf("%d", &numBoards);

  // Compute num of squares and board size
  int numSquares = numCornersHor * numCornersVer;
  board_sz = Size(numCornersHor, numCornersVer);

  intrinsics;
   // Space reference
  vector<vector <Point3f> > object_points;
   // Image reference
  vector<vector <Point2f> > image_points;

  vector<Point2f> corners;
  int successes=0;

  // Trasladar imagen capturada a image

  capture >> image;

  // Vertexes
  vector<Point3f> obj;
  for(int j=0;j<numSquares;j++)
      obj.push_back(Point3f(j/numCornersHor, j%numCornersHor, 0.0f));

  // Detect all the square
  while(successes<numBoards)
  {
      // To grayscale
      cvtColor(image, gray_image, CV_BGR2GRAY);
      // Find corners
      bool found = findChessboardCorners(gray_image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
      if(found)
      {
          cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
          drawChessboardCorners(gray_image, board_sz, corners, found);
      }
      // Show images
      imshow("Result", gray_image);
      // New capture
      capture >> image;
      // Catch key
      int key = waitKey(1);
      // Escape
      if(key==27)
          return 0;
      // Space
      if(key== ' ' &&found!=0)
      {
          image_points.push_back(corners);
          object_points.push_back(obj);
          printf("Snap stored!");

          successes++;

          if(successes>=numBoards)
              break;
      }
  }
  //First camera matrix aproximation based on objectPoints
  intrinsics = initCameraMatrix2D(object_points, image_points, board_sz);


  vector<Mat> rvecs;
  vector<Mat> tvecs;

  // Calibrate camera
  calibrateCamera(object_points, image_points, image.size(), intrinsics, distortion, rvecs, tvecs);
}

void drawAxis(Mat* image,  vector<Point2d> imageFramePoints){
  line(*image, imageFramePoints[0], imageFramePoints[1], GREEN_COLOR, 5 );
  line(*image, imageFramePoints[0], imageFramePoints[2], RED_COLOR , 5 );
  line(*image, imageFramePoints[0], imageFramePoints[3], BLUE_COLOR, 5 );
}

int identifyAxis(){
    board_sz = Size(numCornersHor, numCornersVer);

    // Set up matrices for transformation vectors
    Mat rvec,tvec;
    //setup vectors to hold the chessboard corners in the chessboard coordinate system and in the image
    vector<Point2d> imageFramePoints, imageOrigin;
    vector<Point3d> boardPoints, framePoints;
    vector<Point2f> imagePoints;

    //Calculates the position in space
    for( int i = 0; i < board_sz.height; ++i )
            for( int j = 0; j < board_sz.width; ++j )
                boardPoints.push_back(Point3f(float(j), float( i), 0));

    framePoints.push_back(Point3f(0, 0, 0));               //Origin
    framePoints.push_back(Point3f(3, 0, 0));    //X axis
    framePoints.push_back(Point3f(0, 3, 0));    //Y axis
    framePoints.push_back(Point3f(0, 0, -3));   //Z axis




    // Routine for capturing pictures
    bool doneYet = false;
    while(!doneYet){
        capture.read(image);

        // Detect board corners
        bool found = findChessboardCorners(image, board_sz, imagePoints,
                CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
        // Find camera orientation
        if(found){
                solvePnP(boardPoints, imagePoints, intrinsics, distortion, rvec, tvec);
                projectPoints(framePoints, rvec, tvec, intrinsics, distortion, imageFramePoints);

                drawAxis(&image, imageFramePoints);
        }

     // Show picture
     imshow("Result", image);
     // Catch key
         int key = waitKey(1);
         // Escape
         if(key==27)
             return 0;
    }
    return 0;

}



void saveData(){
  try{
    FileStorage fs(FILENAME, FileStorage::WRITE);
    fs<< "Intrinsics"<<intrinsics;
    fs<< "Distortion" <<distortion;
    fs.release();
  }
  catch(std::exception& e)
  {
      std::cout << e.what() << "\n";
  }

}

int loadData(){
  try{
    FileStorage fs;
    fs.open(FILENAME, FileStorage::READ);
    if (!fs.isOpened())
          {
              std::cerr << "Failed to open " << FILENAME << std::endl;
              return 1;
          }

    fs["Intrinsics"]>>intrinsics;
    fs["Distortion"]>>distortion;
    fs.release();
  }
  catch(std::exception& e)
  {
      std::cout << e.what() << "\n";
  }

}
int main(int ac, char* av[]){
  try{
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "help")
    ("mode,m",po::value<int>()->default_value(1), "operation mode 1: Calibrate Camera 2: Show Axis")
    ("hor,h",po::value<int>()->default_value(9), "number of corners along width")
    ("ver,v",po::value<int>()->default_value(7), "number of corners along height")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    if (vm.count("mode")) {}
    if (vm.count("hor")) {}
    if (vm.count("ver")) {}

    numCornersHor=vm["hor"].as<int>();
    numCornersVer=vm["ver"].as<int>();
    operationMode= vm["mode"].as<int>();
  }
  catch(std::exception& e)
  {
      std::cout << e.what() << "\n";
  }

  if(operationMode==CALIBRATE_MODE){
    calibrateCam();
    saveData();
    operationMode=AXIS_MODE;
  }
  if(operationMode==AXIS_MODE){
    loadData();
    identifyAxis();
  }
}



/*
    References:
    http://aishack.in/tutorials/calibrating-undistorting-opencv-oh-yeah/
    http://www.swarthmore.edu/NatSci/mzucker1/opencv-2.4.10-docs/doc/tutorials/calib3d/camera_calibration_square_chess/camera_calibration_square_chess.html
      http://www.swarthmore.edu/NatSci/mzucker1/opencv-2.4.10-docs/doc/tutorials/calib3d/camera_calibration/camera_calibration.html
    https://github.com/foxymop/3DPoseEstimation/tree/master/src
    https://docs.opencv.org/3.3.0/d7/d53/tutorial_py_pose.html
*/
