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

#include "../src/mediapipe_client/mediapipe_client.hpp"
#include "../src/data_classes.hpp"

#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::__fs::filesystem;

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1"; 

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

    // get the powerpoint file 
    const char* relativePath = "gesture_pptx/test.pptx";
    std::string fullPath = getFullPathFromRelativePath(relativePath);
    std::cout << "Full Path: " << fullPath << std::endl;
    const char* filePath = fullPath.c_str();

    // Open the existing PowerPoint presentation
    openExistingPresentation(filePath);

    // model 

    // Establish connection with server
    int clientSocket = connectToServer(PORT, SERVER_IP);
    if (clientSocket == -1) {
        return -1;
    }


    // Camera capture and gesture recognition
    // cv::VideoCapture cap(0);  // Open the default camera (usually the built-in webcam)

    // if (!cap.isOpened()) {
    //     std::cerr << "Error: Could not open camera." << std::endl;
    //     return -1;
    // }

    cv::namedWindow("Camera Feed", cv::WINDOW_AUTOSIZE);

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

    // cap.release();
    cv::destroyAllWindows();



   


    return 0;
}