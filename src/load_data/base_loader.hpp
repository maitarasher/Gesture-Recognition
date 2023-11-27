/*
 * @file base_loader.h
 * @author
 * @brief A class for getting landmarks
 * @version 1.0
 * @date updated Nov 25, 2023
 */

#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <filesystem>
#include <string>

// need to import landmark struct/point struct?
#import //landmark class to get the landmarks
#import //pipeline? to run a pieline of images

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
    // pipeline to use to detect landmarks
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

    /*
    @brief: Processes a single image and retrieves the landmarks of this image.
    @param file_path: the file path of the file to read
    @param world_landmarks:?

    @return: the list of landmarks detected by MediaPipe or an empy list
    if no landmarks were found
    @return list of handedness labels (left,right)
        */
    vector<int> create_landmarks(string file_path); {

        
    }
    
     
};

// maybe include only class definitions and function names
// write function implementations in cpp?
