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
Point(): x(0.0), y(0.0), z(0.0) {}
Point(double x, double y, double z) : x(x), y(y), z(z) {}


    // Function to print the coordinates
    void printPoint() const {
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
    private:
    vector<Landmark> landmarks(21);
    //int handedness;

    public: 
    Hand_Landmark(vector<Point>& landmarks) {
        assert(landmarks.size()==21)
        this->landmarks=landmarks;
        
    }
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