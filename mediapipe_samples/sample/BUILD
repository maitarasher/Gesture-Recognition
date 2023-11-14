# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

licenses(["notice"])

package(default_visibility = [
    "//visibility:public",
])

cc_library(
    name = "mediapipe_sample",
    srcs = ["mediapipe_sample.cc"],
    deps = [
        "//src/dependencies/mediapipe/mediapipe/calculators/util:landmarks_to_render_data_calculator",
        "//src/dependencies/mediapipe/mediapipe/framework/formats:landmark_cc_proto",
        "//src/dependencies/mediapipe/mediapipe/framework:calculator_framework",
        "//src/dependencies/mediapipe/mediapipe/framework/formats:image_frame",
        "//src/dependencies/mediapipe/mediapipe/framework/formats:image_frame_opencv",
        "//src/dependencies/mediapipe/mediapipe/framework/port:file_helpers",
        "//src/dependencies/mediapipe/mediapipe/framework/port:opencv_highgui",
        "//src/dependencies/mediapipe/mediapipe/framework/port:opencv_imgproc",
        "//src/dependencies/mediapipe/mediapipe/framework/port:opencv_video",
        "//src/dependencies/mediapipe/mediapipe/framework/port:parse_text_proto",
        "//src/dependencies/mediapipe/mediapipe/framework/port:status",
        "//src/dependencies/mediapipe/mediapipe/framework/port:commandlineflags",
        "//src/dependencies/mediapipe/mediapipe/util:resource_util",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log:absl_log",
    ],
)

