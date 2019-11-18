cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 14)

project( myopencv1 )
set(OpenCV_DIR "/home/tms/software/opencv-4.1.2/build/" )
find_package( OpenCV 4 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( myopencv1     otsu.cpp)
target_link_libraries( myopencv1 ${OpenCV_LIBS} )