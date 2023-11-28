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
 * @brief Stores an xyz coordinates of one point in the image (one point in landmarks)
 *
 * ## Construction and deconstruction
 * Initialize a new class using construct_representatives within the FalsePositiveFilter
 * 
 * ##Class Variables:
 * @double x
 * @double y
 * @double z
 */
class Landmark {
public:
    double x;
    double y;
    double z;
Landmark(): x(0.0), y(0.0), z(0.0) {}
Landmark(double x, double y, double z) : x(x), y(y), z(z) {}


    // Function to print the coordinates
    void printLandmark() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
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

    // Constructor with parameter
    Hand_Landmarks(const std::vector<Landmark>& landmarks) {
        assert(landmarks.size() == 21);
        this->landmarks = landmarks;
    }

    // Default constructor
    Hand_Landmarks() {
        this->landmarks.resize(21);  // Resizing with default-constructed Landmark objects
    }

    // Function to print hand landmarks
    std::string printHandLandmarks() const {
        std::ostringstream oss;
        oss << "Hand_Landmarks: [";
        for (const auto& landmark : landmarks) {
            oss << landmark.printLandmark() << ", ";
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