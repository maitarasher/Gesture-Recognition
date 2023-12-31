#include <fstream>
#include <json/json.h>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include <typeinfo>

// using namespace std;


int main(int argc, char* argv[]) {

    if (argc != 3){
        std::cerr << "Usage: " << argv[0] << " <coco_folder_path> <output_folder>\n";
        std::cerr << "example: " << argv[0] <<" /Users/elifiamuthia/Desktop/like /Users/elifiamuthia/Desktop/output_folder\n";
        return -1;
    }

    std::string path = argv[1];
    std::string output_folder = argv[2];

    size_t lastSlashPos = path.find_last_of('/');
    std::string folderName = path.substr(lastSlashPos + 1);
    

    // Read the JSON file
    std::ifstream file(path + "/" + folderName + ".json");
    Json::Value jsonData;
    file >> jsonData;

    // Iterate over each entry in the JSON
    for (const auto& entry : jsonData.getMemberNames()) {
        // Get image name (key)
        std::string imageName = entry;
        std::cout<<"name: "<<imageName<<"\n";

        // Get values
        auto& values = jsonData[entry];
        // std::cout<<"values: "<<values<<"\n";
        std::cout<<values["bboxes"][0][0].asFloat()<<"\n";

        float x = 0.0, y = 0.0, width = 0.0, height = 0.0;
        x = values["bboxes"][0][0].asFloat();
        y = values["bboxes"][0][1].asFloat();
        width = values["bboxes"][0][2].asFloat();
        height = values["bboxes"][0][3].asFloat();

        // Print the extracted values
        std::cout << "Image: " << imageName << "\n";
        std::cout << "x: " << x << "\n";
        std::cout << "y: " << y << "\n";
        std::cout << "width: " << width << "\n";
        std::cout << "height: " << height << "\n\n";

        // Load image using OpenCV
        std::string imagePath = path + "/" + imageName + ".jpg";
        std::cout << "ImagePath: " << imagePath << "\n";
        cv::Mat image = cv::imread(imagePath);

        // Crop the image based on bounding box
        cv::Rect roi(x * image.cols, y * image.rows, width * image.cols, height * image.rows);
        cv::Mat croppedImage = image(roi);
        std::cout << "Cropped Image Size: " << croppedImage.size() << "\n";

        // Get label
        std::string label = values["labels"][0].asString();
        std::cout << "label:  " << label << "\n";

        // std::string label = values["labels"][0].asString();
        std::string outputFolder = output_folder + "/" + label + "/";

        if (!std::filesystem::exists(outputFolder))
            std::filesystem::create_directories(outputFolder);

        // Save the cropped image to a folder based on its label
        // std::string outputFolder = "/Users/elvinawibisono/Desktop/designusingc+/Gesture-Recognition/gesture_pptx/output_folder/" + label + "/";
        std::cout << "outputFolder  " << outputFolder << "\n";
        std::string imagePathToSave = outputFolder + imageName + "_cropped.jpg";
        std::cout << "Saving to: " << imagePathToSave << "\n";
        cv::imwrite(imagePathToSave, croppedImage);
    }

    return 0;
}
























