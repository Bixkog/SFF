cmake_minimum_required(VERSION 2.8)
project( SFF )
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++17 -ggdb -g -Wall -Wextra - Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++17 -Ofast -flto -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")

find_package( OpenCV REQUIRED )
add_executable( SFF sff.cpp 
					aux.h 
					aux.cpp 
					Image.h 
					depth.h
					depth.cpp
					focus.h
					focus.cpp)
target_link_libraries( SFF ${OpenCV_LIBS} )
