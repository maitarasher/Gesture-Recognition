#ifndef IMAGE_AUGMENTATION_HPP
#define IMAGE_AUGMENTATION_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/*
The paper proposed the use of image augmentations in an attempt
to increase the detection rates of MediaPipe Hands.

in python git it it under preprocessing/images.py
*/

cv::Mat increaseBrightness(const cv::Mat& image, double value = 30) {
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV); // convert RGB to HSV

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels); // channels[0] = h,  channels[1] = s, channels[2] = v

    channels[2] = channels[2] + value; //v = cv.add(v, value)
    cv::threshold(channels[2], channels[2], 255, 255, cv::THRESH_TRUNC); //v[v > 255] = 255
    cv::threshold(channels[2], channels[2], 0, 0, cv::THRESH_TOZERO); //v[v < 0] = 0

    cv::merge(channels, hsv);

    cv::Mat result;
    cv::cvtColor(hsv, result, cv::COLOR_HSV2BGR); // convert HSV to RGB

    return result;
}

cv::Mat rotate(const cv::Mat& image, double degrees = 0){
    // credit: https://cppsecrets.com/users/204211510411798104971091085153504964103109971051084699111109/C00-OpenCV-to-rotate-an-image.php
    cv::Mat result;
    int h = image.rows / 2; //getting mid point of rows
    int w = image.cols / 2; //getting mid point of height
    cv::Mat r = cv::getRotationMatrix2D(cv::Point(w, h), degrees, 1.0);
    cv::warpAffine(image, result, r, cv::Size(image.cols, image.rows));  ///applie an affine transforation to image (an affine transformation or affinity is a geometric transformation that preserves lines and parallelism)
    return result;
}

#endif // IMAGE_AUGMENTATION_HPP

/*
FOR TESTING change file to .cpp, comment defs and add main
int main(){
    // Read an image
    cv::Mat image = cv::imread("/Users/maitarasher/Downloads/image.jpg");
    if (image.empty()) {
    std::cerr << "Error: Could not read the image file." << std::endl;
    return -1;
    }

    // Increase brightness
    cv::Mat brightenedImage = increaseBrightness(image, 30);

    // Rotate by 45 degrees
    cv::Mat rotatedImage = rotate(image, 30);

    // Display the images (requires user to press a key to close the windows)
    cv::imshow("Original Image", image);
    cv::imshow("Brightened Image", brightenedImage);
    cv::imshow("Rotated Image", rotatedImage);
    cv::waitKey(0);
    return 0;
}
*/
