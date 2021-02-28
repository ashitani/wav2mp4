cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(TARGET wav2mp4)
project(${TARGET})
add_executable(${TARGET} ${TARGET}.cpp)

# openCV
set(OpenCV_DIR $ENV{HOME}/git/opencv/build)
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    target_include_directories(${TARGET} PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${TARGET} ${OpenCV_LIBS})
endif()

# wave (https://github.com/audionamix/wave)
target_include_directories(${TARGET} PUBLIC wave/src)
add_library(wave
  wave/src//wave/header/data_header.h
  wave/src//wave/header/data_header.cc
  wave/src//wave/header/riff_header.h
  wave/src//wave/header/riff_header.cc
  wave/src//wave/header/fmt_header.h
  wave/src//wave/header/fmt_header.cc
  wave/src//wave/header/wave_header.h
  wave/src//wave/header/wave_header.cc
  wave/src//wave/header.h
  wave/src//wave/header.cc
  wave/src//wave/header_list.h
  wave/src//wave/header_list.cc
  wave/src//wave/error.h
  wave/src//wave/file.h
  wave/src//wave/file.cc)
target_include_directories(wave PUBLIC wave/src)
target_link_libraries(${TARGET} wave)
