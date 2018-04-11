#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <numeric>
#include <chrono>
#include <fstream>
#include <string>

// Define 10 runs for time averaging
#define nRuns 5
// Define kernel sizes
const std::vector<int> kernelSizes = {3,9,27,49,81,243,399,511,729,1023};
//const std::vector<int> kernelSizes = {3,9,27,49,81};

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
// Get time and apply filter
// ------------------------------
double ApplySpaceFilter(const cv::Mat& src, cv::Mat& dst, cv::Ptr<cv::FilterEngine> filter2D)
{
    // Start counter
    auto start = std::chrono::high_resolution_clock::now();
    // Apply Filter
    filter2D->apply(src, dst);
    // Stop and record counter
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    // Return time difference
    return diff.count();
}

// ------------------------------
// Prepare Kernel
// ------------------------------
void makeKernel(cv::Mat& kernel, int kernel_size)
{
    kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
}

// ------------------------------
// Linear filters
// ------------------------------

// No separable filter
void ApplyNoSeparableLinearFilter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& kernel, double& elapsedTime)
{
    // Applying the filter
    cv::Ptr<cv::FilterEngine> filter2D = cv::createLinearFilter(src.type(), dst.type(), kernel);
    elapsedTime = ApplySpaceFilter(src,dst,filter2D);
}

// Separable filter
void ApplySeparableLinearFilter(const cv::Mat& src, cv::Mat& dst, const cv::Mat& kernel, double& elapsedTime)
{
    // Creating kernels
    cv::Mat rowkernel = kernel.row(0);
    cv::Mat colkernel = kernel.col(0);
    // Applying the filter
    cv::Ptr<cv::FilterEngine> filter2D = cv::createSeparableLinearFilter(src.type(), dst.type(), rowkernel, colkernel);
    elapsedTime = ApplySpaceFilter(src,dst,filter2D);
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
void ApplyGaussianFilter(const cv::Mat& src, cv::Mat& dst, int kernel_size, double& elapsedTime)
{
    // Compute sigma
    double sigma = sigmaCompute(kernel_size);
    // Create an applicable filter
    cv::Ptr<cv::FilterEngine> filter2D = cv::createGaussianFilter(src.type(), cv::Size(kernel_size,kernel_size), sigma, sigma);
    // Apply
    elapsedTime = ApplySpaceFilter(src,dst,filter2D);
    
}

// ########################################################################################
// File management and tabulation
// ########################################################################################
void createNewFile(std::ofstream& newFile, std::string filename)
{
    // Create and open a new file
    newFile.open(filename);
    // Create header
    newFile << "Type\tImageSize\tKernelSize\tAvgTime\n";
} 

void closeFile(std::ofstream& file)
{
    file.close();
}

void writeRowInFile(std::ofstream& file, std::string registerType ,cv::Size ImageSize, cv::Size KernelSize, double AvgTime)
{
    file << registerType << "\t" << ImageSize.width << "x" << ImageSize.height << "\t" <<  KernelSize.width << "x" << KernelSize.height << "\t" << AvgTime << std::endl;
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

        /*
            Loading images from args
        */
       
        if (vm.count("input-file"))
        {
           
            // Image path vector       
            std::vector<std::string> path = vm["input-file"].as< std::vector<std::string> >();
            // Open new File for log
            std::ofstream results;
            createNewFile(results, "results.txt");
            // Explore all images
            std::cout << "Starting..." << std::endl;
            for(int i = 0; i < path.size(); i++)
            {
                // ------------------------------
                // Loading each image
                // ------------------------------
                // Read the file
                src = cv::imread(path[i], CV_LOAD_IMAGE_COLOR);
                std::cout << "Image: " << path[i] << " loaded..." << std::endl;
                cvtColor(src, src, CV_BGR2GRAY);  
                dst = src.clone();
                // Check for invalid input
                if(! src.data )                              
                {
                    std::cout <<  "Could not open or find the image" << "\n" ;
                    return -1;
                } 
                // Get size image
                cv::Size srcSize = src.size();
                // ------------------------------
                // Run SPACE filtering 
                // ------------------------------
                // Run each kernel
                for(int kSize : kernelSizes)
                {
                    double timeSum = 0;
                    // Create kernel
                    cv::Mat kernel;
                    makeKernel(kernel, kSize);
                    // ##################
                    // Gaussian
                    // ##################
                    std::cout << "Filtering with Gaussian in Space - ImageSize:" << srcSize.width << "x" << srcSize.height << " KernelSize:"<< kSize << std::endl;
                    // Run each kernel n times for averaging
                    for(int i = 0; i < nRuns; i++)
                    {
                        double elapsedTime = 0;
                        ApplyGaussianFilter(src, dst, kSize, elapsedTime);
                        timeSum += elapsedTime;
                    }
                    // Average
                    writeRowInFile(results, "Gauss_Space", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);
                    // ##################
                    // SeparableLinear
                    // ##################
                    std::cout << "Filtering with Separable Linear Filter in Space - ImageSize:" << srcSize.width << "x" << srcSize.height << " KernelSize:"<< kSize << std::endl;
                    // Run each kernel n times for averaging
                    timeSum = 0;
                    for(int i = 0; i < nRuns; i++)
                    {
                        double elapsedTime = 0;
                        ApplySeparableLinearFilter(src, dst, kernel, elapsedTime);
                        timeSum += elapsedTime;
                    }
                    // Average
                    writeRowInFile(results, "SepLinear_Space", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);
                    // ##################
                    // Non-SeparableLinear
                    // ##################
                    std::cout << "Filtering with Non-Separable Linear Filter in Space - ImageSize:" << srcSize.width << "x" << srcSize.height << " KernelSize:"<< kSize << std::endl;
                    // Run each kernel n times for averaging
                    timeSum = 0;
                    for(int i = 0; i < nRuns; i++)
                    {
                        double elapsedTime = 0;
                        ApplyNoSeparableLinearFilter(src, dst, kernel, elapsedTime);
                        timeSum += elapsedTime;
                    }
                    // Average
                    writeRowInFile(results, "NonSepLinear_Space", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);
                }


                // ------------------------------
                // Debugging
                // ------------------------------
                // See below 
                
            }
            //cv::waitKey(0);   // Wait for a keystroke in the window 
            std::cout << "Execution Finished..." << std::endl;
            closeFile(results);
            
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
        https://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html#Ptr%3CFilterEngine%3E%20createLinearFilter(int%20srcType,%20int%20dstType,%20InputArray%20kernel,%20Point%20_anchor,%20double%20delta,%20int%20rowBorderType,%20int%20columnBorderType,%20const%20Scalar&%20borderValue)
        
*/


/*
    Final Table Scheme:
        Type        ImageSize   KernelSize  AvgTime
    Ex:
        NonSepSpace 200x200     3x3         0.00003
        SepSpace    300x300     9x9         0.00050
        NonSepDFT   500x500     11x11       0.2230

    For the point 3:
        Substract one img (time filtered) with the other one (frequency filtered)
        Use frobenius (maximize)
        Use Abs
*/

/*
    How to use the printing function

    // Create the file once
    std::ofstream results;
    createNewFile(results, "results.txt");

    // To write the file, use:            
    writeRowInFile(results, "NonSepTest", src.size(), cv::Size(3,3), 0.0222);

    // At the end, close the file
    closeFile(results);
*/

/*

    Last debugging:

    //ApplyNoSeparableLinearFilter(src, dst, 9);
    //ApplySeparableLinearFilter(src, dst, 9);
    //ApplyGaussianFilter(src, dst, 9);
    double elapsedTime = 0;
    ApplyGaussianFilter(src, dst, 9, elapsedTime);
    std::cout << "Time: " << elapsedTime <<std::endl;

    std::ofstream results;
    createNewFile(results, "results.txt");
    writeRowInFile(results, "NonSepTest", src.size(), cv::Size(3,3), 0.0222);
    closeFile(results); 

    // Test
    // Create a window for display.
    cv::namedWindow( "Original", cv::WINDOW_AUTOSIZE );  
    cv::namedWindow( "Filtrada", cv::WINDOW_AUTOSIZE );  
    // Show our image inside it.
    cv::imshow( "Original", src);    
    cv::imshow( "Filtrada", dst);   
*/