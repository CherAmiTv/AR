cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
project(AR)

INCLUDE(FindPkgConfig)

find_package (OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

SET(CMAKE_CXX_FLAGS -pthread)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(include)
include_directories(lib)

file(GLOB GKIT lib/*.cpp lib*.h)
file(GLOB SRC src/*.cpp include/*.h)

add_executable(AR ${SRC} ${GKIT})
target_link_libraries (AR ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${GLEW_LIBRARY})

add_executable(calibrage Calibrage/calibre.cpp)
target_link_libraries(calibrage ${OpenCV_LIBS})
