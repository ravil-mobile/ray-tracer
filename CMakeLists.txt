cmake_minimum_required(VERSION 3.5)
project(ray-tracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED filesystem program_options)
find_package (Eigen3 3.3 REQUIRED)
find_package(JPEG REQUIRED)
find_package(OpenMP REQUIRED)

file(GLOB SRC_FILES src/*.cpp src/Engine/*.cpp src/Misc/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
        Boost::filesystem
        Boost::program_options
        Eigen3::Eigen
        JPEG::JPEG
        OpenMP::OpenMP_CXX)
