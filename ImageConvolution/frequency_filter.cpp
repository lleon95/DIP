
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <numeric>
#include <chrono>


/*
Obtiene la respuesta al impulso de un filtro lineal
*/
void impulseResponseLinear(cv::Mat& kernel,int kernel_size){

kernel = cv::Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

}
/*
obtiene la respuesta al impulso de un filtro gaussiano
*/
void impulseResponseGaussian(cv::Mat& kernel,int kernel_size){

  double sigma= (kernel_size + 2)/6;


  cv::Mat kernelX = cv::getGaussianKernel(kernel_size, sigma,CV_32F);
  cv::Mat kernelY = cv::getGaussianKernel(kernel_size, sigma,CV_32F);
  kernel= kernelX * kernelY.t();

}


/*
agrega padding a una imagen segun una imagen auxiliar, siendo el nuevo tamaño
de la imagen source.size +aux.size -1
*/

void addPadding(cv::Mat& source, cv::Mat& aux, cv::Mat& dest){


  int aux_cols= aux.cols;
  int aux_rows=aux.rows;

  int top= 0;
  int bottom=aux_rows-1;
  int left=0;
  int right= aux_cols -1;

  cv::copyMakeBorder(source, dest, top,bottom,left,right, cv::BORDER_CONSTANT,cv::Scalar(0,0,0));
}
/*
Calcula la transformada discreta de fourier de una imagen
*/
void calculateDFT(cv::Mat& src, cv::Mat& dest){

    cv::Mat planes[] = {cv::Mat_<float>(src), cv::Mat::zeros(src.size(), CV_32F)};
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros


    cv::dft(complexI, complexI);            // this way the result may fit in the source matrix
    dest= complexI;
}


/*
Calcula la transformada inversa de fourier a una imagen discretamente
*/
void calculateInverseDFT(cv::Mat& src, cv::Mat& dest){
  cv::idft(src,dest,cv::DFT_COMPLEX_OUTPUT| cv::DFT_SCALE);
}

/*
Reconstruye la imagen luego de obtener la inversa de la transformada discreta de fourier
a partir de la imagen filtrada y la original (para colocarle el mismo tamaño)
*/
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

/*
Mueve la imagen centrada al punto (0,0) y elimina el borde negro generado anteriormente para realizar la convolucion
*/
void resizeImg(cv::Mat &img,cv::Mat& ker, int offsetx, int offsety){
    cv::Mat trans_mat = (cv::Mat_<double>(2,3) << 1, 0, offsetx, 0, 1, offsety);
    cv::warpAffine(img,img,trans_mat,cv::Size(img.cols-ker.cols+1,img.rows-ker.rows+1));
}

/*
Realiza todo el proceso de convolucion en la frecuencia de una imagen con un respectivo filtro
Retornando la imagen ya filtrada en el espacio al realizar la inversa de DFT de la imagen resultante

*/

double applyFrequencyFilter(cv::Mat& image, cv::Mat& kernel, cv::Mat& res){
  cv::Mat paddK,paddI,dftK,dftI,iDFT;

  auto start = std::chrono::high_resolution_clock::now();
  addPadding(image,kernel,paddI);
  addPadding(kernel,image,paddK);

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
