#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <regex>
#include "../src/feature_extraction/pipeline.hpp"
#include "../src/feature_extraction/stage.hpp"
#include "../src/load_data/folder_loader.hpp"
#include "../src/data_classes.hpp"
#include "../src/mediapipe_client.hpp"

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";

int main() {

    // (1) create a client socket and connect to the MediaPipe Server

    ////////////// uncomment below code ///////

    int clientSocket = connectToServer(PORT, SERVER_IP);

    if (clientSocket == -1) {
        return -1;
    }

    ////////////// uncomment above code ////////

    // (2) Create Pipeline structure

    // Create a pipeline with 2 hands
    Pipeline my_pipeline(2);

    // Add stages to the pipeline (brightness, rotation)
    my_pipeline.add_stage(15.0, 315.0);
    my_pipeline.add_stage(15.0, 0.0);
    my_pipeline.add_stage(15.0, 45.0);
    my_pipeline.add_stage(15.0, 90.0);

    // (3) Load images by folder - call folder_loader, ImageData contians the image_path and the classlabel
    std::string folderPath = "/Users/elifiamuthia/Desktop/gesture_data/subsample";
    std::vector<ImageData> images = loadImgsFromFolder(folderPath);
    std::cout << "Finished loading images\n";

    std::vector<std::vector<Hand_Landmarks>> all_images_landmarks;
    // Iterate over all images
    for (const ImageData& imageData : images)
    {
      cv::Mat image = cv::imread(imageData.filePath);
      // Check if the image was loaded
      if (!image.empty())
      {
        // (4) Use Pipeline for each image - augment the image
        for (const Stage &stage : my_pipeline.getStages()){
            cv::Mat inputImage = stage.process(image);

            // Show the original and processed images for each stage
            // std::cout << "Class Label: " << imageData.classLabel << std::endl;
            // cv::imshow("Original Image", image);
            // cv::imshow("Brightened Image", processedImage);
            // cv::waitKey(1000);

            // (a) Get landmarks for each augmented image in the Pipeline by sending a the image to the server and getting a reposnd

            // (b) The first landmark corresponding to at least one hand being detected is selected

            // (c) Optional apply False Positive filter

            // (d) Add landmark to the Data Structure along with the label

        }
      }
      else
      {
          std::cerr << "Error loading image: " << imageData.filePath << std::endl;
      }
    }

    // (5) Split the Data into train and test

    // (6) Train the data using K classiffier

    // (7) Get results

    // (8) Use the model for parsing the gestures in a recorded video stream - retrurn script of words


    ///// SEND VIDEO TO MEDIAPIPE, GET LANDMARKS BACK, GET ACTION FROM CLASSIFIER, DO ACTION
    cv::VideoCapture capture;
    capture.open(0);
    bool grab_frames = true;
    while (grab_frames) {
        cv::Mat inputImage;
        capture >> inputImage;

        std::vector<Hand_Landmarks> landmarks;
        bool success = getLandmarksFromServer(clientSocket, inputImage, landmarks);
        if (success == false) {
            return -1;
        }

        // std::cout << landmarks.size() << std::endl;


        // TODO
        // RUN LANDMARKS THROUGH CLASSIFIER AND GET CORRESPONDING ACTION    
    }
    close(clientSocket);

    return 0;
}
