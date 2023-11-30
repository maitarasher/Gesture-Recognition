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

    float label = 0.;

    // Iterate over subfolders to get class labels
    for (const auto &classEntry : std::filesystem::directory_iterator(folderPath))
    {

        if (classEntry.is_directory())
        {
            std::string classStr = classEntry.path().filename().string();
            std::cout << "Adding file path images for class string: " << classStr << ", class label: " << label <<"\n";

            // Iterate over images in the current class folder
            for (const auto &imageEntry : std::filesystem::directory_iterator(classEntry.path()))
            {
                std::string filePath = imageEntry.path().string();
                // std::cout << "File path: " << filePath << "\n";
                ImageData imageData{filePath, classStr, label};
                images.push_back(imageData);

            }
            label += 1.;
        }
    }
    return images;
}
