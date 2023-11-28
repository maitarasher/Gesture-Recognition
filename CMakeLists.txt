
SET(PROJECT_NAME Gesture-Recognition)
PROJECT($(PROJECT_NAME))

CMAKE_MINIMUM_REQUIRED (VERSION 3.12)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_BUILD_TYPE Release)

FIND_PACKAGE(OpenCV REQUIRED)

SET(SOURCE_FILES
    gesture_asl/asl_application.cpp
    src/load_data/folder_loader.cpp
    src/feature_extraction/stage.hpp
    src/feature_extraction/pipeline.hpp
)

INCLUDE_DIRECTORIES(
    ${OpenCV_INCLUDE_DIRS}
    src/feature_extraction
    src/load_data
)


ADD_EXECUTABLE(gesture_project ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(gesture_project ${OpenCV_LIBS})


