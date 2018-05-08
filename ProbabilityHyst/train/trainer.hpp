/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, TEC, Costa Rica
 *
 * This file is part of CE5201 Image Processing and Analysis lecture at TEC
 *
 * @Author: Pablo Alvarado
 * @Date  : 24.03.2018
 */

#ifndef PAID_TRAINER_HPP
#define PAID_TRAINER_HPP

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

#include <vector>

namespace fs = boost::filesystem;

namespace paid {

  /**
   * Capture frames from a video stream taken from a video device.
   */
  class trainer {
  public:
    /// Capture from the video device into the given directory
    trainer(const fs::path& dir,
            const std::string& name,
            const int bins);
    
    ~trainer();

    /// Try to train with the data
    bool train();

    /// Save the histogram with YAML in the given file
    bool save(const std::string& filename) const;
    
  private:
    /**
     * Return the next filename or an empty string if no image is
     * found.
     */
    std::string nextFilename(const int number) const;

    /**
     * Add the prefix "_mask" to the filename
     */
    std::string maskName(const std::string& file) const;

    /**
     * Compute the LUT to convert a channel level to a bin index
     */
    void computeLUT();
    
    /// Directory path
    fs::path _dir;
       
    /// Image name
    std::string _imgName;

    /// Number of bins for each dimension of the histograms
    int _bins;

    /// Object histogram
    cv::Mat _objHist;

    /// Non-object histogram
    cv::Mat _nonObjHist;

    /// Channel level to index LUT
    std::vector<int> _l2i;

    enum MaskLabel {
      Ignore,
      Object,
      NonObject
    };
    
    /// LUT to convert a mask value into 
    std::vector<MaskLabel> _mskLUT;  
  };

} // end namespace paid
  
#endif
