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

    std::string toString() const {
        std::ostringstream oss;
        oss << "(" << x << ", " << y << ", " << z << ")";
        return oss.str();
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
    std::vector<Landmark> landmarks;

    Hand_Landmarks(const std::vector<Landmark>& landmarks) {
        assert(landmarks.size() == 21);
        this->landmarks = landmarks;
    }

    Hand_Landmarks() {
        this->landmarks.resize(21, Landmark());
    }

    // Function to convert Hand_Landmarks to a string
    std::string toString() const {
        std::ostringstream oss;
        oss << "Hand_Landmarks: [";
        for (const auto& landmark : landmarks) {
            oss << landmark.toString() << ", ";
        }
        // Remove the trailing comma and space
        if (!landmarks.empty()) {
            oss.seekp(-2, std::ios_base::end);
        }
        oss << "]";
        return oss.str();
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
