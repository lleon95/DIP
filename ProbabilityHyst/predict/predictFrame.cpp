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
#include <math.h>

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

  // Gaussian filter to iterate the result


  bool predictFrame::predict(const cv::Mat& input,cv::Mat& prediction) const {

    // Verify that the input image is a 3 channel color image
    if (input.type() != CV_8UC3) {
      return false;
    }

    // only reserve memory if necessary
    if ( (input.rows != prediction.rows) ||
         (input.cols != prediction.cols) ) {
      prediction = cv::Mat_<float>(input.rows,input.cols, float());
    }

    assert(prediction.type() == CV_32F);

    // TODO: PUT YOUR CODE HERE

    /*
      Ecuaciones:
        p(objeto | c) = [p(c | objeto) p(objeto)] / p(c)
        p(c) = p(c|objeto)p(objeto) + p(c|¬objeto)p(¬objeto)
        Se parte con p(objeto) = 0.5

    */

   float P_Color_NoObj;
    float P_Color_Obj,P_Obj_Color;
    float P_Color;
    float P_Objeto = _apriori;


    // Iterations
    for(unsigned int iteration = 0; iteration < _iterations+1; iteration++)
    {
      // Get the pixel:
      for(int row = 0; row < prediction.rows; row++)
      {
        for(int col = 0; col < prediction.cols; col++)
        {
          // Establish initial probabilities
          if(iteration == 0)
          {
            prediction.at<float>(row, col) = _apriori;
          }
          else{
            //se le aplica un filtro gausiano a la prediccion calculada en la iteración anterior
            cv::Ptr<cv::FilterEngine> filter2D = cv::createGaussianFilter(prediction.type(), cv::Size(_gaussian,_gaussian), _sigma, _sigma);
            filter2D->apply(prediction, prediction);
          }
          // There are 3 colours in total
          for(int pix_colour = 0; pix_colour < 3; pix_colour++)
          {
            // Get colour:
            uchar colour_val = input.at<cv::Vec3b>(cv::Point(col,row))[pix_colour];
            // Get the bin
            colour_val = floor(colour_val/_bins);
            // Get the probability of the pixel to be object
            P_Objeto = prediction.at<float>(row, col);
            // Get bin probability: These are the probabilities of a color given the result (Obj or Not Obj)
            switch(pix_colour)
            {
              // Blue
              case 0:
                P_Color_NoObj = _nonObjHist.at<float>(colour_val,0,0);
                P_Color_Obj = _objHist.at<float>(colour_val,0,0);
                break;
              // Green
              case 1:
                P_Color_NoObj = _nonObjHist.at<float>(0,colour_val,0);
                P_Color_Obj = _objHist.at<float>(0,colour_val,0);
                break;
              // Red
              case 2:
                P_Color_NoObj = _nonObjHist.at<float>(0,0,colour_val);
                P_Color_Obj = _objHist.at<float>(0,0,colour_val);
                break;
              // Error case
              default:
                P_Color_NoObj = 0;
                P_Color_Obj = 0;
                break;
            }
            // Compute P_Color
            P_Color = P_Color_Obj*P_Objeto+P_Color_NoObj*(1-P_Objeto);

            // Compute the P_Obj_Color -> p(objeto | c) = [p(c | objeto) p(objeto)] / p(c)
            P_Obj_Color = (P_Color_Obj*P_Objeto)/P_Color;
            // Compute P_Objeto = Sum(P(Color_i|Obj)*P(Color_i)),i=0,i=2) on the pixel
            prediction.at<float>(row, col) += P_Obj_Color*P_Color;

          }
        }
      }

    }

    //std::cout << prediction.at<float>(prediction.rows/2, prediction.cols/2) << std::endl;


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
