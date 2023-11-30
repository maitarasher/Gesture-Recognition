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


using namespace std;
int main () {

//get data from json file 

//convert to landmarks vector<Hand_Landmarks> all_data, vector<float> all_labels

//call cv::Ptr<cv::ml::KNearest> KNN_build(const std::vector<Hand_Landmarks>& all_data, const vector<float>& all_labels)

//save the model
//knn->save("knn_model.xml");

//open a different file and load
//cv::Ptr<cv::ml::KNearest> loaded_knn = cv::ml::KNearest::load("knn_model.xml");

//use for new predictions , K is a parameter fo rnumber of neighbors
//loaded_knn->findNearest(test_data_cvMat, K, predict);

//the label will be in int predict = result.at<float>(i, 0); 
// cout<<"predicted class: "<<predict<<endl;
}