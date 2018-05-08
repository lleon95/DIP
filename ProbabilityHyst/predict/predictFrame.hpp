/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, TEC, Costa Rica
 *
 * This file is part of CE5201 Image Processing and Analysis lecture at TEC
 *
 * @Author: Pablo Alvarado
 * @Date  : 24.03.2018
 */

#ifndef PAID_PREDICT_FRAME_HPP
#define PAID_PREDICT_FRAME_HPP

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

namespace fs = boost::filesystem;

namespace paid {

  /**
   * Predict frames from a video stream taken from a video device.
   */
  class predictFrame {
  public:
    /// Predict from the video device into the given directory
    predictFrame(const int videv,
                 const std::string& histoName,
                 const float apriori=0.5f,
                 const std::uint32_t iterations=0,
                 const std::uint32_t gaussian=3);
    
    ~predictFrame();

    bool predict(const cv::Mat& input,cv::Mat& prediction) const;

    
    /**
     * Video predict
     *
     * This method captures one frame from the video stream, and estimates
     * the probability of each pixel to be a pixel based on the histograms
     * for object and non-object.
     *
     * If the x-key is pressed, then false is returned and no image is saved.
     *
     * @return true if the image is properly saved 
     */
    bool loop();
    
  private:
    
    /// Ensure the directory exists
    bool loadHistograms(const std::string& filename);

    /// Compute channel value to index LUT
    void computeLUT();
    
    /// Video device
    int _videoDev;

    /// Name of histograms file
    std::string _histoName;

    /// A priori probability of being object
    float _apriori;

    /// Number of iterations to re-used smoothed result as a-priori probability
    std::uint32_t _iterations;

    /// Gaussian size (must be odd)
    std::uint32_t _gaussian;

    /// Computed sigma
    double _sigma;
    
    /// Capture device 
    cv::VideoCapture _cap;

    /// Number of bins in each dimension of the histogram
    int _bins;
    
    /// Object histogram
    cv::Mat _objHist;

    /// Non-object histogram
    cv::Mat _nonObjHist;

    /// Channel level to index LUT
    std::vector<int> _l2i;
  };

} // end namespace paid
  
#endif
