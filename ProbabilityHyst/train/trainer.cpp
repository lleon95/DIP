/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, TEC, Costa Rica
 *
 * This file is part of CE5201 Image Processing and Analysis lecture at TEC
 *
 * @Author: Pablo Alvarado
 * @Date  : 24.03.2018
 */

#include <boost/filesystem.hpp>
#include "trainer.hpp"
#include <math.h>  
#include <cstdint>

namespace paid {

  trainer::trainer(const fs::path& dir,
                   const std::string& name,
                   const int bins)
    : _dir(dir)
    , _imgName(name)
    , _bins(bins)
  {
    // Create the two histograms
    std::vector<int> b(3,bins);

    // A 3D histogram initialized with a residual to avoid division by
    // zero.
    _objHist = cv::Mat(3,b.data(),CV_32F,
                       cv::Scalar(1.0f/(_bins*_bins*_bins)));
    _nonObjHist = _objHist.clone();

    // Compute the fast access LUT
    computeLUT();
  }
    
  trainer::~trainer() {
  }

  bool trainer::train() {
    
    for (std::size_t i=0;i<100;++i) {
      
      std::string f = nextFilename(i);
      std::string mf = maskName(f);
      
      if (fs::exists(fs::path(f))) {

        std::cout << "File: '" << f << "'... " << std::flush;
        
        cv::Mat img,msk;

        img = cv::imread(f,CV_LOAD_IMAGE_COLOR);
        
        if (img.data != 0) {
          msk = cv::imread(mf,CV_LOAD_IMAGE_GRAYSCALE);

          if ( (img.rows != msk.rows) ||
               (img.cols != msk.cols) ) {
            std::cerr << "failed.\n  Error: mask has the wrong size."
                      << std::endl;
          }
          
          if (msk.data != 0) {

            // TODO: PUT YOUR CODE HERE!!!
            // Number of pixelsimg_size
            double P_pixel = msk.rows*msk.cols*_bins*_bins; // CHECK THIS
            P_pixel = 1 / P_pixel;
            // Explore all pixels
            for(int row = 0; row < msk.rows; row++)
            {
              for(int col = 0; col < msk.cols; col++)
              {
                // Position: row, col
                // - Get clasification
                uchar classification = msk.at<uchar>(cv::Point(row,col)) ;
                // - There are 3 colours 
                for(int colour = 0; colour < 3; colour++)
                {
                  // Get colour
                  uchar colour_val = img.at<cv::Vec3b>(cv::Point(row,col))[colour];
                  // Put in a bin
                  colour_val = floor(colour_val/_bins);
                  // Classify
                  if(classification > 192)
                  {
                    // It is an object
                    switch(colour)
                    {
                      case 0: // Blue
                        _objHist.at<float>(colour_val,0,0) += P_pixel;
                        break;
                      case 1: // Green
                        _objHist.at<float>(0,colour_val,0) += P_pixel;
                        break;
                      case 2: // Red
                        _objHist.at<float>(0,0,colour_val) += P_pixel;
                        break;
                      default:
                        break;
                    }
                  }
                  else if(classification < 64)
                  {
                    // It is not an object
                    switch(colour)
                    {
                      case 0: // Blue
                        _nonObjHist.at<float>(colour_val,0,0) += P_pixel;
                        break;
                      case 1: // Green
                        _nonObjHist.at<float>(0,colour_val,0) += P_pixel;
                        break;
                      case 2: // Red
                        _nonObjHist.at<float>(0,0,colour_val) += P_pixel;
                        break;
                      default:
                        break;
                    }
                  }
                }
              }
            }
              
            std::cout << "P: " << P_pixel << "done." << std::endl;
          } else {
            std::cerr << "failed.\n  Error: no mask found." << std::endl;
          }
        }
      } else {
        // File does not exist.  Simply stop for now
        return true;
      }
    }

    return false;
  }

  bool trainer::save(const std::string& filename) const {
    try {
      cv::FileStorage fs(filename,cv::FileStorage::WRITE);
      fs << "bins" << _bins;
      fs << "obj" << _objHist;
      fs << "nobj" << _nonObjHist;
      fs.release();
      return true;
    } catch(std::exception& ex) {
      std::cout << "Error: " << ex.what() << std::endl;
    }
    return false;
  }
    
  /**
   * Return the next filename or an empty string if no image is
   * found.
   */
  std::string trainer::nextFilename(const int number) const {

    std::stringstream ss;

    ss << _imgName << "_";
    ss.fill('0');
    ss.width(2);
    ss << number << ".png";
    
    return (fs::canonical(_dir).string() + '/' + ss.str());
  }

  std::string trainer::maskName(const std::string& file) const {
    std::string str=file.substr(0,file.find(".png"));
    str += "_mask.png";
    return str;
  }
  
  void trainer::computeLUT() {
    _l2i.assign(256,0);
    _mskLUT.assign(256,Ignore);
    
    for (size_t i=0;i<_l2i.size();++i) {
      _l2i[i] = static_cast<int>(float(i)*_bins/_l2i.size());

      if (i<64)  _mskLUT[i] = NonObject;
      if (i>192) _mskLUT[i] = Object;
    }
  }
  
} // end namespace paid
