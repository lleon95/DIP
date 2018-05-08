# Copyright (C) 2018
# Área Académica de Ingeniería en Computadoras, ITCR, Costa Rica
#
# This file is part of CE5201 Image Processing and Analysis lecture at TEC
#
# Author: Pablo Alvarado
# Date  : 24.03.2018

set( OpenCV_FOUND 1 )
cmake_minimum_required(VERSION 3.5)
set( OpenCV_DIR "/usr/local/share/OpenCV" )

find_package ( Boost COMPONENTS system filesystem program_options REQUIRED )
include_directories (${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

find_package( OpenCV REQUIRED )


file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.?pp *.h)

list(REMOVE_ITEM SRCS "train.cpp")

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(CheckIncludeFiles)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
  # Specify best possible CPU architecture for available hardware
  # The following line fails in some architectures
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

#CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/cmake/PaidConfig.hpp.in ${CMAKE_SOURCE_DIR}/include/PaidConfig.hpp)

add_executable(trainer train.cpp ${SRCS} ${HEADERS})
target_link_libraries(trainer ${OpenCV_LIBS} ${Boost_LIBRARIES})
