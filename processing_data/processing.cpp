#include <iostream>
#include <cstring>
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
    if (argc != 3){
        std::cerr << "Usage: " << argv[0] << " <folder_path> <prefix_save_path>\n";
        std::cerr << "example: " << argv[0] << "  /Users/maitarasher/data ../../data/asl\n";
        return -1;
    }

    // Extract the folder path from the command-line arguments
    std::string folderPath = argv[1];
    // Extract the prefix path
    std::string prefixPath = argv[2];

    // (1) create a client socket and connect to the MediaPipe Server
    int clientSocket = connectToServer(PORT, SERVER_IP);
    if (clientSocket == -1) {
        return -1;
    }

    // (2) Create Pipeline structure

    // Create a pipeline with 1 hands
    Pipeline my_pipeline(1);

    // Add stages to the pipeline (brightness, rotation)
    my_pipeline.add_stage(40.0, 0.0);
    my_pipeline.add_stage(40.0, -15.0);
    my_pipeline.add_stage(40.0, 15.0);
    my_pipeline.add_stage(40.0, -30.0);
    my_pipeline.add_stage(40.0, 30.0);
    my_pipeline.add_stage(40.0, -45.0);
    my_pipeline.add_stage(40.0, 45.0);

    // (3) Load images by folder - call folder_loader, ImageData contians the image_path and the classlabel
    std::vector<ImageData> images = loadImgsFromFolder(folderPath);
    std::cout << "Finished loading images\n";

    std::vector<Hand_Landmarks> all_images_landmarks;
    std::vector<float> all_labels;
    std::unordered_map<float, std::string> stringLabelMap;

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
                // (d) Add label_string pair to the map
                stringLabelMap[imageData.label] = imageData.classStr;

                // (e) The first landmarks corresponding to at least one hand being detected is selected
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


    // (5) Save data to csv files.
    saveToCSV(all_images_landmarks , prefixPath + "/landmarks.csv");
    saveLabelsToCSV(all_labels, prefixPath + "/labels.csv");
    saveMapToCSV(stringLabelMap, prefixPath + "/map.csv");


    /*  (6) Load the data (for testing purposes)

        std::unordered_map<float, std::string> label_map_csv = readMapFromCSV(prefixPath + "/map.csv");
        for (auto pair : label_map_csv)
        {
        std::cout << pair.first << " " << pair.second << "\n";
        }

        std::vector<Hand_Landmarks> all_images_landmarks_from_csv = readFromCSV(prefixPath + "/landmarks.csv");
        std::vector<float> all_labels_from_csv = readLabelsFromCSV(prefixPath + "/labels.csv");


        int num = 1;
        for (const Hand_Landmarks& handLandmarks : all_images_landmarks_from_csv){
        std::cout << "handLandmark num " << num << " label: " << all_labels_from_csv[num-1] << " size: " << handLandmarks.landmarks.size() <<"\n";
        handLandmarks.print();
        }
     */
    close(clientSocket);
    return 0;
}
