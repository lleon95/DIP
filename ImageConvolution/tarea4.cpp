#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <numeric>
#include <chrono>

// Define 10 runs for time averaging
#define n 10

// Boost program options
namespace po = boost::program_options;

// A helper function to simplify the main part.
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
    return os;
}


int main(int ac, char* av[]){
    
    // Vars needed
    cv::Mat src, dst;
    cv::Mat kernel;
    int kernel_size;

    try {

        /*
            Getting the command parameters
        */
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("input-file", po::value< std::vector<std::string> >(), "input file")
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
            std::cout << "Usage: ./tarea4 picture1 picture2 [...]\n";
            std::cout << desc;
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
            std::vector<std::string> path = vm["input-file"].as< std::vector<std::string> >();
            
            for(int i = 0; i < path.size(); i++)
            {
                // ------------------------------
                // Loading each image
                // ------------------------------
                // Read the file
                src = cv::imread(path[i], CV_LOAD_IMAGE_COLOR);   
                // Check for invalid input
                if(! src.data )                              
                {
                    std::cout <<  "Could not open or find the image" << "\n" ;
                    return -1;
                } 
                // ------------------------------
                // Creating a kernel
                // ------------------------------
                kernel_size = 10;
                kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
                // ------------------------------
                // Applying the filter
                // ------------------------------
                cv::Ptr<cv::FilterEngine> filter2D = cv::createLinearFilter(src.type(), src.type(), kernel);
                cv::filter2D(src, dst, -1, kernel);

                // Test
                // Create a window for display.
                cv::namedWindow( "Image Loader ", cv::WINDOW_AUTOSIZE );  
                // Show our image inside it.
                cv::imshow( "Image Loader ", dst );    
                
            }
            cv::waitKey(0);   // Wait for a keystroke in the window 
            
            
        }
        else
        {
            std::cout << "Error: there aren't pictures to load" << "\n";
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


/*
    References:
        http://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now
        https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/filter_2d/filter_2d.html
        https://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html#Ptr%3CFilterEngine%3E%20createLinearFilter(int%20srcType,%20int%20dstType,%20InputArray%20kernel,%20Point%20_anchor,%20double%20delta,%20int%20rowBorderType,%20int%20columnBorderType,%20const%20Scalar&%20borderValue)
*/


/*
    Final Table Scheme:
        ImageSize   KernelSize  AvgTime
*/