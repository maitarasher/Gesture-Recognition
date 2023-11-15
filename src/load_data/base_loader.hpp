/*
 * @file base_loader.h
 * @author
 * @brief A class for
 * @version
 * @date
 */

#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <filesystem>
#include <string>

// need to import landmark struct/point struct?
#import //whatever you need

// include exceptions - write exception/error handlers!
// need landmarks class?
using namespace std;
namespace fs = filesystem;

/*
    @brief: Retrieves landmarks from folder with images
*/
class BaseLoader
{
    string fpath;
    // pipeline
public:
    // BaseLoader (Pipeline, path, bool) //add Pipeline class
    // add any public
    BaseLoader(const std::string &folder_path)
    {
        self.folder_path = folder_path;
        // pipeline
    }
    /*
    A function to load the images from folder into vector
    */
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
    //probably need a class landmark? also exception for NoHandDetect
    vector<int> create_landmarks(string file_path);
    
    //do we need load_landmarks? what df can we use?
     
};

// maybe include only class definitions and function names
// write function implementations in cpp?
