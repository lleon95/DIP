
set( OpenCV_FOUND 1 )
cmake_minimum_required(VERSION 2.4.9)
project( getParameters )
set( OpenCV_DIR "/usr/local/share/OpenCV" )
find_package( OpenCV REQUIRED )
add_executable( getParameters getParameters.cpp )
target_link_libraries( getParameters ${OpenCV_LIBS} -lboost_program_options )
