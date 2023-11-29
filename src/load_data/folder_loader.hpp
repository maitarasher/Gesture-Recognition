#pragma once
#include <vector>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>

struct ImageData {
    //cv::Mat image;
    std::string filePath;
    std::string classStr;
    int label;
};

std::vector<ImageData> loadImgsFromFolder(const std::string& folderPath);
