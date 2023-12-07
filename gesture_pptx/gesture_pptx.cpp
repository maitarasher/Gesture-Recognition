#include "../src/mediapipe_client/mediapipe_client.hpp"
#include "../src/data_classes.hpp"
#include "../src/load_data/save_and_read.cpp"
#include "../src/classifier/knn.hpp"
#include "../src/classifier/svm.hpp"
#include "../src/classifier/dtree.hpp"

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

int main(int argc, char* argv[]) {

    std::cout << "Hello, MacOS PowerPoint Application!" << std::endl;

    if (argc != 3){
        std::cerr << "Usage: " << argv[0] << " <prefix_save_path> <absolute_path_to_pptx>\n";
        std::cerr << "example: " << argv[0] <<" ../../data/pptx /Desktop/test.pptx\n";
        return -1;
    }

    std::string prefixPath = argv[1];
    const char* filePath = argv[2];
    /** TRAINING **/

    // prepare data
    std::vector<Hand_Landmarks> all_images_landmarks_from_csv = readFromCSV(prefixPath + "/landmarks.csv");
    std::vector<float> all_labels_from_csv = readLabelsFromCSV(prefixPath + "/labels.csv");
    std::unordered_map<float, std::string> stringLabelMap = readMapFromCSV(prefixPath + "/map.csv");

    // std::cout << "Training KNN...\n";
    // auto [knn,knn_accuracy] = KNN_build(all_images_landmarks_from_csv, all_labels_from_csv, stringLabelMap.size());
    // std::cout << "KNN Accuracy: " << knn_accuracy << std::endl;
    
    std::cout << "Training SVM...\n";
    auto [svm,svm_accuracy] = SVM_build(all_images_landmarks_from_csv, all_labels_from_csv, stringLabelMap.size());
    std::cout << "SVM Accuracy: " << svm_accuracy << std::endl;

    // std::cout << "Training Decision Tree...\n";
    // auto [dtree,dtree_accuracy] = DTree_build(all_images_landmarks_from_csv, all_labels_from_csv, stringLabelMap.size());
    // std::cout << "Decision Tree Accuracy: " << dtree_accuracy << std::endl;


    /** APPLICATION **/

    // Open the existing PowerPoint presentation
    openExistingPresentation(filePath);

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

            // knn->findNearest(input_cvMat, 3, result);
            svm->predict(input_cvMat,result);
            // dtree->predict(input_cvMat,result);

            float predict = result.at<float>(0, 0);

            // convert class prediction number into string class label
            curr_gesture = stringLabelMap[predict];
        }

        std::cout << prev_gesture << "\t" << curr_gesture << std::endl;
        if (prev_gesture == curr_gesture && curr_gesture == "ok") {
            std::cout << "START SLIDE\n";
            handleGesture("StartSlide");
        }
        else if (prev_gesture == curr_gesture && curr_gesture == "fist") {
            std::cout << "END SLIDE\n";
            handleGesture("EndSlide");
        }
        else if (prev_gesture == curr_gesture && curr_gesture == "like") {
            std::cout << "NEXT SLIDE\n";
            handleGesture("NextSlide");
        }
        else if (prev_gesture == curr_gesture && curr_gesture == "dislike") {
            std::cout << "PREV SLIDE\n";
            handleGesture("PrevSlide");
        }

        prev_gesture = curr_gesture;

    }
    close(clientSocket);

    cap.release();
    cv::destroyAllWindows();

    return 0;
}