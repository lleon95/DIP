cmake_minimum_required(VERSION 3.5)


project(tarea05 VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(capture)
add_subdirectory(predict)
add_subdirectory(train)

