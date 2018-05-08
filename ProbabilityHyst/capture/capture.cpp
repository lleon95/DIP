/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, TEC, Costa Rica
 *
 * This file is part of CE5201 Image Processing and Analysis lecture at TEC
 *
 * @Author: Pablo Alvarado
 * @Date  : 24.03.2018
 */


#include <cstdlib>
#include <string>
#include <iostream>

#include <boost/program_options.hpp>

#include "captureFrame.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  po::options_description desc("Allowed options");
  desc.add_options()
    ("dir,d",po::value<std::string>()->default_value("img/"),
     "destination directory")
    ("img,i",po::value<std::string>()->default_value("img"),
     "image name")
    ("video,v",po::value<int>()->default_value(0),
     "video device")
    ("help,h", "produce help message")
    ;

  po::variables_map vm;
  po::store(parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << '\n';
    return EXIT_SUCCESS;
  }

  std::cout << "Initializing... " << std::flush;
  paid::captureFrame c(vm["video"].as<int>(),
                       fs::path(vm["dir"].as<std::string>()),
                       vm["img"].as<std::string>());
  std::cout << "done.\n" << std::endl;

  c.help();
  
  for(;c.next();) {}

  
  return EXIT_FAILURE;

}

