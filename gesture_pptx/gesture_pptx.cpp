#include "../src/mediapipe_client/mediapipe_client.hpp"
#include "../src/data_classes.hpp"
#include "../src/load_data/save_and_read.cpp"
#include "../src/classifier/knn.hpp"

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
        system("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'go to next slide slide show view of slide show window 1' -e 'end tell'");
        std::cout << "Moving to the next slide!" << std::endl;
    }
    else if(strcmp(gesture, "PrevSlide") == 0){
        system("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'go to previous slide slide show view of slide show window 1' -e 'end tell'");
        std::cout << "Moving to the previous slide!" << std::endl;
    }
    else if(strcmp(gesture, "StartSlide") == 0){
        system("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'activate' -e 'run slide show slide show settings of active presentation' -e 'end tell'");
        std::cout << "Starting slideshow!" << std::endl;
    }
    else if(strcmp(gesture, "EndSlide") == 0){
        system("osascript -e 'tell application \"Microsoft PowerPoint\"' -e 'exit slide show slide show view of slide show window of active presentation' -e 'end tell'");
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


int main(int argc, char* argv[]) {

    std::cout << "Hello, MacOS PowerPoint Application!" << std::endl;

    const char* relativePath = "gesture_pptx/test.pptx";
    std::string fullPath = getFullPathFromRelativePath(relativePath);
    std::cout << "Full Path: " << fullPath << std::endl;
    const char* filePath = fullPath.c_str();

    /** TRAINING **/

    // prepare data
    std::vector<Hand_Landmarks> all_images_landmarks_from_csv = readFromCSV("../../data/pptx/landmarks.csv");
    std::vector<float> all_labels_from_csv = readLabelsFromCSV("../../data/pptx/labels.csv");
    std::unordered_map<float, std::string> stringLabelMap = readMapFromCSV("../../data/pptx/map.csv");

    cv::Ptr<cv::ml::KNearest> knn = KNN_build(all_images_landmarks_from_csv, all_labels_from_csv);

    /** APPLICATION **/

    // Open the existing PowerPoint presentation
    openExistingPresentation(filePath);

    // model 

    // Establish connection with server
    int clientSocket = connectToServer(PORT, SERVER_IP);
    if (clientSocket == -1) {
        return -1;
    }


    // Camera capture and gesture recognition
    cv::VideoCapture cap(0);  // Open the default camera (usually the built-in webcam)

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    cv::namedWindow("PPT Controls - Video Feed", cv::WINDOW_AUTOSIZE);

    cv::VideoCapture capture;
    capture.open(0);

    int frameWidth = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));

    bool grab_frames = true;
    std::string prev_gesture = "";
    while (grab_frames) {
        cv::Mat inputImage;
        capture >> inputImage;

        std::vector<Hand_Landmarks> landmarks;
        bool success = getLandmarksFromServer(clientSocket, inputImage, landmarks);
        if (success == false) {
            return -1;
        }

        std::string curr_gesture;
        for (auto& lm : landmarks) {
            cv::Mat result;
            cv::Mat input_cvMat(1, 63, CV_32F);
            input_cvMat = lm.toMatRow();
            knn->findNearest(input_cvMat, stringLabelMap.size(), result);

            float predict = result.at<float>(0, 0);

            // convert class prediction number into string class label
            curr_gesture = stringLabelMap[predict];
        }

        std::cout << prev_gesture << "\t" << curr_gesture << std::endl;
        if (prev_gesture != curr_gesture && curr_gesture == "palm") {
            std::cout << "START SLIDE\n";
            handleGesture("StartSlide");
        }
        else if (prev_gesture != curr_gesture && curr_gesture == "fist") {
            std::cout << "END SLIDE\n";
            handleGesture("EndSlide");
        }
        else if (prev_gesture == "stop_inverted" && curr_gesture == "stop") {
            std::cout << "NEXT SLIDE\n";
            handleGesture("NextSlide");
        }
        else if (prev_gesture == "stop" && curr_gesture == "stop") {
            std::cout << "PREV SLIDE\n";
            handleGesture("PrevSlide");
        }


        int fontScale = 4;
        int textBaseline = 0;
        int thickness = 20;
        cv::Scalar outlineColor(0, 0, 0);
        cv::Scalar textColor(255, 255, 255);

        cv::Size textSize = cv::getTextSize(curr_gesture, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, nullptr);
        cv::Point textPosition(frameWidth - textSize.width - 500, frameHeight - 20);
        cv::putText(inputImage, curr_gesture, textPosition, cv::FONT_HERSHEY_SIMPLEX, fontScale, outlineColor, thickness + 10);
        cv::putText(inputImage, curr_gesture, textPosition, cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);


        cv::imshow("PPT Controls - Video Feed", inputImage);
        int key = cv::waitKey(1);

        // Check for user input (press 'q' to exit)
        if (key == 'q') {
            break;
        }

        prev_gesture = curr_gesture;

    }
    close(clientSocket);

    cap.release();
    cv::destroyAllWindows();



   


    return 0;
}