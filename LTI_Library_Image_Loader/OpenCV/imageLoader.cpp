/*
    Institution: Tecnologico de Costa Rica
    Course: Digital Image Processing
    Author: Luis Leon Vega
    Last Uptate: Feb/11th/2018

    Code: This is a image loader based on OpenCV Library.

    https://docs.opencv.org/2.4/doc/tutorials/introduction/linux_gcc_cmake/linux_gcc_cmake.html
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include <boost/program_options.hpp>

using namespace boost;
namespace po = boost::program_options;

using namespace cv;
using namespace std;


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
            // Load image
            cout << "Info: " <<path.size() << " pictures to load" << "\n";
            for(unsigned int i = 0; i < path.size() ;i++)
            {
                Mat image;
                image = imread(path[i], CV_LOAD_IMAGE_COLOR);   // Read the file
            
                if(! image.data )                              // Check for invalid input
                {
                    cout <<  "Could not open or find the image" << std::endl ;
                    return -1;
                }
            
                namedWindow( "Display window: " + i, WINDOW_AUTOSIZE );// Create a window for display.
                imshow( "Display window "+ path[i], image );                   // Show our image inside it.
            
                
            }
            waitKey(0);                                          // Wait for a keystroke in the window
            
        }
        else
        {
            cout << "Error: there aren't pictures to load" << "\n";
        }

    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }
    return 0;
}

/* using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
} */