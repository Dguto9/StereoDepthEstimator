cmake_minimum_required(VERSION 3.12)

project(StereoDepthEstimator)

add_subdirectory(ImageProcessingLibrary)

if (NOT TARGET MatrixModule)
	add_subdirectory(MatrixModule)
endif()

add_executable(StereoDepthEstimator Main.cpp)

target_include_directories(StereoDepthEstimator PUBLIC MatrixModule ImageProcessingLibrary)

target_link_libraries(StereoDepthEstimator PUBLIC ImageProcessingLibrary)