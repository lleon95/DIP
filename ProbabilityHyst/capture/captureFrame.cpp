/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, TEC, Costa Rica
 *
 * This file is part of CE5201 Image Processing and Analysis lecture at TEC
 *
 * @Author: Pablo Alvarado
 * @Date  : 24.03.2018
 */

#include "captureFrame.hpp"

namespace paid {

  captureFrame::captureFrame(const int videv,
                             const fs::path& dir,
                             const std::string& name)
    : _videoDev(videv)
    , _dir(dir)
    , _counter(0u)
    , _imgName(name)
    , _cap(videv)
  {
    ensureDirectory();
    cv::namedWindow("Capture frame",CV_WINDOW_NORMAL);
    cv::Mat frame;
    _cap >> frame; // capture one frame just to force initialization
  }
  
  captureFrame::~captureFrame() {
  }
  
  void captureFrame::help() {
    std::cout << "SPC capture next image.\n"
              << "X   stop capturing images" << std::endl;
  }
  
  bool captureFrame::next() {
    if (!_cap.isOpened()) {
      return false;
    }
    
    cv::Mat frame;
    
    for (;;) {
      _cap >> frame;
      cv::imshow("Capture frame",frame);
      
      int key = cv::waitKey(10);
      if (key > 0) {
        key = key & 0xFF;
        if ( (key == 'X') || (key == 'x') || (key == 27) ) {
          return false;
        }
      
        // std::cout << "Key: " << key << std::endl;
      
        std::string f = fs::canonical(_dir).string() + '/' + nextFilename();
        std::cout << "Saving '" << f << "'... " << std::flush;
        try {
          cv::imwrite(f,frame);
          std::cout << "done." << std::endl;
          ++_counter;
          return true;
        } catch(std::exception& ex) {
          std::cerr << "failed.\nError: " << ex.what() << std::endl;
          return false;
        }
      }
    }
  }

  /// Ensure the directory exists
  bool captureFrame::ensureDirectory() {

    try {
      boost::system::error_code ec;
      
      if (fs::is_directory(fs::path(_dir),ec)) {
        return true;
      }
      
      if (fs::create_directories(fs::path(_dir))) {
        return true;
      }
    } catch (...) {
      return false;
    }
                     
    return false;
  }

  /// Create the next filename
  std::string captureFrame::nextFilename() {
    std::stringstream ss;

    ss << _imgName << "_";
    ss.fill('0');
    ss.width(2);
    ss << _counter << ".png";

    return ss.str();
  }
} // end of namespace paid
