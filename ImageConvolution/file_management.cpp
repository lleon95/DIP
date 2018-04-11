#include <opencv2/core/core.hpp>
#include <fstream>
#include <string>

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
