CMAKE_MINIMUM_REQUIRED (VERSION 3.12)
SET(PROJECT_NAME Gesture-Recognition)
PROJECT($(PROJECT_NAME))

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_BUILD_TYPE Release)

FIND_PACKAGE(OpenCV REQUIRED)

SET(SOURCE_FILES
    processing.cpp
    ../src/load_data/folder_loader.cpp
    ../src/feature_extraction/stage.hpp
    ../src/feature_extraction/pipeline.hpp
    ../src/data_classes.hpp
    ../src/mediapipe_client/mediapipe_client.hpp
    ../src/mediapipe_client/mediapipe_client.cpp
    ../src/classifier/knn.hpp
)

INCLUDE_DIRECTORIES(
    ${OpenCV_INCLUDE_DIRS}
    ../src/feature_extraction
    ../src/load_data
    ../src/mediapipe_client
    ../src/classifier
)


ADD_EXECUTABLE(processing ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(processing ${OpenCV_LIBS})


