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
class Landmark {
public:
    double x;
    double y;
    double z;
Landmark(): x(0.0), y(0.0), z(0.0) {}
Landmark(double x, double y, double z) : x(x), y(y), z(z) {}
Landmark(const std::vector<double>& point) {
        if (point.size() == 3) {
            x = point[0];
            y = point[1];
            z = point[2];
        } else {
            // Handle incorrect vector size, set default values
            x = 0.0;
            y = 0.0;
            z = 0.0;
            std::cerr << "Warning: Invalid vector size for Landmark constructor. Default values used." << std::endl;
        }
    }

    // Function to print the coordinates
    void print()  {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
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
 
class Hand_Landmarks {
    private:
    vector<Landmark> landmarks(21);
    //int handedness;

    public: 
    //Construct with vector
    Hand_Landmark(vector<Point>& landmarks) {
        assert(landmarks.size()==21)
        this->landmarks=landmarks;
        
    }
    //Default constructor
    Hand_Landmark() {

        this->landmarks.resize(21,0);
        
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
