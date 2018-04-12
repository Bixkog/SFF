cmake_minimum_required(VERSION 2.8)
project( SFF )
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++17 -ggdb -g -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++17 -O3 -Wall -Wextra")

find_package( OpenCV REQUIRED )
add_executable( SFF sff.cpp aux.h aux.cpp image.h image.cpp)
target_link_libraries( SFF ${OpenCV_LIBS} )
