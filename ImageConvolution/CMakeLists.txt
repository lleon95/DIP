set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)
project( tarea4 )
find_package( OpenCV REQUIRED )
FIND_PACKAGE( Boost 1.58 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable( tarea4 tarea4.cpp )
target_link_libraries( tarea4 ${OpenCV_LIBS} ${Boost_LIBRARIES} )
