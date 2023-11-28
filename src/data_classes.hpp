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
class Point {
private:
    double x;
    double y;
    double z;
public:
Point(): x(0.0), y(0.0), z(0.0) {}
Point(double x, double y, double z) : x(x), y(y), z(z) {}

    // Getter methods
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }

    // Setter methods
    void setX(double newX) { x = newX; }
    void setY(double newY) { y = newY; }
    void setZ(double newZ) { z = newZ; }

    // Function to print the coordinates
    void printPoint() const {
        std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
    }
}

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
class Landmark {
    private:
    vector<Point> landmarks(21);
    int handedness;

    public: 
    Landmark(vector<Point>& landmarks, int handedness) {

    }
}

/*
 * @brief Stores possible names and actions for "labels" for images
 *
 * ## Construction and deconstruction
 * 
 * ##Class Variables:
 * @string name
 * @string action
 */
class Classifier {
    private:
    string name;
    string action;
    
    public: 
    Landmark(const string& name, const string& action) {
        
    }
}