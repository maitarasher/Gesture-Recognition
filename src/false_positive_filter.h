/*
 * @file false_positive_filter

 * @brief Filters out landmarks that are not close enough to any class.
 * First, creates the representative samples of each class by taking the mean landmarks of each class.
 * Then compares every incoming image using euclidean or cosine distance to the representative.
 * If any of the representatives are withing confidence, the sample is marked as relevant and saved further.
 * 
 * @version 1.0
 * @date updated Nov 27, 2023
 */

#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <opencv2/core.hpp>
using namespace std;


class Representative {
/*
 * @brief Stores a representative of the class, the handedness information and the action
 * to perform upon recognizing the gesture.
 *
 * ## Construction and deconstruction
 * Initialize a new class using construct_representatives within the FalsePositiveFilter
 * 
 * ##Class Variables:
 * @vector<double> mean_landmarks: 3*1 vector of xyz mean coordinates
 * @int handness: 0 stands for left, 1 stands for right hand
 * @string action: action associated with the class
 * @string class_name 
 */
public:
    vector<Point> mean_landmarks;
    int handedness;
    Classifier label;
    
    //or 
    //Point mean_landmarks

    // constructor
    Representative(const vector<double>& meanLandmarks, const string& handedness, const string& action, const string& class_name) {
        self.meanLandmarks = meanLandmarks;
        self.handedness = handedness;
        self.action = action;
        self.class_name = class_name;
        label = //construct
    }
};


class FalsePositiveFilter {
/**
 * @brief Filters out landmarks that are not close enough to any class.
 * First constructs the class representatives,then compares the euclidean distance.
 *
 * ## Construction and deconstruction
 * Gets a dataset of all classes with labels, metric choice, confidence measure
 * 
 * ##Class Variables
 * @vector<> dataset
 * @const string metric: specification whether to calculate euclidean or cosine distance
 * @float confidence: confidence parameter
 * @map<string, Representative>
 *
 * ## Class Functions
 * 
 */
public:
    // constructor
    FalsePositiveFilter(const vector<Classifiers>& dataset, const sstring& metric = "cosine", const float confidence = 0.9);

    void constructRepresentatives();

    tuple<float, int> closestRepresentative(const cv::Mat& sampleLandmarks, const cv::Mat& sampleHandedness);

    std::vector<int> bestHandsIndices(const cv::Mat& landmarks, const cv::Mat& handedness, int returnHands = 1);

    static float cosineSimilarity(const cv::Mat& v1, const cv::Mat& v2);

    static float euclideanSimilarity(const cv::Mat& v1, const cv::Mat& v2);

private:
    
    vector<Gesture> dataset;
    std::string metric;
    float confidence;
    std::unordered_map<std::string, Representative> representatives;
};