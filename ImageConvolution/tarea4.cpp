
#include <boost/program_options.hpp>
#include <cmath>
#include  "file_management.cpp"
#include "space_filter.cpp"
#include "frequency_filter.cpp"
// Define 10 runs for time averaging
#define nRuns 1


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



double divergencia(cv::Mat& img1, cv::Mat& img2){

    double diver=0;
    cv::Mat diffM;
    absdiff(img1, img2,diffM);

    for(int i=0;i<diffM.rows; i++ ){
      for(int j=0; i<diffM.cols; j++){

        double pixel= diffM.at<double>(i,j);
        diver= diver+ pixel*pixel;
      }
    }
    return diver/(diffM.rows*diffM.cols);

}
// ########################################################################################
// Main Routine
// ########################################################################################


int main(int ac, char* av[]){

    // Vars needed
    cv::Mat src, dst,kernel;
    int kernel_size;

    try {

        /*
            Getting the command parameters
        */
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("input-file,i", po::value< std::vector<std::string> >(), "input file Usage: ./tarea4 -i picture1 picture2 [...]")
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
        if (vm.count("help")) {;
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
            std::ofstream resultsSLS,resultsNSLS, resultsGS,resultsLF,resultsGF,diver ;
            createNewFile(resultsSLS, "resultsSLS.txt");
            createNewFile(resultsNSLS, "resultsNSLS.txt");
            createNewFile(resultsGS, "resultsGS.txt");
            createNewFile(resultsLF, "resultsLF.txt");
            createNewFile(resultsGF, "resultsGF.txt");
            createNewFile(diver, "divergencia.txt");


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
                    cv::Mat dstGS=dst;
                    // Average
                    writeRowInFile(resultsGS, "Gauss_Space", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);
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
                    cv::Mat dstLS=dst;
                    // Average
                    writeRowInFile(resultsSLS, "SepLinear_Space", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);
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
                    writeRowInFile(resultsNSLS, "NonSepLinear_Space", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);

                    // ##################
                    // Gaussian
                    // ##################
                    std::cout << "Filtering with Gaussian in Frequency - ImageSize:" << srcSize.width << "x" << srcSize.height << " KernelSize:"<< kSize << std::endl;
                    // Run each kernel n times for averaging
                    impulseResponseGaussian(kernel,kSize);
                    for(int i = 0; i < nRuns; i++)
                    {
                        double elapsedTime = 0;
                        elapsedTime=applyFrequencyFilter(src,kernel,dst);
                        timeSum += elapsedTime;
                    }
                    // Average
                    writeRowInFile(resultsGF, "Gauss_Frequency", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);
                    double errorG= divergencia(dstGS,dst);
                    writeRowInFile(diver, "Gauss_diver", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);

                    // ##################
                    // Linear
                    // ##################
                    std::cout << "Filtering with Linear Filter in frequency - ImageSize:" << srcSize.width << "x" << srcSize.height << " KernelSize:"<< kSize << std::endl;
                    // Run each kernel n times for averaging
                    timeSum = 0;
                    impulseResponseLinear(kernel,kSize);
                    for(int i = 0; i < nRuns; i++)
                    {
                        double elapsedTime = 0;
                        elapsedTime=applyFrequencyFilter(src,kernel,dst);
                        timeSum += elapsedTime;
                    }
                    // Average
                    writeRowInFile(resultsLF, "Linear_Frequency", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);
                    double errorL= divergencia(dstLS,dst);
                    writeRowInFile(diver, "Gauss_diver", src.size(), cv::Size(kSize,kSize), timeSum/nRuns);
                }


                // ------------------------------
                // Debugging
                // ------------------------------
                // See below

            }
            //cv::waitKey(0);   // Wait for a keystroke in the window
            std::cout << "Execution Finished..." << std::endl;
            closeFile(resultsSLS);
            closeFile(resultsNSLS);
            closeFile(resultsGS);
            closeFile(resultsGF);
            closeFile(resultsLF);
            closeFile(diver);

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
