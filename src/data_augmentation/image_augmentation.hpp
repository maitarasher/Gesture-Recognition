#ifndef IMAGE_AUGMENTATION_HPP
#define IMAGE_AUGMENTATION_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/*
The paper proposed the use of image augmentations in an attempt
to increase the detection rates of MediaPipe Hands.
*/

/*
- increaseBrightness Function
  cv::Mat increaseBrightness(const cv::Mat& image, double value = 30)
  Description: Increases the brightness of the input image.
  Parameters:
    image: Input image.
    value: Degree of brightness adjustment (default: 30).
  Return Type: cv::Mat
  Why: Modifies the brightness of the image in HSV color space, ensuring that values do not exceed the valid range.
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

/*
- rotate Function
  cv::Mat rotate(const cv::Mat& image, double degrees = 0)
  Description: Rotates the input image by the specified degrees.
  Parameters:
    image: Input image.
    degrees: Degree of rotation (default: 0).
  Return Type: cv::Mat
  Why: Applies an affine transformation to rotate the image, preserving lines and parallelism.
*/

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
