#pragma once
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;


//using cv::ml
cv::Ptr<cv::ml::KNearest> trainKNN(const cv::Mat& trainData, const cv::Mat& labels) {
    cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();
    
    // Set KNN parameters
    knn->setDefaultK(K_VALUE);
    knn->setIsClassifier(true);
    
    // Train the KNN model
    knn->train(trainData, cv::ml::ROW_SAMPLE, labels);

    return knn;
}

