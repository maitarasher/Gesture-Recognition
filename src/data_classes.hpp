/*
 * @file false_positive_filter
 
 * @brief A helper file to organize data structure in Point, Landmark, Frames
 * @version 1.0
 * @date updated Nov 27, 2023
 */

#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;

/*
 * @brief Stores an xyz coordinates of one landmark in the image 
 * ## Construction and deconstruction
 * Default construction is (0,0,0), can construct from 3 floats or from a vector
 * 
 * ##Class Variables:
 * @float x
 * @float y
 * @float z
 */

//TESTED
class Landmark {
public:
    float x;
    float y;
    float z;
//Default constructor , sets all coordinates to 0
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

    // Function to convert Landmark to a cv::row vector
    cv::Mat toMatRow() const {
        cv::Mat row(1, 3, CV_32F);
        row.at<float>(0, 0) = x;
        row.at<float>(0, 1) = y;
        row.at<float>(0, 2) = z;
        return row;
    }
    // Function to print the coordinates
    void print()  {
        cout << "(" << x << ", " << y << ", " << z << ")" << endl;
    }

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
 *
 * ## Construction and deconstruction
 * 
 * ##Class Variables:
 * @double x
 * @double y
 * @double z
 */
class Hand_Landmarks {
    public:
    vector<Landmark> landmarks;
    //int handedness;
    
    // Constructor to initialize the vector with 21 default Landmark objects
    Hand_Landmarks() : landmarks(21, Landmark()) {}
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


/*
 * @brief Stores possible names and actions for "labels" for images
 *
 * ## Construction and deconstruction
 * 
 * ##Class Variables:
 * @string name
 * @string action
 */
/*class Label {
    private:
    string name;
    string action;
    
    public: 
    Landmark(const string& name, const string& action) {
        
    }
}*/

//test
