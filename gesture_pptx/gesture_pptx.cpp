// #include <iostream>
// #include <cstring>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <opencv2/opencv.hpp>
// #include <arpa/inet.h>
// #include <regex>
// #include "../src/feature_extraction/pipeline.hpp"
// #include "../src/feature_extraction/stage.hpp"
// #include "../src/load_data/folder_loader.hpp"

#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <regex>

#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::__fs::filesystem;

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


//  PPTX CODE 
// handle gesture 
void handleGesture(const char* gesture){

    std::cout << "Gesture Recognized: " << gesture << std::endl;

    if (strcmp(gesture, "NextSlide") == 0){
        system("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'go to next slide of slide show window 1' -e 'end tell'");
        std::cout << "Moving to the next slide!" << std::endl;
    }
    else if(strcmp(gesture, "PrevSlide") == 0){
        system("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'go to previous slide of slide show window 1' -e 'end tell'");
        std::cout << "Moving to the previous slide!" << std::endl;
    }
    else if(strcmp(gesture, "StartSlide") == 0){
        system("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'start slide show of slide show window 1' -e 'end tell'");
        std::cout << "Starting slideshow!" << std::endl;
    }
    else if(strcmp(gesture, "EndSlide") == 0){
        system("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'end slide show of slide show window 1' -e 'end tell'");
        std::cout << "Ending slideshow!" << std::endl;
    }

}

// open an existing Presentation 
void openExistingPresentation(const char* filePath){

    char script[256];
    snprintf(script, sizeof(script), "osascript -e 'tell application \"Microsoft PowerPoint\" to open \"%s\"'", filePath);
    system(script);

}

void openNewPresentation(){
     //Open a new powerpoint file 
    FILE* pipe = popen("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'activate' -e 'set myPresentation to make new presentation' -e 'set mySlide to make new slide at end of slides of myPresentation' -e 'set title of mySlide to \"Hello, PowerPoint from C++ on macOS!\"' -e 'end tell' 2>&1", "r");

    if (pipe) {
        char buffer[128];
        std::string result;

        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }

        int status = pclose(pipe);

        std::cout << "Command returned: " << status << std::endl;
        std::cout << "Output and error message: " << result << std::endl;
    }

}

std::string getFullPathFromRelativePath(const char* relativePath) {
    // Get the current working directory
    fs::path currentPath = fs::current_path();
    currentPath = currentPath.parent_path().parent_path();
    fs::path fullPath = currentPath / relativePath;

    return fullPath.string();
}


int main() {

    std::cout << "Hello, MacOS PowerPoint Application!" << std::endl;

    const char* relativePath = "gesture_pptx/test.pptx";
    std::string fullPath = getFullPathFromRelativePath(relativePath);
    std::cout << "Full Path: " << fullPath << std::endl;
    const char* filePath = fullPath.c_str();

    // Open the existing PowerPoint presentation
    openExistingPresentation(filePath);

    // model 

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr));

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting to the server" << std::endl;
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;


    // Camera capture and gesture recognition
    cv::VideoCapture cap(0);  // Open the default camera (usually the built-in webcam)

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    cv::namedWindow("Camera Feed", cv::WINDOW_AUTOSIZE);

    while (true) {
        // cv::Mat frame;
        // cap >> frame;  // Capture frame from the camera
        cv::Mat inputImage; 
        cap >> inputImage;

        // Encode the image to JPEG format
        std::vector<uchar> imageData;
        cv::imencode(".jpg", inputImage, imageData);

        size_t imageSize = imageData.size();
        ssize_t sizeSent = send(clientSocket, &imageSize, sizeof(size_t), 0);

        if (sizeSent != sizeof(size_t)) {
            std::cerr << "Error sending image size. Sent: " << sizeSent << " bytes" << std::endl;
            close(clientSocket);
            return -1;
        }

        // Send the image data
        ssize_t bytesSent = send(clientSocket, imageData.data(), imageSize, 0);

        if (bytesSent < 0) {
            std::cerr << "Error sending image data" << std::endl;
            close(clientSocket);
            return -1;
        }

        std::cout << "Image sent!" << std::endl;

        // Receive the number of landmarks
        size_t numOfLandmarks;
        ssize_t sizeReceived = recv(clientSocket, &numOfLandmarks, sizeof(size_t), 0);

        if (sizeReceived != sizeof(size_t)) {
            std::cerr << "Error receiving image size. Received: " << sizeReceived<< std::endl;
            close(clientSocket);
            return -1;
        }

        ssize_t bytesRead = 0;
        const size_t bufferSize = 4056;
        std::vector<char> landmarkBuffer(bufferSize);
        std::vector<std::vector<std::vector<Landmark>>> landmarks;
        // receive landmarks
        for (int i = 0; i < numOfLandmarks; ++i) {        
            bytesRead = recv(clientSocket, landmarkBuffer.data(), bufferSize, 0);
            if (bytesRead < 0) {
                std::cerr << "Error receiving landmark data" << std::endl;
                close(clientSocket);
                return -1;
            }

            std::string landmarkString(landmarkBuffer.data(), bytesRead);

            std::vector<std::vector<Landmark>> landmarks_parsed = parseLandmarks(landmarkString);

            landmarks.emplace_back(landmarks_parsed);

            // Clear the buffer for the next iteration
            // landmarkBuffer.clear();
            landmarkBuffer.resize(bufferSize);
        }

        // // Your gesture recognition logic here
        // // For simplicity, let's assume a basic gesture based on the color of a detected object
        // cv::Scalar targetColor = cv::Scalar(0, 255, 0);  // Green color as an example
        // cv::Rect regionOfInterest(100, 100, 50, 50);  // Example region for gesture detection
        // cv::Mat roi = frame(regionOfInterest);

        // // Check if the average color in the region is close to the target color
        // double avgColorDiff = cv::norm(cv::mean(roi) - targetColor);

        // if (avgColorDiff < 30.0) {
        //     // Detected gesture (example: green object detected)
        //     handleGesture("NextSlide");
        // }

        // cv::imshow("Camera Feed", frame);
        cv::imshow("Camera Feed", inputImage);

        char c = cv::waitKey(1);
        if (c == 27)  
            break;
    }

    cap.release();
    cv::destroyAllWindows();



   


    return 0;
}