cmake_minimum_required(VERSION 3.12)
project(CrossPlatformPowerPoint)

set(CMAKE_CXX_STANDARD 11)

add_executable(CrossPlatformPowerPoint gesture_pptx.cpp)

if(WIN32)
    # Windows-specific code
    target_compile_definitions(CrossPlatformPowerPoint PRIVATE _WIN32)
    target_link_libraries(CrossPlatformPowerPoint PRIVATE your_powerpoint_interop_library)
elseif(APPLE)
    # macOS-specific code
    find_library(APPKIT_LIBRARY AppKit)
    target_link_libraries(CrossPlatformPowerPoint PRIVATE ${APPKIT_LIBRARY})
endif()