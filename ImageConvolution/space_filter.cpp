
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <numeric>
#include <chrono>



// ------------------------------
// Prepare Kernel
// ------------------------------
void makeKernel(cv::Mat& kernel, int kernel_size)
{
    kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
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
// Linear filters
// ------------------------------

// No separable filter
void ApplyNoSeparableLinearFilter(const cv::Mat& src, cv::Mat& dst, cv::Mat& kernel, double& elapsedTime)
{

    // Applying the filter
    cv::Ptr<cv::FilterEngine> filter2D = cv::createLinearFilter(src.type(), dst.type(), kernel);
    elapsedTime = ApplySpaceFilter(src,dst,filter2D);
}

// Separable filter
void ApplySeparableLinearFilter(const cv::Mat& src, cv::Mat& dst, cv::Mat& kernel, double& elapsedTime)
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
