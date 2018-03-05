
set( OpenCV_FOUND 1 )
cmake_minimum_required(VERSION 2.4.9)
project( cambell_robson )
set( OpenCV_DIR "/usr/local/share/OpenCV" )
find_package( OpenCV REQUIRED )
add_executable( cambell_robson cambell_robson.cpp )
target_link_libraries( cambell_robson ${OpenCV_LIBS} -lboost_program_options)
