#include <iostream>
#include <opencv2/opencv.hpp>
#include<opencv2/ml.hpp>
#include <vector>
#include <iostream>
#include <filesystem>
#include <string>
#include <cassert>
#include <complex>
#include <algorithm>

const int K_VALUE = 5;

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

