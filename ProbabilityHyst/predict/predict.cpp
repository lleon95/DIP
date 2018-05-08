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

#include "predictFrame.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  po::options_description desc("Allowed options");
  desc.add_options()
    ("video,v",po::value<int>()->default_value(0),
     "video device")
    ("histograms,h",po::value<std::string>()->default_value("hists.yaml"),
     "name of file used to store the histograms")
    ("apriori,p",po::value<float>()->default_value(0.5f),
     "a-priori probability of being object")
    ("iterations,i",po::value<std::uint32_t>()->default_value(0u),
     "number of iterations of using last result as a-priori probability")
    ("gaussian,g",po::value<std::uint32_t>()->default_value(3u),
     "size of the gaussian filter used ad every iteration to smooth a-priori "
     "probability")
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
  paid::predictFrame p(vm["video"].as<int>(),
                       vm["histograms"].as<std::string>(),
                       vm["apriori"].as<float>(),
                       vm["iterations"].as<std::uint32_t>(),
                       vm["gaussian"].as<std::uint32_t>());
  std::cout << "done.\n" << std::endl;

  if (p.loop()) {
    return EXIT_SUCCESS;
  }
  
  return EXIT_FAILURE;
}

