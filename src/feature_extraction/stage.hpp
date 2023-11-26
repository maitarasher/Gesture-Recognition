#ifndef STAGE_HPP
#define STAGE_HPP

#include <opencv2/opencv.hpp>
#include "../data_augmentation/image_augmentation.hpp"

class Stage{
public:
    Stage(int initial_index, double brightness, double rotation)
    :initial_index(initial_index), brightness(brightness), rotation(rotation){};

    cv::Mat process(const cv::Mat& image) const{
        cv::Mat processedImage = increaseBrightness(image, brightness);
        processedImage = rotate(processedImage, rotation);
        return processedImage;
    }

    //void getLandmarks(const cv::Mat& image);

    //void getWorldLandmarks(const cv::Mat& image);

private:
    int initial_index;
    double brightness;
    double rotation;
    // need to add MediaPipe object
    //int recognized_counter;
    //void* last_detected_hands;
    //void* last_detected_handedness;
};

#endif // STAGE_HPP

/*
FOR TESTING change file to .cpp, comment defs and add main
int main(){
    // Example usage
    cv::Mat image = cv::imread("/Users/maitarasher/Downloads/image.jpg");
    if (image.empty()){
    std::cerr << "Error: Could not read the image file." << std::endl;
    return -1;
    }

    Stage stage(0, 30.0, 45.0);
    cv::Mat processedImage = stage.process(image);

    cv::imshow("Original Image", image);
    cv::imshow("Brightened Image", processedImage);
    cv::waitKey(0);
    return 0;
}
*/
