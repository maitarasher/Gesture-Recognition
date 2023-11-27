#include <iostream>
#include "base_loader.hpp"
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <filesystem>
#include <string>
#include <cassert>
#include <complex>
#include <algorithm>

const int K_VALUE = 5;

using namespace std;


int build_Classifier(string folder_path) {
    //vector<cv::Mat> imgs = loadImgsFromFolder(folder_path) //function from base_loader
    //list or vectors of classes?
    vector<GestClass> classifier;
    int i =0;
    for (const auto &entry : fs::directory_iterator(folder_path))
    {
            string file_path = entry.path().string();
            string file_name = entry.path().filename().string();
            // Read the image
            cv::Mat image = cv::imread(filePath);

            // check if the image was loaded
            if(image.empty())
            {
                cerr << "Error laoding image: " << filePath << endl;
            }
            //build a class for each image
            GestClass class_obj;
            class_obj.number = i;
            class_obj.name = file_name;
            class_obj.img = image;
            class_obj.landmarks = getlandmarks(image);
            classifier.push_back(class_obj);
            i++
    
    }
    return 1;
}
 
//in case we want to implement the function ourselves
int train_KNN(vector<GestClass> classifier)
{
    Gest_KNN classifier_KNN;
    classifier_KNN.K_value =K_VALUE;
    //perform training and data_augmentation for each class
    for (const auto &class : classifier) 
    {
        //create pipelie for augmentation
        Pipeline pipe;
        pipeline.add_stge();



    }
}

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

