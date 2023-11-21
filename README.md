# Gesture-Recognition
Gesture-Recognition Libary in C++

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

To locally get the submodules:

```git submodule update --init```

## Running Mediapipe
1. Install bazel https://bazel.build/install/bazelisk

2. In Gesture-Recognition/src/dependencies/mediapipe, run the following command to build

```bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/mediapipe_samples/mediapipe_sample:mediapipe_sample```

3. To run mediapipe, execute the following command

```GLOG_logtostderr=1 bazel-bin/mediapipe/mediapipe_samples/mediapipe_sample/mediapipe_sample --calculator_graph_config_file=mediapipe/graphs/hand_tracking/hand_tracking_desktop_live.pbtxt /Users/elifiamuthia/Desktop/test-image.jpg```

**replace the last command line argument with your test image
