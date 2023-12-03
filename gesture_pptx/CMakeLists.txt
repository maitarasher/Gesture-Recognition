cmake_minimum_required(VERSION 3.12)
project(CrossPlatformPowerPoint)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

SET(SOURCE_FILES
    gesture_pptx.cpp
    ../src/mediapipe_client/mediapipe_client.hpp
    ../src/mediapipe_client/mediapipe_client.cpp
    ../src/data_classes.hpp
)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ../src/mediapipe_client

)
    

add_executable(CrossPlatformPowerPoint ${SOURCE_FILES})

target_link_libraries(CrossPlatformPowerPoint PRIVATE ${OpenCV_LIBS})

# macOS-specific code
find_library(APPKIT_LIBRARY AppKit)
target_link_libraries(CrossPlatformPowerPoint PRIVATE ${APPKIT_LIBRARY})
