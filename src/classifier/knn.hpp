#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;


class Gest_KNN{
    int K_value;
    int num_classes;
    string Name;
    cv::Mat image;
    vector<Point> landmarks;
    //vector<Point> xyz;



}

class Gest_Model {
    int num_classes;
    std::vector<Gest_Class> Classes;
}
