#ifndef FALSE_POSITIVE_FILTER_H
#define FALSE_POSITIVE_FILTER_H

#include <vector>
#include <string>
#include <stdexcept>
#include <opencv2/core.hpp>

class Representative {
public:
    cv::Mat mean_landmarks;
    cv::Mat handedness;

    // constructor
    Representative(const cv::Mat& meanLandmarks, const cv::Mat& handedness);
};

class FalsePositiveFilter {
public:
    // constructor
    FalsePositiveFilter(const cv::Mat& dataset, const std::string& metric = "cosine", const float confidence = 0.9);

    void constructRepresentatives();

    std::tuple<float, int> closestRepresentative(const cv::Mat& sampleLandmarks, const cv::Mat& sampleHandedness);

    std::vector<int> bestHandsIndices(const cv::Mat& landmarks, const cv::Mat& handedness, int returnHands = 1);

    static float cosineSimilarity(const cv::Mat& v1, const cv::Mat& v2);

    static float euclideanSimilarity(const cv::Mat& v1, const cv::Mat& v2);

private:
    cv::Mat dataset;
    std::string metric;
    float confidence;
    std::unordered_map<std::string, Representative> representatives;
};