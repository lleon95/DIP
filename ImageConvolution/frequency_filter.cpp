
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <numeric>
#include <chrono>

void impulseResponseLinear(cv::Mat& kernel,int kernel_size){

kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

}

void impulseResponseGaussian(cv::Mat& kernel,int kernel_size){

  double sigma= (kernel_size + 2)/6;


  cv::Mat kernelX = cv::getGaussianKernel(kernel_size, sigma,CV_32F);
  cv::Mat kernelY = cv::getGaussianKernel(kernel_size, sigma,CV_32F);
  kernel= kernelX * kernelY.t();

}



void addPadding(cv::Mat& source, cv::Mat& aux, cv::Mat& dest){

  int source_cols= source.cols;
  int source_rows=source.rows;

  int aux_cols= aux.cols;
  int aux_rows=aux.rows;

  int top= 0;
  int bottom=aux_rows-1;
  int left=0;
  int right= aux_cols -1;

  cv::copyMakeBorder(source, dest, top,bottom,left,right, cv::BORDER_CONSTANT,cv::Scalar(0,0,0));
}

void calculateDFT(cv::Mat& src, cv::Mat& dest){

    cv::Mat planes[] = {cv::Mat_<float>(src), cv::Mat::zeros(src.size(), CV_32F)};
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros


    cv::dft(complexI, complexI);            // this way the result may fit in the source matrix
    dest= complexI;
}

void calculateInverseDFT(cv::Mat& src, cv::Mat& dest){
  cv::idft(src,dest,cv::DFT_COMPLEX_OUTPUT| cv::DFT_SCALE);
}


void reconstructImage(cv::Mat& original,cv::Mat& filtered){
  cv::Mat padded;
  cv::copyMakeBorder(original, padded, 0, original.rows, 0, original.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
  cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
  cv::split(filtered, planes);
  cv::magnitude(planes[0], planes[1], planes[0]);
  cv::Mat mag = planes[0];

  //mag += cv::Scalar::all(1);
  //cv::log(mag, mag);

  // crop the spectrum, if it has an odd number of rows or columns
  mag = mag(cv::Rect(0, 0, mag.cols & -2, mag.rows & -2));

  int cx = mag.cols/2;
  int cy = mag.rows/2;

  cv::normalize(mag, filtered, 1, 0, CV_MINMAX);
}

void resizeImg(cv::Mat &img,cv::Mat& ker, int offsetx, int offsety){
    cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    cv::warpAffine(img,img,trans_mat,cv::Size(img.cols-ker.cols+1,img.rows-ker.rows+1));
}


double applyFrequencyFilter(cv::Mat& image, cv::Mat& kernel, cv::Mat& res){
  cv::Mat paddK,paddI,dftK,dftI,iDFT;

  auto start = std::chrono::high_resolution_clock::now();
  addPadding(image,kernel,paddI);
  addPadding(kernel,image,paddK);

  std::cout<<paddI.cols<<paddI.rows<<std::endl;
  calculateDFT(paddI,dftI);
  calculateDFT(paddK,dftK);

  cv::mulSpectrums(dftI,dftK,iDFT,cv::DFT_COMPLEX_OUTPUT);

  calculateInverseDFT(iDFT,res);

  reconstructImage(image,res);
  // Stop and record counter
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end-start;



  resizeImg(res,kernel,-((kernel.rows-1)/2),-((kernel.rows-1)/2));
  return diff.count();



}


/*int main(int argc, char const *argv[]) {
  cv::Mat kernel,image,res;

  image = cv::imread("images/220x220.png", CV_LOAD_IMAGE_GRAYSCALE);
  impulseResponseGaussian(kernel,13);

  applyFrequencyFilter(image,kernel,res);

  std::cout<<res.cols<<res.rows<<std::endl;
  /// Create window
 cv::imshow("original",image);
 cv::imshow("gaus",res);
 cv::waitKey(0);
  return 0;

}*/
