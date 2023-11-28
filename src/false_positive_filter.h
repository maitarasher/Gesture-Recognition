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
/*
 * @brief Filters out landmarks that are not close enough to any class.
 * First constructs the class representatives,then compares the euclidean distance.
 *
 * ## Construction and deconstruction
 * Gets a dataset of all classes with labels, metric choice, confidence measure
 * 
 * ##Class Variables
 * @vector<(Landmark, Classifier)> dataset of all the viewed pictures with their label
 * @const string metric: specification whether to calculate euclidean or cosine distance
 * @float confidence: confidence parameter
 * @map<string, Representative>
 *
 * ## Class Functions
 * 
 */
public:
    FalsePositiveFilter(const vector<(cv::Mat,Classifiers)>& dataset, const string& metric = "cosine", const float confidence = 0.9);
/*
 * @brief Constructs the FalsePositiveFilter with dataset of all classes, metric,and confidence level
 */

    void constructRepresentatives();
/*
 * @brief Builds the representatives of each class. Constructed by taking the mean
 * of all the landmarks within a class.
 * @param p1: self, must have access to self.dataset
 * @return populates the map of [str, Representative]
 */

    tuple<float, int> closestRepresentative(const Landmark& sample);
/*
 * @brief Finds the closest representative to the sample using the specified similarity metric
 * @param sample: Landmarks information (21*3 vector) of the sample+ handness information of the sample
 * @return tuple of similiarity score and the class closest to the sample
 */

    bool is_relevant(Landmark sample);
/*
 * @brief Checks of a sample is relevant, if it is close enough to any class.
 * @param sample: sample to check if is relevant
 * @return True or False if sample is close to any class
 */

std::vector<int> bestHandsIndices(Landmark sample, int returnHands = 1);
/*
 * @brief Produces a list of indices of hands to keep
 * @param sample: all landmarks and handness information of sample picture
 * @param return hands: number of hands to return (1 is a default)
 * @return list of indices of hands to keep
 */

    static float cosineSimilarity(const Point& p1, const Point& p2);
/*
 * @brief Computes cosine similiarity of two points
 * @param p1: the first point
 * @param p2: the second point
 * @return similiarity
 */

    static float euclideanSimilarity(const Point& p1, const Point& p2);
/*
 * @brief Computes Euclidean similiarity of two points
 * @param v1: the first vector/or version with a point
 * @param v2: the second vector
 * @return similiarity
 */
private:
    
    vector<(Landmark, Classifier)> dataset;
    std::string metric;
    float confidence;
    std::unordered_map<std::string, Representative> representatives;
};