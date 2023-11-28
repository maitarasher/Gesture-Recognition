#include <vector>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>
#include "folder_loader.hpp"

std::vector<ImageData> loadImgsFromFolder(const std::string& folderPath)
{
    std::vector<ImageData> images;

    // Check if the folder path exists
    if (!std::filesystem::exists(folderPath) || !std::filesystem::is_directory(folderPath))
    {
      std::cerr << "Error: Folder path not found or is not a directory: " << folderPath << std::endl;
      return images; // Return empty vector since the folder path is invalid
    }

    // Iterate over subfolders to get class labels
    for (const auto &classEntry : std::filesystem::directory_iterator(folderPath))
    {
        if (classEntry.is_directory())
        {
            std::string classLabel = classEntry.path().filename().string();
            std::cout << "Adding file path images for classLabel: " << classLabel << "\n";

            // Iterate over images in the current class folder
            for (const auto &imageEntry : std::filesystem::directory_iterator(classEntry.path()))
            {
                std::string filePath = imageEntry.path().string();
                // std::cout << "File path: " << filePath << "\n";
                ImageData imageData{filePath, classLabel};
                images.push_back(imageData);

                // // Read the image
                // cv::Mat image = cv::imread(filePath);
                // // Check if the image was loaded
                // if (!image.empty())
                // {
                //     // Create an ImageData structure with image and class label
                //     ImageData imageData{image, classLabel};
                //     images.push_back(imageData);
                // }
                // else
                // {
                //     std::cerr << "Error loading image: " << filePath << std::endl;
                // }
            }
        }
    }
    return images;
}
