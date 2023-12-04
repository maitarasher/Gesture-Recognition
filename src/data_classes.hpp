/**
 * @file data_classes.hpp
 * @brief A helper file to organize the data structure in Landmark and Hand_Landmarks
 * @version 1.0
 * @date updated Dec 2, 2023
 */

#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;

/**
 * @brief Stores an xyz coordinates of one landmark in the image 
 * 
 * ## Construction and destruction
 * A new landmark can be constructed from x, y, z float variables or from a single vector of size 3.
 * If no parameters are given the Landmark is set to [0,0,0].
 *
 * ## Methods
 * - toMatRow() : Conerts the Landmark into cv::Mat object
 * - toVector (): Converts the Landmark into vector<float>
 * - print(): prints the Landmark's coordinates
 */

class Landmark {
public:
    float x;
    float y;
    float z;

Landmark(): x(0.0), y(0.0), z(0.0) {}
Landmark(float x, float y, float z) : x(x), y(y), z(z) {}
Landmark(vector<float>& point) {
        if (point.size() == 3) {
            x = point[0];
            y = point[1];
            z = point[2];
        } else {
            // Handle incorrect vector size, set default values
            x = 0.0;
            y = 0.0;
            z = 0.0;
            cerr << "Warning: Invalid vector size for Landmark constructor. Default values used." << std::endl;
        }
    }

    // Convert Landmark to a cv::row vector
    cv::Mat toMatRow() const {
        cv::Mat row(1, 3, CV_32F);
        row.at<float>(0, 0) = x;
        row.at<float>(0, 1) = y;
        row.at<float>(0, 2) = z;
        return row;
    }
    // Print the coordinates
    void print()  {
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }
    //Convert Landmark to a vector
    vector<float> toVector() {
        vector<float> landmark_vector; 
        landmark_vector.push_back(x);
        landmark_vector.push_back(y);
        landmark_vector.push_back(z);
        return landmark_vector;   
    }

};

/*
 * @brief Represents all the landmarks returned by the mediapipe for one hand.
 * One Hand_Landmark consists of 21 Landmarks.
 *
 * ## Construction and deconstruction
 * A new Hand_Landmarks object can be constructed from a vector <Landmark>.
 * The default constructor creates an empty vector <Landmark>.
 * 
 * ##Methods 
 * toMatRow(): Converts the Hand_Landmarks to cv::Mat of size 63 (21*3)
 * print(): prints the Hand_Landmarks values
 */
class Hand_Landmarks {
    public:
    vector<Landmark> landmarks;
    //int handedness;
    
    Hand_Landmarks() : landmarks() {}
    // Constructor to initialize the vector with specific Landmark objects
    Hand_Landmarks(const vector<Landmark>& initialLandmarks) : landmarks(initialLandmarks) {}

    //function to convert the Hand_Landmarks to cv Matrix for data manipulations
    cv::Mat toMatRow() const {
        cv::Mat matRow(1, 63, CV_32F);
        int colIndex = 0;

        for (const Landmark& landmark : landmarks) {
            cv::Mat landmarkRow = landmark.toMatRow();
            landmarkRow.copyTo(matRow(cv::Rect(colIndex, 0, 3, 1)));
            colIndex += 3;
        }

        return matRow;
    }
    // Print function to display the contents of Hand_Landmarks
    void print() const {
        for (const Landmark& landmark : landmarks) {
            cout << "(" << landmark.x << ", " << landmark.y << ", " << landmark.z << ")" << endl;
        }
    }
};

