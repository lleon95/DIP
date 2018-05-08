/**
 * Copyright (C) 2018
 * Área Académica de Ingeniería en Computadoras, TEC, Costa Rica
 *
 * This file is part of CE5201 Image Processing and Analysis lecture at TEC
 *
 * @Author: 
 * @Date  : 24.02.2018
 */


#include <cstdlib>
#include <string>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "trainer.hpp"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

  po::options_description desc("Allowed options");
  desc.add_options()
    ("dir,d",po::value<std::string>()->default_value("img/"),
     "source directory")
    ("img,i",po::value<std::string>()->default_value("img"),
     "image name")
    ("bins,b",po::value<int>()->default_value(32),
     "number of bins in each dimension of the histogram")
    ("histograms,h",po::value<std::string>()->default_value("hists.yaml"),
     "name of file used to store the histograms")
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
  paid::trainer t(fs::path(vm["dir"].as<std::string>()),
                  vm["img"].as<std::string>(),
                  vm["bins"].as<int>());
  std::cout << "done.\n" << std::endl;

  if (t.train()) {
    if (t.save(vm["histograms"].as<std::string>())) {
      return EXIT_SUCCESS;
    }
  }

  return EXIT_FAILURE;
}

