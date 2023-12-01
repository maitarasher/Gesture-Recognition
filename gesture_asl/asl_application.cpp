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
#include "../src/load_data/save_and_read.cpp"
#include "../src/data_classes.hpp"
#include "../src/mediapipe_client/mediapipe_client.hpp"
#include "../src/classifier/knn.hpp"

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";

int main(int argc, char* argv[]){
    // (1) create a client socket and connect to the MediaPipe Server

    // int clientSocket = connectToServer(PORT, SERVER_IP);
    // if (clientSocket == -1) {
    //     return -1;
    // }

    // (6) Load the data
    // std::vector<Hand_Landmarks> all_images_landmarks_from_csv = readFromCSV("../gesture_asl/data/pptx_data_landmarks.csv");
    // std::vector<float> all_labels_from_csv = readLabelsFromCSV("../gesture_asl/data/pptx_data_labels.csv");

    // // get number of classes
    // std::set<float> unique_classes(all_labels_from_csv.begin(), all_labels_from_csv.end());
    // std::size_t classes_count = unique_classes.size();

    // // (8) Train the data using K classiffier
    // // Create KNN classifier
    // cv::Ptr<cv::ml::KNearest> knn = KNN_build(all_images_landmarks_from_csv, all_labels_from_csv);
    // std::cout << "accuracy: " << accuracy << "\n";

    ///// SEND VIDEO TO MEDIAPIPE, GET LANDMARKS BACK, GET ACTION FROM CLASSIFIER, DO ACTION
    cv::VideoCapture capture;
    capture.open(0);
    bool grab_frames = true;

    int frameWidth = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));

    cv::namedWindow("ASL Application", cv::WINDOW_NORMAL);
    cv::resizeWindow("ASL Application", frameWidth, frameHeight);

    while (grab_frames) {
        cv::Mat inputImage;
        capture >> inputImage;

        // std::vector<Hand_Landmarks> landmarks;
        // bool success = getLandmarksFromServer(clientSocket, inputImage, landmarks);
        // if (success == false) {
        //     return -1;
        // }

        // // std::cout << landmarks.size() << std::endl;

        // // TODO
        // // RUN LANDMARKS THROUGH CLASSIFIER 
        
        // std::vector<std::string> frame_label_preds;
        // for (auto& lm : landmarks) {
        //     cv::Mat result;
        //     cv::Mat input_cvMat(1, 63, CV_32F);
        //     input_cvMat = lm.toMatRow();
        //     knn->findNearest(input_cvMat, 3, result);

        //     int predict = result.at<float>(0, 0);

        //     // convert class prediction number into string class label

        // }

        std::string text = "A"; // THE PREDICTION

        int fontScale = 8;
        int textBaseline = 0;
        int thickness = 30;  // Thickness of the text

        // Get the size of the text
        cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &textBaseline);
        cv::Point textPosition(frameWidth - textSize.width - 20, frameHeight - 20);
        cv::putText(inputImage, text, textPosition, cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar(255, 255, 255), thickness);

        cv::imshow("ASL Application", inputImage);

        int key = cv::waitKey(1);

        // Check for user input (press 'q' to exit)
        if (key == 'q') {
            break;
        }

    
        
        
        //AND GET CORRESPONDING ACTION



    }
    // close(clientSocket);
    return 0;
}
