cmake_minimum_required(VERSION 3.12)
project(CrossPlatformPowerPoint)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(CrossPlatformPowerPoint gesture_pptx.cpp)

target_link_libraries(CrossPlatformPowerPoint PRIVATE ${OpenCV_LIBS})

# macOS-specific code
find_library(APPKIT_LIBRARY AppKit)
target_link_libraries(CrossPlatformPowerPoint PRIVATE ${APPKIT_LIBRARY})
