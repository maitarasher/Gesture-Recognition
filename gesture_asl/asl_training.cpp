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

    // Check if the folder path is provided as a command-line argument
    if (argc != 2){
        std::cerr << "Usage: " << argv[0] << " <folder_path>" << std::endl;
        return -1;
    }

    // (1) create a client socket and connect to the MediaPipe Server

    int clientSocket = connectToServer(PORT, SERVER_IP);
    if (clientSocket == -1) {
        return -1;
    }

    // (2) Create Pipeline structure

    // Create a pipeline with 2 hands
    Pipeline my_pipeline(2);

    // Add stages to the pipeline (brightness, rotation)
    my_pipeline.add_stage(40.0, 0.0);
    my_pipeline.add_stage(40.0, -45.0);
    my_pipeline.add_stage(40.0, 45.0);
    my_pipeline.add_stage(40.0, 90.0);

    // std::string folderPath = "/Users/elifiamuthia/Desktop/asl_dataset/asl_alphabet_train/";
    // std::string folderPath = "/Users/maitarasher/Desktop/gesture_data/subsample";
    // std::string folderPath = "/Users/noamzaid/data/test/subsample";
    // Extract the folder path from the command-line arguments
    std::string folderPath = argv[1];
    // (3) Load images by folder - call folder_loader, ImageData contians the image_path and the classlabel
    std::vector<ImageData> images = loadImgsFromFolder(folderPath);
    std::cout << "Finished loading images\n";

    std::vector<Hand_Landmarks> all_images_landmarks;
    std::vector<float> all_labels;
    // std::vector<int> all_images_labels;
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

                //Show the original and processed images for each stage
                // std::cout << "Class String: " << imageData.classStr << std::endl;
                // std::cout << "Class Label: " << imageData.label << std::endl;
                // cv::imshow("Original Image", image);
                // cv::imshow("Brightened Image", inputImage);
                // cv::waitKey(1000);

                // (a) Get landmarks for each augmented image in the Pipeline by sending a the image to the server and getting a reposnd
                std::vector<Hand_Landmarks> landmarks;
                bool success = getLandmarksFromServer(clientSocket, inputImage, landmarks);
                if (success == false) {
                    return -1;
                }

                // (b) Add landmark to the Data Structure
                // (c) Add landmarks to the class labels Data Structure
                for (Hand_Landmarks& lm : landmarks) {
                    all_images_landmarks.push_back(lm);
                    all_labels.push_back(imageData.label);
                }

                // (d) The first landmarks corresponding to at least one hand being detected is selected
                std::cout << "\nGetting landmarks for " << imageData.filePath << " landmarks.size(): " << landmarks.size() << " imageData.label: " << imageData.label << "\n";
                if (landmarks.size() > 0) break;
            }
        }
        else
        {
            std::cerr << "Error loading image: " << imageData.filePath << std::endl;
        }
    }
    std::cout << "finshed getting Landmarks for all images\n";
    std::cout << "all_images_landmarks.size(): " << all_images_landmarks.size() << "\n";
    std::cout << "all_labels.size(): " << all_labels.size() << "\n";


    // (5) Save the two vectors all_images_landmarks, all_labels in csv files
    saveToCSV(all_images_landmarks , "../gesture_asl/data/asl_landmarks.csv");
    saveLabelsToCSV(all_labels, "../gesture_asl/data/asl_labels.csv");

    // The rest of the code below can be in a seprate program once ready
    // (6) Load the data
    std::vector<Hand_Landmarks> all_images_landmarks_from_csv = readFromCSV("../gesture_asl/data/pptx_data_landmarks.csv");
    std::vector<float> all_labels_from_csv = readLabelsFromCSV("../gesture_asl/data/pptx_data_labels.csv");

    // printing the landmarks and labels for testing purposes
    // (ONLY WHEN USING SMALL AMOUNTS OF DATA):
    // int num = 1;
    // for (const Hand_Landmarks& handLandmarks : all_images_landmarks_from_csv){
    //     std::cout << "handLandmark num " << num << " label: " << all_labels_from_csv[num-1] << " size: " << handLandmarks.landmarks.size() <<"\n";
    //     handLandmarks.print();
    //     cout << endl; // Optional: for better readability between each Hand_Landmarks
    //     num++;
    // }

    // for (const float& label : all_labels_from_csv)
    //     std::cout << label << std::endl;



    // (7) Split the Data into train and test /* HANDLED IN KNN Classifier */
    // (8) Train the data using K classiffier
    // Create KNN classifier
    float accuracy = KNN_build(all_images_landmarks, all_labels);
    std::cout << "accuracy: " << accuracy << "\n";

    // (9) Get results

    // (10) Use the model for parsing the gestures in a recorded video stream - retrurn script of words


    ///// SEND VIDEO TO MEDIAPIPE, GET LANDMARKS BACK, GET ACTION FROM CLASSIFIER, DO ACTION
    // cv::VideoCapture capture;
    // capture.open(0);
    // bool grab_frames = true;
    // while (grab_frames) {
    //     cv::Mat inputImage;
    //     capture >> inputImage;

    //     std::vector<Hand_Landmarks> landmarks;
    //     bool success = getLandmarksFromServer(clientSocket, inputImage, landmarks);
    //     if (success == false) {
    //         return -1;
    //     }

    //     // std::cout << landmarks.size() << std::endl;


    //     // TODO
    //     // RUN LANDMARKS THROUGH CLASSIFIER AND GET CORRESPONDING ACTION
    // }
    close(clientSocket);
    return 0;
}
