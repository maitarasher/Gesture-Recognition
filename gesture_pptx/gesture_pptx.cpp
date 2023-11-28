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

#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::__fs::filesystem;
               

// handle gesture 
void handleGesture(const char* gesture){

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


    // Camera capture and gesture recognition
    cv::VideoCapture cap(0);  // Open the default camera (usually the built-in webcam)

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    cv::namedWindow("Camera Feed", cv::WINDOW_AUTOSIZE);

    while (true) {
        cv::Mat frame;
        cap >> frame;  // Capture frame from the camera

        // // Your gesture recognition logic here
        // // For simplicity, let's assume a basic gesture based on the color of a detected object
        // cv::Scalar targetColor = cv::Scalar(0, 255, 0);  // Green color as an example
        // cv::Rect regionOfInterest(100, 100, 50, 50);  // Example region for gesture detection
        // cv::Mat roi = frame(regionOfInterest);

        // // Check if the average color in the region is close to the target color
        // double avgColorDiff = cv::norm(cv::mean(roi) - targetColor);

        // if (avgColorDiff < 30.0) {
        //     // Detected gesture (example: green object detected)
        //     performPowerPointAction("NextSlide");
        // }

        cv::imshow("Camera Feed", frame);

        char c = cv::waitKey(1);
        if (c == 27)  
            break;
    }

    cap.release();
    cv::destroyAllWindows();



   


    return 0;
}