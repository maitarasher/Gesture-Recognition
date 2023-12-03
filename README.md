# Gesture-Recognition
Gesture-Recognition Libary in C++

COMS W4995: Design Using C++

Fall 2023

Yana Botvinnik, Maitar Asher, Noam Zaid, Elvina Wibisono, Elifia Muthia

# About
This is a Gesture Recognition library written for C++ that allows for easy gesture recognitions in images and live videostreams. As of December 2023, this library is **only tested and compatible with MacOS**. Intended to be run alongside this [repository](https://github.com/elifia-muthia/mediapipe) containing Google's Mediapipe that has been configured to MacOS as a default and modified to function as a server to obtain hand landmarks – a vector of detected/tracked hands in an image represented as a list of 21 3D (x,y,z) coordinates.

This project takes inspiration from a similar gesture recognition library in Python, the [GRLib](https://github.com/mikhail-vlasenko/grlib/tree/master), that's algorithm has been nicely documented [here](https://arxiv.org/pdf/2310.14919v1.pdf).

## OpenCV Setup
1. Install Homebrew
   
```/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"```

2. Install OpenCV
   
```brew install opencv```

3. Go to the project on VSCode and execute the following commands in the root

```mkdir build```

```cd build```

```cmake ..```

```make```

```./gesture-project```

To locally get the submodules the first time:

```git submodule update --init```

For any time after that use the command:

```git submodule update --recursive --remote```

## Running Mediapipe
1. Install bazel https://bazel.build/install/bazelisk

2. In Gesture-Recognition/src/dependencies/mediapipe, run the following command to build

```bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/mediapipe_samples/mediapipe_sample:mediapipe_sample```

3. To run mediapipe, execute the following command

```GLOG_logtostderr=1 bazel-bin/mediapipe/mediapipe_samples/mediapipe_sample/mediapipe_sample --calculator_graph_config_file=mediapipe/graphs/hand_tracking/hand_tracking_desktop_live.pbtxt /Users/elifiamuthia/Desktop/test-image.jpg```

**replace the last command line argument with your test image
