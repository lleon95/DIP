#include <cv.h>
#include <highgui.h>
#include <stdio.h>

using namespace cv;

int main()
{
    int numBoards = 0;
    int numCornersHor;
    int numCornersVer;

    // Get parameters
    printf("Enter number of squares along width: ");
    scanf("%d", &numCornersHor);

    printf("Enter number of squares along height: ");
    scanf("%d", &numCornersVer);

    printf("Enter number of boards to analize: ");
    scanf("%d", &numBoards);

    // Compute num of squares and board size
    int numSquares = numCornersHor * numCornersVer;
    Size board_sz = Size(numCornersHor, numCornersVer);

    // Use camera
    VideoCapture capture = VideoCapture(0);
    // Space reference
    vector<vector <Point3f> > object_points;
    // Image reference
    vector<vector <Point2f> > image_points;

    vector<Point2f> corners;
    int successes=0;

    // Trasladar imagen capturada a image
    Mat image;
    Mat gray_image;
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
        imshow("win1", image);
        
        // New capture
        capture >> image;
        // Catch key
        int key = waitKey(1);
        // Escape
        if(key==27)
            return 0;
        // Space
        if(found!=0)
        {
            image_points.push_back(corners);
            object_points.push_back(obj);
            imshow("win2", gray_image);
            printf("Snap stored!");

            successes++;

            if(successes>=numBoards)
                break;
        }
    }
    // Matrix of intrinsic parameters
    Mat intrinsic = Mat(3, 3, CV_32FC1);
    Mat distCoeffs;
    vector<Mat> rvecs;
    vector<Mat> tvecs;
    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;

    // Calibrate camera
    calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

    // Parameters intrinsic, distCoeffs

    // TEST - correct distortion - superposition of axis is not done
    Mat imageUndistorted;
    while(1)
    {
        capture >> image;
        undistort(image, imageUndistorted, intrinsic, distCoeffs);

        imshow("win1", image);
        imshow("win2", imageUndistorted);
        waitKey(1);
    }
    capture.release();
    
        return 0;
}

    
