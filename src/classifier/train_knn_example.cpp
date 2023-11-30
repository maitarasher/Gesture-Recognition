/*
example I used to test the model, returns a working KNN


#include <stdio.h>
#include <iostream>
#include <vector>
#include <complex>
#include <chrono>
#include <random>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <numeric>

using namespace std;

class Landmark {
public:
    float x;
    float y;
    float z;
Landmark(): x(0.0), y(0.0), z(0.0) {}
Landmark(float x, float y, float z) : x(x), y(y), z(z) {}
Landmark(vector<float>& point) {
        if (point.size() == 3) {
            x = point[0];
            y = point[1];
            z = point[2];
        } else {
            // Handle incorrect vector size, set default values
            x = 0.0;
            y = 0.0;
            z=0.0;
            cerr << "Warning: Invalid vector size for Landmark constructor. Default values used." << std::endl;
        }
    }

    // Function to print the coordinates
    void print()  {
        cout << "(" << x << ", " << y << ")" << endl;
    }

    //don't think we need it, convert to regular vector
    vector<float> toVector() {
        vector<float> landmark_vector;
        
        landmark_vector.push_back(x);
        landmark_vector.push_back(y);
        landmark_vector.push_back(z);
        return landmark_vector;   

    }
    // Function to convert Landmark to a cv::row vector
    cv::Mat toMatRow() const {
        cv::Mat row(1, 3, CV_32F);
        row.at<float>(0, 0) = x;
        row.at<float>(0, 1) = y;
        row.at<float>(0, 2) = z;
        return row;
    }

};

class Hand_Landmarks {
    public:
    vector<Landmark> landmarks;
    //int handedness;
    
    // Constructor to initialize the vector with 21 default Landmark objects
    Hand_Landmarks() : landmarks(21, Landmark()) {}
    // Constructor to initialize the vector with specific Landmark objects
    Hand_Landmarks(const vector<Landmark>& initialLandmarks) : landmarks(initialLandmarks) {}

    // Print function to display the contents of Hand_Landmarks
    void print() const {
        for (const Landmark& landmark : landmarks) {
            std::cout << "(" << landmark.x << ", " << landmark.y << ", " << landmark.z << ")" << std::endl;
        }
    }

    cv::Mat toMatRow() const {
        cv::Mat matRow(1, 63, CV_32F);
        int colIndex = 0;

        for (const Landmark& landmark : landmarks) {
            cv::Mat landmarkRow = landmark.toMatRow();
            landmarkRow.copyTo(matRow(cv::Rect(colIndex, 0, 3, 1)));
            colIndex += 3;
        }

        return matRow;
    }
};

cv::Ptr<cv::ml::KNearest> KNN_build(const std::vector<Hand_Landmarks>& all_data, const vector<float>& all_labels) {
//float KNN_build(const std::vector<Hand_Landmarks>& all_data, const vector<int>& all_labels) {
    // Split the data into training and testing sets
    float train_percentage = 0.8;
    int total_size = static_cast<int>(all_data.size());
    int train_size = static_cast<int>(train_percentage * total_size); 
    int test_size = static_cast<int>(total_size-train_size);


    
    // Shuffle the indices to randomly select samples for training 
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
    std::vector<Hand_Landmarks> train_data(train_size);
    std::vector<float> train_labels(train_size);
    for (int i = 0; i < train_size; ++i) {
        train_data[i] = all_data[train_indx[i]];
        train_labels[i] = all_labels[train_indx[i]];
    }

    
    //Create test dataset
    std::vector<Hand_Landmarks> test_data(test_size);
    std::vector<float> test_labels(test_size);
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
        train_labels_cvMat.at<float>(i, 0) = train_labels[i];
    }
    
    //Conver test Hand_Landmarks
    cv::Mat test_data_cvMat(test_size, 63, CV_32F);
    cv::Mat test_labels_cvMat(test_size, 1, CV_32F);
    for (int i = 0; i < test_size; ++i) {
        cv::Mat matRow = test_data[i].toMatRow();
        matRow.copyTo(test_data_cvMat.row(i));
        test_labels_cvMat.at<float>(i, 0) = test_labels[i];
    }
    
    //Create KNN model within cv::ml::KNearest
    cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();
    // Set KNN parameters if we want to adjust the default K=32
    
    knn->setDefaultK(5);
    knn->setIsClassifier(true); //+other parameters
    
    

    //Train the KNN model
    knn->train(train_data_cvMat, cv::ml::ROW_SAMPLE, train_labels_cvMat);

    //evaluate the model on the testing set
    cv::Mat eval;
    knn->findNearest(test_data_cvMat, 5, eval);

 
    //check the evaluation results of test data, for now using the simplest Accuracy
    //can also use confusion Matrix(?)
    cv::Mat correctLabels = (eval == test_labels_cvMat);
    double accuracy = cv::countNonZero(correctLabels) / static_cast<double>(test_labels_cvMat.rows);
    if(accuracy>0.9) {
        cout<<"accuracy is high"<<endl;
    }
    return knn;
    //return accuracy;
}

//using namespace cv;
int main() {
    std::cout<<"Hello!"<<std::endl;
    Landmark dummy;
    Landmark point(1,2,3);
    dummy.print();
    point.print();
    cv::Mat data;
    // Create a Hand_Landmarks object with 21 default Landmarks
    Hand_Landmarks defaultHandLandmarks;
    //defaultHandLandmarks.print();
    std::vector<Landmark> initialLandmarks(21, Landmark(5.0, 3.0, 0));
    

    // Create a Hand_Landmarks object with specific initial Landmarks
    Hand_Landmarks customHandLandmarks(initialLandmarks);
    //create allData vector
    vector<Hand_Landmarks> all_data(50, customHandLandmarks);
    //create labels vector
    vector<float> labels(50,0);

    //add another class
    std::vector<Landmark> initialLandmarks2(21, Landmark(-1.0, -3.0, 0));
    Hand_Landmarks customHandLandmarks2(initialLandmarks2);
    for(int i=0; i<50;i++) {
    all_data.push_back(customHandLandmarks2);
    labels.push_back(1);
    }
    std::cout<<all_data.size()<<" size "<<labels.size()<<endl;


    //test knn model
    cv::Ptr<cv::ml::KNearest> knn = KNN_build(all_data,labels);
    //float accuracy = KNN_build(all_data,labels);
    //cout<<accuracy<<endl;

    //check a new example
    cv::Mat ret,result,neighbor,dist;
    //make a new example
    vector<Landmark> sample(21, Landmark(10,33,0));
    Hand_Landmarks new_sample(sample);
    //convert to cvMat
    cv::Mat test_data_cvMat(1, 63, CV_32F);
    test_data_cvMat = new_sample.toMatRow();
    knn->findNearest(test_data_cvMat, 3,result);

    int predict = result.at<float>(0, 0); 
    cout<<"predicted class: "<<predict<<endl;

    return 0;
}
*/