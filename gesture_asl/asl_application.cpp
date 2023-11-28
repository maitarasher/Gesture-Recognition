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

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";

class Landmark {
public:
    double x, y, z;

    Landmark(double x, double y, double z) : x(x), y(y), z(z) {}
};

std::vector<std::vector<Landmark>> parseLandmarks(const std::string& input) {
    std::regex landmarkRegex(R"regex(landmark \{\s*x: ([\d.-]+)\s*y: ([\d.-]+)\s*z: ([\d.-]+)\s*\})regex");
    std::smatch match;
    std::vector<std::vector<Landmark>> landmarks;

    auto it = input.cbegin();
    while (std::regex_search(it, input.cend(), match, landmarkRegex)) {
        std::vector<Landmark> landmarkGroup;

        double x = std::stod(match[1].str());
        double y = std::stod(match[2].str());
        double z = std::stod(match[3].str());
        landmarkGroup.emplace_back(x, y, z);

        landmarks.push_back(landmarkGroup);
        it = match.suffix().first;
    }

    return landmarks;
}


int main() {

    // (1) create a client socket and connect to the MediaPipe Server

    ////////////// uncomment below code ///////

    // int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    // if (clientSocket < 0) {
    //     std::cerr << "Error creating socket" << std::endl;
    //     return -1;
    // }
    //
    // sockaddr_in serverAddress;
    // memset(&serverAddress, 0, sizeof(serverAddress));
    // serverAddress.sin_family = AF_INET;
    // serverAddress.sin_port = htons(PORT);
    // inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr));
    //
    // if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
    //     std::cerr << "Error connecting to the server" << std::endl;
    //     close(clientSocket);
    //     return -1;
    // }
    //
    // std::cout << "Connected to server!" << std::endl;

    ////////////// uncomment above code ////////

    // (2) Create Pipeline structure

    // Create a pipeline with 2 hands
    Pipeline my_pipeline(2);

    // Add stages to the pipeline (brightness, rotation)
    my_pipeline.add_stage(30.0, 45.0);
    my_pipeline.add_stage(15.0, 90.0);

    // (3) Load images by folder - call folder_loader, ImageData contians the image_path and the classlabel
    std::string folderPath = "/Users/maitarasher/Desktop/gesture_data/subsample";
    std::vector<ImageData> images = loadImgsFromFolder(folderPath);
    std::cout << "Finished loading images\n";


    // Iterate over all images
    for (const ImageData& imageData : images)
    {
      cv::Mat image = cv::imread(imageData.filePath);
      // Check if the image was loaded
      if (!image.empty())
      {
        // (4) Use Pipeline for each image - augment the image
        for (const Stage &stage : my_pipeline.getStages()){
            cv::Mat processedImage = stage.process(image);

            // Show the original and processed images for each stage
            std::cout << "Class Label: " << imageData.classLabel << std::endl;
            cv::imshow("Original Image", image);
            cv::imshow("Brightened Image", processedImage);
            cv::waitKey(1000);

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

    ////////////// change below code to accommodate the above ////////

    // Load an example image (replace with your image loading code)
    // cv::Mat inputImage = cv::imread("/Users/elifiamuthia/Desktop/test-image.jpg");
    // if (inputImage.empty()) {
    //     std::cerr << "Error loading image" << std::endl;
    //     close(clientSocket);
    //     return -1;
    // }

    // // Encode the image to JPEG format
    // std::vector<uchar> imageData;
    // cv::imencode(".jpg", inputImage, imageData);
    //
    // size_t imageSize = imageData.size();
    // ssize_t sizeSent = send(clientSocket, &imageSize, sizeof(size_t), 0);
    //
    // if (sizeSent != sizeof(size_t)) {
    //     std::cerr << "Error sending image size" << std::endl;
    //     close(clientSocket);
    //     return -1;
    // }
    //
    // // Send the image data
    // ssize_t bytesSent = send(clientSocket, imageData.data(), imageSize, 0);
    //
    // if (bytesSent < 0) {
    //     std::cerr << "Error sending image data" << std::endl;
    //     close(clientSocket);
    //     return -1;
    // }
    //
    // std::cout << "Image sent!" << std::endl;
    //
    // // Receive the number of landmarks
    // size_t numOfLandmarks;
    // ssize_t sizeReceived = recv(clientSocket, &numOfLandmarks, sizeof(size_t), 0);
    //
    // if (sizeReceived != sizeof(size_t)) {
    //     std::cerr << "Error receiving image size" << std::endl;
    //     close(clientSocket);
    //     return -1;
    // }
    //
    // ssize_t bytesRead = 0;
    // const size_t bufferSize = 4056;
    // std::vector<char> landmarkBuffer(bufferSize);
    // std::vector<std::vector<std::vector<Landmark>>> landmarks;
    // // receive landmarks
    // for (int i = 0; i < numOfLandmarks; ++i) {
    //     bytesRead = recv(clientSocket, landmarkBuffer.data(), bufferSize, 0);
    //     if (bytesRead < 0) {
    //         std::cerr << "Error receiving landmark data" << std::endl;
    //         close(clientSocket);
    //         return -1;
    //     }
    //
    //     std::string landmarkString(landmarkBuffer.data(), bytesRead);
    //
    //     std::vector<std::vector<Landmark>> landmarks_parsed = parseLandmarks(landmarkString);
    //
    //     landmarks.emplace_back(landmarks_parsed);
    //
    //     // Clear the buffer for the next iteration
    //     landmarkBuffer.clear();
    //     landmarkBuffer.resize(bufferSize);
    // }
    //
    //
    // // how to access landmarks
    // for (const auto& group : landmarks) {
    //     for (const auto& landmark : group) {
    //         for (const auto& coords : landmark) {
    //             std::cout << "x: " << coords.x << " y: " << coords.y << " z: " << coords.z << '\n';
    //         }
    //         std::cout << "----\n"; // Separate each vector
    //     }
    //     std::cout << "====\n"; // Separate each group
    // }
    //
    // close(clientSocket);

    return 0;
}
