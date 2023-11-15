// implementation of functions
#include "base_loader.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <filesystem>
#include <string>

vector<cv::Mat> loadImgsFromFolder()
{
    vector<cv::Mat> images;
    // check if the path is valid?
    for (const auto &entry : fs::directory_iterator(fpath))
    {
        string filePath = entry.path().string();

        // Read the image
        cv::Mat image = cv::imread(filePath);

        // check if the image was loaded
        if (!image.empty())
        {
            images.push_back(image);
        }
        else
        {
            cerr << "Error laoding image: " << filePath << endl;
        }
    }
    return images;
}