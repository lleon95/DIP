/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, TEC, Costa Rica
 *
 * This file is part of CE5201 Image Processing and Analysis lecture at TEC
 *
 * @Author: Pablo Alvarado
 * @Date  : 24.03.2018
 */

#include "predictFrame.hpp"

namespace paid {

  predictFrame::predictFrame(const int videv,
                             const std::string& histoName,
                             const float apriori,
                             const std::uint32_t iterations,
                             const std::uint32_t gaussian)
    : _videoDev(videv)
    , _histoName(histoName)
    , _apriori(apriori)
    , _iterations(iterations)
    , _gaussian(1u + 2u*(gaussian/2u)) // ensure odd size
    , _sigma( (_gaussian+2u)/6.0 )
    , _cap(videv)
  {
    loadHistograms(_histoName);
    computeLUT();
    cv::namedWindow("Probability",CV_WINDOW_NORMAL);
    cv::namedWindow("Input frame",CV_WINDOW_NORMAL);
    cv::Mat frame;
    _cap >> frame; // capture one frame just to force initialization
  }
  
  predictFrame::~predictFrame() {
  }

  bool predictFrame::predict(const cv::Mat& input,cv::Mat& prediction) const {

    // Verify that the input image is a 3 channel color image
    if (input.type() != CV_8UC3) {
      return false;
    }
    
    // only reserve memory if necessary
    if ( (input.rows != prediction.rows) ||
         (input.cols != prediction.cols) ) {
      prediction = cv::Mat_<float>(input.rows,input.cols,float());
    }

    assert(prediction.type() == CV_32F);

    // TODO: PUT YOUR CODE HERE

    return true;
  }

  void predictFrame::computeLUT() {
    _l2i.assign(256,0);
    
    for (size_t i=0;i<_l2i.size();++i) {
      _l2i[i] = static_cast<int>(float(i)*_bins/_l2i.size());
    }
  }

    
  bool predictFrame::loop() {
    if (!_cap.isOpened()) {
      return false;
    }

    static const char* progress = "/-\\|";
    
    cv::Mat frame,pred;

    size_t i=0;
    
    for (;;) {
      _cap >> frame;
      cv::imshow("Input frame",frame);
      
      int key = cv::waitKey(10);
      if (key > 0) {
        key = key & 0xFF;
        if ( (key == 'X') || (key == 'x') || (key == 27) ) {
          return true;
        }
      }

      std::cout << "\r" << progress[i++ & 0x03] << std::flush;
      
      predict(frame,pred);
      cv::imshow("Probability",pred);
    }
  }

  /// Ensure the directory exists
  bool predictFrame::loadHistograms(const std::string& filename) {
    try {
      cv::FileStorage fs(filename,cv::FileStorage::READ);
      fs["bins"] >> _bins;
      fs["obj"]  >> _objHist;
      fs["nobj"] >> _nonObjHist;
      fs.release();
    } catch(std::exception& ex) {
      std::cout << "Error: " << ex.what() << std::endl;
      return false;
    }

    // Normalize histograms
    _objHist    /= cv::sum(_objHist)[0];
    _nonObjHist /= cv::sum(_nonObjHist)[0];
    
    return true;
  }

} // end of namespace paid
