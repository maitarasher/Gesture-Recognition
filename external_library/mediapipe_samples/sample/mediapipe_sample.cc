// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// An example of sending OpenCV webcam frames into a MediaPipe graph.
#include <cstdlib>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/absl_log.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/util/resource_util.h"

#include "mediapipe/calculators/util/landmarks_to_render_data_calculator.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"

constexpr char kInputStream[] = "input_video";
constexpr char kOutputStream[] = "output_video";
constexpr char kLandmarksStream[] = "landmarks";
constexpr char kWindowName[] = "MediaPipe";

ABSL_FLAG(std::string, calculator_graph_config_file, "",
          "Name of file containing text format CalculatorGraphConfig proto.");
ABSL_FLAG(std::string, input_video_path, "",
          "Full path of video to load. "
          "If not provided, attempt to use a webcam.");
ABSL_FLAG(std::string, output_video_path, "",
          "Full path of where to save result (.mp4 only). "
          "If not provided, show result in a window.");

//////////////////////

absl::Status RunMPPGraph(cv::Mat& inputImage, cv::Mat& outputImage, std::vector<::mediapipe::NormalizedLandmarkList>& multiHandLandmarks) {
  std::string calculator_graph_config_contents;
  MP_RETURN_IF_ERROR(mediapipe::file::GetContents(
      absl::GetFlag(FLAGS_calculator_graph_config_file),
      &calculator_graph_config_contents));
  ABSL_LOG(INFO) << "Get calculator graph config contents: "
                 << calculator_graph_config_contents;
  mediapipe::CalculatorGraphConfig config =
      mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(
          calculator_graph_config_contents);

  ABSL_LOG(INFO) << "Initialize the calculator graph.";
  mediapipe::CalculatorGraph graph;
  MP_RETURN_IF_ERROR(graph.Initialize(config));

  // ABSL_LOG(INFO) << "Initialize the camera or load the video.";
  // cv::VideoCapture capture;
  // const bool load_video = !absl::GetFlag(FLAGS_input_video_path).empty();
  // if (load_video) {
  //   capture.open(absl::GetFlag(FLAGS_input_video_path));
  // } else {
  //   capture.open(0);
  // }
  // RET_CHECK(capture.isOpened());

  // cv::VideoWriter writer;
  // const bool save_video = !absl::GetFlag(FLAGS_output_video_path).empty();
  // if (save_video) {
  // ABSL_LOG(INFO) << "Prepare video writer.";
  //     cv::Mat test_frame;
  //     capture.read(test_frame);                    // Consume first frame.
  //     capture.set(cv::CAP_PROP_POS_AVI_RATIO, 0);  // Rewind to beginning.
  //     writer.open(absl::GetFlag(FLAGS_output_video_path),
  // mediapipe::fourcc('a', 'v', 'c', '1'),  // .mp4
  //                 capture.get(cv::CAP_PROP_FPS), test_frame.size());
  // RET_CHECK(writer.isOpened());
  //   } else {

  // cv::namedWindow(kWindowName, /*flags=WINDOW_AUTOSIZE*/ 1);
    // }

//   cv::VideoWriter writer;
//   const bool save_video = !absl::GetFlag(FLAGS_output_video_path).empty();
//   if (!save_video) {
//     cv::namedWindow(kWindowName, /*flags=WINDOW_AUTOSIZE*/ 1);
// #if (CV_MAJOR_VERSION >= 3) && (CV_MINOR_VERSION >= 2)
//     capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
//     capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
//     capture.set(cv::CAP_PROP_FPS, 30);
// #endif
//   }

  
  MP_ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller,
                      graph.AddOutputStreamPoller(kOutputStream));
    // hand landmarks stream
  MP_ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller_landmark,
              graph.AddOutputStreamPoller(kLandmarksStream));

  MP_ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller presence_poller,
                   graph.AddOutputStreamPoller("landmark_presence"));

  ABSL_LOG(INFO) << "Start running the calculator graph.";
  MP_RETURN_IF_ERROR(graph.StartRun({}));

  ABSL_LOG(INFO) << "Start grabbing and processing frames.";
  size_t frame_timestamp = 0;
  // bool grab_frames = true;
  // while (grab_frames) {
    // Capture opencv camera or video frame.
    // cv::Mat camera_frame_raw;
    // capture >> camera_frame_raw;

    // if (camera_frame_raw.empty()) {
    //   // if (!load_video) {
    //   //   ABSL_LOG(INFO) << "Ignore empty frames from camera.";
    //   //   continue;
    //   // }
    //   // ABSL_LOG(INFO) << "Empty frame, end of video reached.";
    //   // break;
    //   break;
     
    // }
    cv::Mat inputImgProcessed;
    cv::cvtColor(inputImage, inputImgProcessed, cv::COLOR_BGR2RGB);
    // if (!load_video) {
    //   cv::flip(inputImgProcessed, inputImgProcessed, /*flipcode=HORIZONTAL*/ 1);
    // }

    // Wrap Mat into an ImageFrame.
    auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
        mediapipe::ImageFormat::SRGB, inputImgProcessed.cols, inputImgProcessed.rows,
        mediapipe::ImageFrame::kDefaultAlignmentBoundary);
    cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
    inputImgProcessed.copyTo(input_frame_mat);

    // Send image packet into the graph.
    // size_t frame_timestamp_us =
    //     (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;
    // MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
    //     kInputStream, mediapipe::Adopt(input_frame.release())
    //                       .At(mediapipe::Timestamp(frame_timestamp_us))));

    MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
    kInputStream, mediapipe::Adopt(input_frame.release())
                              .At(mediapipe::Timestamp(frame_timestamp++))));

    // Get the graph result packet, or stop if that fails.
    mediapipe::Packet packet;
    mediapipe::Packet landmark_packet; 
    mediapipe::Packet presence_packet;

    if (!poller.Next(&packet)) return absl::Status(absl::StatusCode::kInvalidArgument, "No Output Packet");;

    if (!presence_poller.Next(&presence_packet)) return absl::Status(absl::StatusCode::kInvalidArgument, "No Presence Packet");;
    
    auto is_landmark_present = presence_packet.Get<bool>();

    if (is_landmark_present) {
      if (!poller_landmark.Next(&landmark_packet)) return absl::Status(absl::StatusCode::kInvalidArgument, "No Landmark Packet");;
      multiHandLandmarks = landmark_packet.Get<std::vector<::mediapipe::NormalizedLandmarkList>>();
    }

    // if (!poller_landmark.Next(&landmark_packet)) break;

    auto& output_frame = packet.Get<mediapipe::ImageFrame>();
    // auto& output_landmarks = landmark_packet.Get<std::vector<::mediapipe::NormalizedLandmarkList>>();

    // Convert back to opencv for display or saving.
    outputImage = mediapipe::formats::MatView(&output_frame);
    cv::cvtColor(outputImage, outputImage, cv::COLOR_RGB2BGR);

   
    // if (save_video) {
    //   // if (!writer.isOpened()) {
    //   //   ABSL_LOG(INFO) << "Prepare video writer.";
    //   //   writer.open(absl::GetFlag(FLAGS_output_video_path),
    //   //               mediapipe::fourcc('a', 'v', 'c', '1'),  // .mp4
    //   //               capture.get(cv::CAP_PROP_FPS), output_frame_mat.size());
    //   //   RET_CHECK(writer.isOpened());
    //   // }
    //   writer.write(output_frame_mat);
    // } else {
    //   cv::imshow(kWindowName, output_frame_mat);
    //   // Press any key to exit.
    //   const int pressed_key = cv::waitKey(5);
    //   if (pressed_key >= 0 && pressed_key != 255) grab_frames = false;
    // }

    
  // }

  // ABSL_LOG(INFO) << "Shutting down.";
  // if (writer.isOpened()) writer.release();
  MP_RETURN_IF_ERROR(graph.CloseInputStream(kInputStream));
  return graph.WaitUntilDone();
}

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  absl::ParseCommandLine(argc, argv);

  // Open image
  std::cout << argv[2] << std::endl;
  cv::Mat inputImage = cv::imread(argv[2]);

  if (inputImage.empty()) {
        std::cerr << "Error: Could not open or find the image." << std::endl;
        return -1;
  }

  cv::Mat outputImage;
  std::vector<::mediapipe::NormalizedLandmarkList> multiHandLandmarks = std::vector<::mediapipe::NormalizedLandmarkList>();
  absl::Status run_status = RunMPPGraph(inputImage, outputImage, multiHandLandmarks);

  cv::imwrite("/Users/elifiamuthia/Desktop/output_image.jpg", outputImage);

  for (auto& landmark: multiHandLandmarks) {
        std::cout << landmark.DebugString();
  }

  // if (!run_status.ok()) {
  //   ABSL_LOG(ERROR) << "Failed to run the graph: " << run_status.message();
  //   return EXIT_FAILURE;
  // } else {
  //   ABSL_LOG(INFO) << "Success!";
  // }
  return EXIT_SUCCESS;
}