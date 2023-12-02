cmake_minimum_required(VERSION 3.12)
project(CrossPlatformPowerPoint)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(OpenCV REQUIRED)
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(JSONCPP jsoncpp)
# link_libraries(${JSONCPP_LIBRARIES})
# link_directories()
# list(APPEND CMAKE_MODULE_PATH "/opt/homebrew/Cellar/jsoncpp/1.9.5/share/jsoncpp/cmake")

# Find Eigen library (if installed)
# find_package(Eigen3 QUIET)

# if (Eigen3_FOUND)
#     message(STATUS "Found Eigen3: ${Eigen3_INCLUDE_DIRS}")
#     include_directories(${Eigen3_INCLUDE_DIRS})
# else()
#     message(WARNING "Eigen3 not found. You might need to install it or set EIGEN3_INCLUDE_DIR.")
# endif()

# # Find jsoncpp library (if installed)
# find_package(PkgConfig QUIET)

# if (PkgConfig_FOUND)
#     pkg_check_modules(JSONCPP jsoncpp)
#     if (JSONCPP_FOUND)
#         message(STATUS "Found jsoncpp: ${JSONCPP_INCLUDE_DIRS}")
#         include_directories(${JSONCPP_INCLUDE_DIRS})
#         link_directories(${JSONCPP_LIBRARY_DIRS})
#     else()
#         message(WARNING "jsoncpp not found. You might need to install it or set JSONCPP_INCLUDE_DIRS and JSONCPP_LIBRARY_DIRS.")
#     endif()
# else()
#     message(WARNING "PkgConfig not found. You might need to install it to automatically find jsoncpp.")
# endif()



# find_path(Jsoncpp_INCLUDE_DIR
#   NAMES json/features.h
#   PATH_SUFFIXES jsoncpp
#   PATHS ${Jsoncpp_PKGCONF_INCLUDE_DIRS}
# )

SET(SOURCE_FILES
    gesture_pptx.cpp
    # src/load_data/folder_loader.cpp
    # src/feature_extraction/stage.hpp
    # src/feature_extraction/pipeline.hpp
    # src/data_classes.hpp
    ../src/mediapipe_client/mediapipe_client.hpp
    ../src/mediapipe_client/mediapipe_client.cpp
    ../src/data_classes.hpp
)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    # ${JSONCPP_INCLUDE_DIRS} 
    ../src/mediapipe_client
    # src/feature_extraction
    # src/load_data)
)
    

add_executable(CrossPlatformPowerPoint ${SOURCE_FILES})

# target_link_libraries(CrossPlatformPowerPoint PRIVATE ${OpenCV_LIBS} ${JSONCPP_LIBRARIES})

target_link_libraries(CrossPlatformPowerPoint PRIVATE ${OpenCV_LIBS})

# macOS-specific code
find_library(APPKIT_LIBRARY AppKit)
target_link_libraries(CrossPlatformPowerPoint PRIVATE ${APPKIT_LIBRARY})
