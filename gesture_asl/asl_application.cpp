#include "../src/feature_extraction/pipeline.hpp"
#include "../src/feature_extraction/stage.hpp"
#include "../src/load_data/folder_loader.hpp"
#include "../src/load_data/save_and_read.cpp"
#include "../src/data_classes.hpp"
#include "../src/mediapipe_client/mediapipe_client.hpp"
#include "../src/classifier/knn.hpp"
#include "../src/classifier/svm.hpp"
#include "../src/classifier/dtree.hpp"

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";

int main(int argc, char* argv[]){
    // Check if the folder path is provided as a command-line argument

    if (argc != 2){
        std::cerr << "Usage: " << argv[0] << " <prefix_save_path>\n";
        std::cerr << "example: " << argv[0] <<" ../../data/asl\n";
        return -1;
    }

    // Extract the prefix path
    std::string prefixPath = argv[1];

    // (1) create a client socket and connect to the MediaPipe Server
    int clientSocket = connectToServer(PORT, SERVER_IP);
    if (clientSocket == -1) {
        return -1;
    }

    // (2) Load the data
    std::vector<Hand_Landmarks> all_images_landmarks_from_csv = readFromCSV(prefixPath + "/landmarks.csv");
    std::vector<float> all_labels_from_csv = readLabelsFromCSV(prefixPath + "/labels.csv");
    std::unordered_map<float, std::string> stringLabelMap = readMapFromCSV(prefixPath + "/map.csv");

    // // (3) Train the data using K classiffier
    // // Create KNN classifier
    // auto [knn,knn_accuracy] = KNN_build(all_images_landmarks_from_csv, all_labels_from_csv, 3);
    // std::cout << "KNN Accuracy: " << knn_accuracy << std::endl;

    auto [svm,svm_accuracy] = SVM_build(all_images_landmarks_from_csv, all_labels_from_csv, stringLabelMap.size());
    std::cout << "SVM Accuracy: " << svm_accuracy << std::endl;

    // auto [dtree,dtree_accuracy] = DTree_build(all_images_landmarks_from_csv, all_labels_from_csv, stringLabelMap.size());
    // std::cout << "DTree Accuracy: " << svm_accuracy << std::endl;


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

        std::vector<Hand_Landmarks> landmarks;
        bool success = getLandmarksFromServer(clientSocket, inputImage, landmarks);
        if (success == false) {
            return -1;
        }

        std::string text;
        for (auto& lm : landmarks) {
            cv::Mat result;
            cv::Mat input_cvMat(1, 63, CV_32F);
            input_cvMat = lm.toMatRow();
            // knn->findNearest(input_cvMat, 3, result);
            svm->predict(input_cvMat,result);

            float predict = result.at<float>(0, 0);

            // convert class prediction number into string class label
            text = stringLabelMap[predict];

            std::cout << "Prediction: " << predict << std::endl;
            std::cout << "Text: " << text << std::endl;
            // std::cout << "Distance: " << dist.at<float>(0,0) << std::endl;
        }

        int fontScale = 8;
        int textBaseline = 0;
        int thickness = 30;
        cv::Scalar outlineColor(0, 0, 0);
        cv::Scalar textColor(255, 255, 255);

        cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, nullptr);
        cv::Point textPosition(frameWidth - textSize.width - 100, frameHeight - 20);
        cv::putText(inputImage, text, textPosition, cv::FONT_HERSHEY_SIMPLEX, fontScale, outlineColor, thickness + 10);
        cv::putText(inputImage, text, textPosition, cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, thickness);

        cv::imshow("ASL Application", inputImage);

        int key = cv::waitKey(1);

        // Check for user input (press 'q' to exit)
        if (key == 'q') {
            break;
        }
    }
    close(clientSocket);
    return 0;   
}
