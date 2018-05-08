/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, TEC, Costa Rica
 *
 * This file is part of CE5201 Image Processing and Analysis lecture at TEC
 *
 * @Author: Pablo Alvarado
 * @Date  : 24.03.2018
 */

#ifndef PAID_CAPTURE_FRAME_HPP
#define PAID_CAPTURE_FRAME_HPP

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

namespace fs = boost::filesystem;

namespace paid {

  /**
   * Capture frames from a video stream taken from a video device.
   */
  class captureFrame {
  public:
    /// Capture from the video device into the given directory
    captureFrame(const int videv,
                 const fs::path& dir,
                 const std::string& name);
    
    ~captureFrame();
    
    /// Print help
    void help();
    
    /**
     * Video capture
     *
     * This method shows the video stream, until the space bar is pressed,
     * when the current frame is stored.  
     *
     * If the x-key is pressed, then false is returned and no image is saved.
     *
     * @return true if the image is properly saved 
     */
    bool next();
    
  private:
    
    /// Ensure the directory exists
    bool ensureDirectory();
    
    /// Create the next filename
    std::string nextFilename();
    
    /// Video device
    int _videoDev;
    
    /// Directory path
    fs::path _dir;
    
    /// Counter
    std::size_t _counter;
    
    /// Image name
    std::string _imgName;
    
    cv::VideoCapture _cap;
  };

} // end namespace paid
  
#endif
