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

//do we need  different class for Point2D?
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

class Landmark {
    private:
    vector<Point> landmarks(21);
    int handedness;
    public: 
    Landmark(vector<Point>& landmarks, int handedness) {

    }
    //get mean?
}