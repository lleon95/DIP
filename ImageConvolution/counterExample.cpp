#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <iostream>
#include <boost/program_options.hpp>
#include <numeric>
#include <chrono>

// Define 10 runs for time averaging
#define n 10



int main(int ac, char* av[]){

    // Start counter
    auto start = std::chrono::high_resolution_clock::now();

    // Dummy delay
    int acc = 0;
    for(int i = 0; i < 1000; i++)
    {
        acc+=i;
    }

    // Stop and record counter
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    // Output: diff.count()
    std::cout << "Duration: " << diff.count() << std::endl;
}


/*
    References:
        http://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now
*/


/*
    Final Table Scheme:
        ImageSize   KernelSize  AvgTime
*/