/*
    Institution: Tecnologico de Costa Rica
    Course: Digital Image Processing
    Author: Luis Leon Vega | Javier Sancho
    Last Uptate: March/5th/2018

    Code: This is a Cambell-Robson chart plotter

    References:
    https://docs.opencv.org/3.1.0/d3/d63/classcv_1_1Mat.html
    https://stackoverflow.com/questions/9250079/drawing-and-plotting-graph-in-opencv
    https://docs.opencv.org/3.1.0/d6/d6e/group__imgproc__draw.html#ga7078a9fae8c7e7d13d24dac2520ae4a2
    https://docs.opencv.org/3.1.0/d0/d2a/contours2_8cpp-example.html#a13
*/

// Libaries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <math.h>

using namespace cv;
using namespace std;

// Definitions
#define default_cols 1024
#define default_rows 512
#define default_selected_row 450
#define default_frequency 100
#define PI 3.14159265

// Default values
int cols = default_cols; // cols
int rows = default_rows; // rows
int selected_row = default_selected_row;
float k = default_frequency; // Min desired frequency


// Signal X
float Sx(int x)
{
    float nx = PI*pow(x,2);
    float dx = (5*(rows-1));
    return sin(nx/dx);
}
// Signal Y
int Ay(int y)
{
    return y*127/(cols-1);
}
// Signal in point
int Cxy(int x, int y)
{
    
    return Sx(x)*Ay(y)+128;
}
// Compute Alphas
float Alpha(int R)
{
    // We already have k
    return log((R-1)/k)/(R-1);
}
// Cxy Log Scale
int Clxy(int x, int y)
{
    // We already have k
    // Compute Alpha
    float Alphax = Alpha(rows);
    float Alphay = Alpha(cols);
    // Compute f
    int fx = k*exp(x*Alphax);
    int fy = k*exp(y*Alphay);
    // Result scaled
    return Cxy(fx,fy);
}
// Row plotter
Mat plotGraph(vector<int>& vals)
{
    // Remember: x: 0 variable, y: 0 255
    Mat image = Mat::zeros(cols, vals.size(), CV_8UC3 );
    image.setTo(255);
    for (int i = 1; i < (int)vals.size()-1; i++)
    {
        Point start = Point(i-1, 255 - vals[i-1]);
        Point end = Point(i, 255 - vals[i]);
        line(image, start, end, 0);
        //cout << vals[i] << endl;
    }

    return image;
}
int main(int ac, char* av[])
{
    try {

        /*
            Creating the images
            Missing: Select kind of char (put some if and aight)
            Change this parameters: cols, rows, k
            Where k is the desired frequency
        */       
        // Filling a Matrix with Zeros
        Mat imgLinear = Mat::zeros(rows,cols, CV_8UC1);
        Mat imgLog = Mat::zeros(rows,cols, CV_8UC1);
        // Columns editor
        for(int n = 0; n < cols; n++)
        {
            // Rows editor
            for(int m = 0; m < rows; m++)
            {
                // Generating points
                imgLinear.at<uchar>(Point(n, m)) = Cxy(n,m); // Linear image generator
                imgLog.at<uchar>(Point(n, m)) = Clxy(n,m);   // Log image generator
            }
        }
        // Create a window for display.
        namedWindow( "Linear Plot", WINDOW_AUTOSIZE );  
        namedWindow( "Logarithmic Plot", WINDOW_AUTOSIZE );  
        // Show our image inside it.
        imshow( "Linear Plot", imgLinear);        
        imshow( "Logarithmic Plot", imgLog);

        /*
            Creating the row plotter
            Missing: Select this kind of plot
            Modify selected_row in order to change the row
        */  
        vector<int> plot;  
        imgLog.row(selected_row).copyTo(plot);
        Mat lineGraph = plotGraph(plot);  
        // Create a window for display.
        namedWindow( "Row Plotter", WINDOW_AUTOSIZE ); 
        // Show our image inside it.
        imshow( "Row Plotter", lineGraph); 
        // Wait for a keystroke in the window 
        waitKey(0);   
            

    }
    // Error Handling
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }
    return 0;
}


