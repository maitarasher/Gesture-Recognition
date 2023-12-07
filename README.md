# Gesture-Recognition in C++

This library is a collaborative effort developed as the final group project for the COMS W4995: Design Using C++ course in Fall 2023, taught by Bjarne Stroustrup.


Authors: Yana Botvinnik, Maitar Asher, Noam Zaid, Elvina Wibisono, Elifia Muthia
## About
This is a Gesture Recognition library written for C++ that allows users to effortlessly create models capable of recognizing gestures in images, live video streams, or recordings.
Note: As of December 2023, **this library is exclusively tested and compatible with MacOS**. Our library is designed to run alongside Google's MediaPipe Libraries. We've set up a server that leverages Google’s MediaPipe and has been configured to work for MacOS as a default. The server plays a vital role in obtaining hand landmarks, a crucial step in our program that simplifies the classification problem. Instead of dealing with numerous pixels for each image, our approach involves working with 21 landmarks for each image.

## Acknowledgment

This project takes inspiration from a similar gesture recognition library in Python, the [GRLib](https://github.com/mikhail-vlasenko/grlib/tree/master), that's algorithm has been nicely documented [here](https://arxiv.org/pdf/2310.14919v1.pdf).

## Additional Resources

- [Gesture Recognition Tutorial](https://docs.google.com/document/d/1tlUAKTq5tPwRVC3Sa62UZCwkMtCfloZU4QGTxBQKAxg/edit?usp=sharing): Explore the step-by-step tutorial on setting up the environment, processing data, training models, and making predictions.

- [Gesture Recognition Manual](https://docs.google.com/document/d/1wTPNB_bm-LVUp-C2Qwciy0hg1adNCyv1YlQNXkRBiFI/edit?usp=sharing): For more detailed information about each step, configuration options, and advanced features, refer to our comprehensive manual.

- [Design Documentation](https://docs.google.com/document/d/1OGW2GpuXKYR4RbnvIpiwjtReCL4Vc0j5KpZ1pBwtPT0/edit?usp=sharing): Read our design documentation for an in-depth understanding of the architecture, system components, and implementation details.

## Set Up the Environment

1. Install Homebrew (a package manager to install library dependencies)

```/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"```

2. Install OpenCV

```brew install opencv```

3. Install [bazelisk](https://bazel.build/install/bazelisk)

```brew install bazelisk```

4. Install opencv@3

```brew install opencv@3```

5. Install ffmpeg  

```brew install ffmpeg```

6. Install Numpy

```brew install numpy```

## Set Up the MediaPipe Server

1. Clone the modified Mediapipe [repository](https://github.com/elifia-muthia/mediapipe)

2. Move into the newly cloned Mediapipe repository

```cd mediapipe```

3. Build the file

```bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/mediapipe_samples/mediapipe_sample:mediapipe_sample```

4. Execute the following command to run the server

```GLOG_logtostderr=1 bazel-bin/mediapipe/mediapipe_samples/mediapipe_sample/mediapipe_sample --calculator_graph_config_file=mediapipe/graphs/hand_tracking/hand_tracking_desktop_live.pbtxt```

## Clone this Gesture-Recognition repository

## Process Training Data

1. Optional: Customize your augmentation pipeline

You may add/remove stages into your pipeline. The code is located at: ```Gesture-Recognition/processing_data/processing.cpp```

2. Navigate to Gesture-Recognition root directory

3. Build the processing application

```cd processing_data```

```mkdir build```

```cd build```

```cmake ..```

```make```

4. Compile the processing application to generate landmarks representation of your data

```./processing <training_images_dir_path> <output_folder>```

## Running the ASL Application Example

1. Navigate to Gesture-Recognition root directory

2. Build the application

``` cd gesture_asl ```

```mkdir build```

```cd build```

```cmake ..```

```make```

3. Run the Application

```./asl_application ../../data/asl```

## Running the Powerpoint Controller Example

1. Run the commands below for the dependencies

```brew install jsoncpp ```

```brew install pkg-config```

2. Navigate to Gesture-Recognition root directory

2. Run the script to prepare the data

``` cd processing_data/processing_cocodataset ```

```mkdir build```

```cd build```

```cmake ..```

```make```

```./coco_dataset_export <coco_folder_path> <output_folder>```

3. Navigate to Gesture-Recognition root directory

4. Build the application

```cd gesture_pptx```

```mkdir build```

```cd build```

```cmake ..```

```make```

```.\CrossPlatformPowerPoint ???```
