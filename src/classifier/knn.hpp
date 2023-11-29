#pragma once
#include <vector>
#include <string>
#include <numeric>
#include <opencv2/opencv.hpp>
using namespace std;


//using cv::ml build KNN model

cv::Ptr<cv::ml::KNearest> KNN_build(const std::vector<Hand_Landmarks>& all_data, const vector<int>& all_labels) {
    
    // Split the data into training and testing sets
    float train_percentage = 0.8;
    int total_size = static_cast<int>(all_data.size());
    int train_size = static_cast<int>(train_percentage * total_size); 
    int test_size = static_cast<int>(total_size-train_size);

    // Shuffle the indices to randomly select samples for training 
    std::vector<int> indx(total_size);
    //fill with range from 0 to total_size-1
    std::iota(indx.begin(), indx.end(), 0); 
    std::random_shuffle(indx.begin(), indx.end());  

    // Separate the indices for training and testing
    std::vector<int> train_indx(indx.begin(), indx.begin() + total_size);
    std::vector<int> test_indx(indx.begin() + total_size, indx.end());
    
    // Create training  dataset
    std::vector<Hand_Landmarks> train_data(train_size);
    std::vector<int> train_labels(train_size);
    for (int i = 0; i < train_size; ++i) {
        train_data[i] = all_data[train_indx[i]];
        train_labels[i] = all_labels[train_indx[i]];
    }

    //Create test dataset
    std::vector<Hand_Landmarks> test_data(test_size);
    std::vector<int> test_labels(test_size);
    for (int i = 0; i < test_size; ++i) {
        test_data[i] = all_data[test_indx[i]];
        test_labels[i] = all_labels[test_indx[i]];
    }
    
    //Convert Hand_Landmarks vectors to cv::Mat for KNearest training
    cv::Mat train_data_cvMat(train_size, 63, CV_32F);
    cv::Mat train_labels_cvMat(train_size,1, CV_32F);
    for (int i = 0; i < train_size; ++i) {
        cv::Mat matRow = train_data[i].toMatRow();
        matRow.copyTo(train_data_cvMat.row(i));
        train_labels_cvMat.at<int>(i, 0) = train_labels[i];
    }
    
    //Conver test Hand_Landmarks
    cv::Mat test_data_cvMat(test_size, 63, CV_32F);
    cv::Mat test_labels_cvMat(test_size, 1, CV_32S);
    for (int i = 0; i < test_size; ++i) {
        cv::Mat matRow = test_data[i].toMatRow();
        matRow.copyTo(test_data_cvMat.row(i));
        test_labels_cvMat.at<int>(i, 0) = test_labels[i];
    }

    //Create KNN model within cv::ml::KNearest
    cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();
    // Set KNN parameters if we want to adjust the default K=32
    /*
    knn->setDefaultK(K_VALUE);
    knn->setIsClassifier(true); //+other parameters
    */

    //Train the KNN model
    knn->train(train_data_cvMat, cv::ml::ROW_SAMPLE, train_labels_cvMat);

    //evaluate the model on the testing set
    cv::Mat eval;
    knn->findNearest(test_data_cvMat, 1, eval);

    //check the evaluation results of test data, for now using the simplest Accuracy
    //can also use confusion Matrix(?)
    cv::Mat correctLabels = (eval == test_labels_cvMat);
    double accuracy = cv::countNonZero(correctLabels) / static_cast<double>(test_labels_cvMat.rows);
    if(accuracy>90) {
        return knn;
    }
    else {
        //do data augmentation and run the model again
    }
    return knn;
}

