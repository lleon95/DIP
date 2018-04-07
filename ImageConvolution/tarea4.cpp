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



// ########################################################################################
// Space - Filters
// ########################################################################################

// ------------------------------
// Linear filters
// ------------------------------

// No separable filter
void ApplyNoSeparableLinearFilter(const cv::Mat& src, cv::Mat& dst, int kernel_size)
{
    // ------------------------------
    // Creating a kernel
    // ------------------------------
    cv::Mat kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
    // ------------------------------
    // Applying the filter
    // ------------------------------
    cv::Ptr<cv::FilterEngine> filter2D = cv::createLinearFilter(src.type(), dst.type(), kernel);
    filter2D->apply(src, dst);
}

// Separable filter
void ApplySeparableLinearFilter(const cv::Mat& src, cv::Mat& dst, int kernel_size)
{
    // ------------------------------
    // Creating kernels
    // ------------------------------
    cv::Mat rowkernel = cv::Mat::ones( kernel_size, 1, CV_32F )/ (float)(kernel_size);
    cv::Mat colkernel = cv::Mat::ones( 1, kernel_size, CV_32F )/ (float)(kernel_size);
    // ------------------------------
    // Applying the filter
    // ------------------------------
    cv::Ptr<cv::FilterEngine> filter2D = cv::createSeparableLinearFilter(src.type(), dst.type(), rowkernel, colkernel);
    filter2D->apply(src, dst);
}

// ------------------------------
// Gaussian filter
// ------------------------------
// Sigma computation
double sigmaCompute(int kernel_size)
{
    return (kernel_size + 2)/6;
}

// Gaussian filter
void ApplyGaussianFilter(const cv::Mat& src, cv::Mat& dst, int kernel_size)
{
    // ------------------------------
    // Applying the filter
    // ------------------------------
    // Compute sigma
    double sigma = sigmaCompute(kernel_size);
    // Create an applicable filter
    cv::Ptr<cv::FilterEngine> filter2D = cv::createGaussianFilter(src.type(), cv::Size(kernel_size,kernel_size), sigma, sigma);
    // Apply
    filter2D->apply(src, dst);
    
}

// ########################################################################################
// Main Routine
// ########################################################################################


int main(int ac, char* av[]){
    
    // Vars needed
    cv::Mat src, dst;
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
        /*https://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html#Ptr%3CFilterEngine%3E%20createLinearFilter(int%20srcType,%20int%20dstType,%20InputArray%20kernel,%20Point%20_anchor,%20double%20delta,%20int%20rowBorderType,%20int%20columnBorderType,%20const%20Scalar&%20borderValue)
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
                cvtColor(src, src, CV_BGR2GRAY);  
                dst = src.clone();
                // Check for invalid input
                if(! src.data )                              
                {
                    std::cout <<  "Could not open or find the image" << "\n" ;
                    return -1;
                } 
                
                // ------------------------------
                // Debugging
                // ------------------------------
                //ApplyNoSeparableLinearFilter(src, dst, 9);
                //ApplySeparableLinearFilter(src, dst, 9);
                ApplyGaussianFilter(src, dst, 9);

                // Test
                // Create a window for display.
                cv::namedWindow( "Original", cv::WINDOW_AUTOSIZE );  
                cv::namedWindow( "Filtrada", cv::WINDOW_AUTOSIZE );  
                // Show our image inside it.
                cv::imshow( "Original", src);    
                cv::imshow( "Filtrada", dst);    
                
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
        https://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html#FilterEngine
        https://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html#Ptr%3CFilterEngine%3E%20createSeparableLinearFilter(int%20srcType,%20int%20dstType,%20InputArray%20rowKernel,%20InputArray%20columnKernel,%20Point%20anchor,%20double%20delta,%20int%20rowBorderType,%20int%20columnBorderType,%20const%20Scalar&%20borderValue)
        https://docs.opencv.org/2.4/modules/core/doc/basic_structures.html
*/


/*
    Final Table Scheme:
        ImageSize   KernelSize  AvgTime

    For the point 3:
        Substract one img (time filtered) to other (frequency filtered)
        Use frobenius (maximize)
        Use Abs
*/