/*
    Institution: Tecnologico de Costa Rica
    Course: Digital Image Processing
    Author: Luis Leon Vega
    Last Uptate: Feb/11th/2018

    Code: This is a image loader based on OpenCV Library.

    https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_gcc_cmake/linux_gcc_cmake.html
*/

// Libaries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>

// Namespaces
using namespace boost;
namespace po = boost::program_options;

using namespace cv;
using namespace std;

// Defining the index of first image
#define FIRST_IMAGE 0

// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
    return os;
}


int main(int ac, char* av[])
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
                // Create a window for display.
                namedWindow( "Image Loader " + path[i], WINDOW_AUTOSIZE );  
                // Show our image inside it.
                imshow( "Image Loader " + path[i], image );                   
                
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
