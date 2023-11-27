#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;


class Gest_CLASS{
    string Name;
    int index;
    vector<Point> landmarks;
    //vector<Point> xyz;



}

class Gest_KNN{
    int K_value
    int num_classes;
    std::vector<Gest_Class> Classes;
    cv::Mat Dataset
}

/*

//we need to add euclideanDistance
float euclideanDistance(const Point& point_test, const Point& point_check) const {
    double x = point_test.x - point_check.x;
    double y = point_test.y - point_check.y;
    double z = point_test.z - point_check.z
    return sqrt(x*x+y*y+z*z);
}*/
