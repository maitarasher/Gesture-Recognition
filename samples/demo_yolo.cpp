#include <iostream>
#include <filesystem>
#include <string>
#include <yaml-cpp/yaml.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

using namespace std;
using namespace cv;

struct Detection
{
    int class_id{0};
    std::string className{};
    float confidence{0.0};
    cv::Scalar color{};
    cv::Rect box{};
};


// create .yaml file for training data
string create_input_config(const string& input_path, const string& train_path, const string& val_path, const vector<string>& classes) {
    YAML::Node config;

    config["path"] = input_path;
    config["train"] = train_path;
    config["val"] = val_path;

    YAML::Node class_names;
    for (int i = 0; i < classes.size(); i++) {
        class_names[i] = classes[i]
    }
    config["names"] = class_names;

    string config_filename = "data.yaml";
    ofstream fout(config_filename);
    fout << config;

    return config_filename;
}


dnn::Net get_trained_model(const string& data_config_path) {

    // train model and export into an onnx file that we can work with
    string train_command = "yolo detect train data=" + data_config_path + " model=yolov8n.pt epochs=2 imgsz=640";
    string export_command = "yolo export model=yolov8n.pt format=onnx";

    system(train_command.c_str());
    system(export_command.c_str());

    // read the onnx file
    dnn::Net net = dnn::readNetFromONNX('yolov8n.onnx')

    // for CPUs
    net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(dnn::DNN_TARGET_CPU);

    return net;
}

vector<Detection> get_prediction(const Mat &input, const Size &modelShape) {
    Mat blob;
    dnn::blobFromImage(input, blob, 1.0/255.0, modelShape, cv::Scalar(), true, false);
    net.setInput(blob);

    vector<Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());



}

Mat formatImageToSquare(const Mat &img) {
    int max = MAX(img.cols, img.rows);
    Mat result = Mat::zeros(max, max, CV_8UC3);
    img.copyTo(result(cv::Rect(0, 0, img.cols, img.rows)));
    return result;
}

// using pretrained model 
int main() {
    
    return 0;
}
