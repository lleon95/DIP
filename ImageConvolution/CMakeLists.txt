
set( OpenCV_FOUND 1 )
cmake_minimum_required(VERSION 2.4.9)
project( tarea4 )
set( OpenCV_DIR "/usr/local/share/OpenCV" )
find_package( OpenCV REQUIRED )
add_executable( tarea4 tarea4.cpp )
target_link_libraries( tarea4 ${OpenCV_LIBS} -lboost_program_options )
