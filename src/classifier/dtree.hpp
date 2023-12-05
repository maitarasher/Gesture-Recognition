/**
 * @file dtree.hpp
 * @brief  Decision trees model for Gesture Recognition
 * @version 0.1
 * @date 2023-12-01
 *
 */


#pragma once
#include <vector>
#include <string>
#include <numeric>
#include <chrono>
#include <complex>
#include <algorithm>
#include <random>
#include <opencv2/opencv.hpp>
#include <opencv2/ml/ml.hpp>
using namespace std;



/**
 * @brief Creates and trains Dtree model for Gesture Recognition by taking in all_data, all_labels, num_classes.
 * The function separates the data into training and testing sets (80/20) and returns Ptr<cv::ml::DTree> and float accuracy of the model.
 * The model is trained with cv::ml::Dtree->train. To learn more about cv::ml::DTree follow https://docs.opencv.org/4.x/d8/d89/classcv_1_1ml_1_1DTrees.html
 *
 * @param all_data  A vector of <Hand_Landmarks> that contains all the data from csv files.
 * @param all_labels A vector of <float> of corresponding labels.
 * @param num_classes Number of unique labels in data
 * @return 
 */

tuple<cv::Ptr<cv::ml::DTrees>, float> DTree_build(const std::vector<Hand_Landmarks>& all_data, const vector<float>& all_labels, int num_classes) {

    // Split the data into training and testing sets
    float train_percentage = 0.8;
    int total_size = static_cast<int>(all_data.size());
    int train_size = static_cast<int>(train_percentage * total_size); 
    int test_size = static_cast<int>(total_size-train_size);

    /// Shuffle the indices to randomly select samples for training 
    std::vector<int> indx(total_size);
    //fill with range from 0 to total_size-1
    std::iota(indx.begin(), indx.end(), 0); 
    // Use a random number generator engine
    unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    // Shuffle the vector
    shuffle(indx.begin(), indx.end(), gen);

    // Separate the indices for training and testing
    std::vector<int> train_indx(indx.begin(), indx.begin() + train_size);
    std::vector<int> test_indx(indx.begin() + train_size, indx.end());
    
    // Create training  dataset
    std::vector<Hand_Landmarks> train_data;
    std::vector<float> train_labels(train_size);
    for (int i = 0; i < train_size; ++i) {
        train_data.push_back(all_data[train_indx[i]]);
        train_labels[i] = all_labels[train_indx[i]];
    }

    //Create test dataset
    std::vector<Hand_Landmarks> test_data;
    std::vector<float> test_labels(test_size);
    for (int i = 0; i < test_size; ++i) {
        test_data.push_back(all_data[test_indx[i]]);
        test_labels[i] = all_labels[test_indx[i]];
    }
    
    //Convert Hand_Landmarks vectors to cv::Mat 
    cv::Mat train_data_cvMat(train_size, 63, CV_32F);
    cv::Mat train_labels_cvMat(train_size,1, CV_32S);
    for (int i = 0; i < train_size; ++i) {
        cv::Mat matRow = train_data[i].toMatRow();
        matRow.copyTo(train_data_cvMat.row(i));
        train_labels_cvMat.at<int>(i, 0) = train_labels[i];
    }
    
    //Convert test Hand_Landmarks
    cv::Mat test_data_cvMat(test_size, 63, CV_32F);
    cv::Mat test_labels_cvMat(test_size, 1, CV_32S);
    for (int i = 0; i < test_size; ++i) {
        cv::Mat matRow = test_data[i].toMatRow();
        matRow.copyTo(test_data_cvMat.row(i));
        test_labels_cvMat.at<int>(i, 0) = test_labels[i];
    }

    //Create Dtree model within cv::ml::DTrees default parameters
    cv::Ptr<cv::ml::DTrees> dtree = cv::ml::DTrees::create();
    cv::Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(train_data_cvMat, cv::ml::ROW_SAMPLE, train_labels_cvMat);
    dtree->train(trainData);

    //specify parameters yourself when known
    /*
    dtree->setCVFolds(1);  // Number of cross-validation folds
    dtree->setMaxDepth(10);  // Maximum depth of the tree
    dtree->setMinSampleCount(1);  // Minimum samples in a leaf node
    dtree->train(trainData);
    */

    cv::Mat eval;
    dtree->predict(test_data_cvMat,eval);
    eval.convertTo(eval,CV_32S);
    cv::Mat correctLabels = (eval == test_labels_cvMat);
    float accuracy = 100*(cv::countNonZero(correctLabels) / static_cast<float>(test_labels_cvMat.rows));


    if(accuracy>90) {
        cout<<"high accuracy"<<endl;
    }
    
    return make_tuple(dtree,accuracy);
}