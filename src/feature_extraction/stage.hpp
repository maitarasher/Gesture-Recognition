#ifndef STAGE_HPP
#define STAGE_HPP

#include <opencv2/opencv.hpp>
#include "../data_augmentation/image_augmentation.hpp"

/*
Type: class Stage
Description: Represents an individual image augmentation stage.
Functions:

- Stage(int initial_index, double brightness, double rotation): Constructor, initializes the stage with an initial index, brightness, and rotation.
  Type: Constructor
  Parameters:
    initial_index: Index of the stage.
    brightness: Degree of brightness adjustment.
    rotation: Degree of image rotation.
  Why: Sets up the stage with specified parameters.

- cv::Mat process(const cv::Mat& image) const: Processes the input image through brightness adjustment and rotation.
  Type: Member Function
  Parameters:
    image: Input image to be processed.
    Return Type: cv::Mat
  Why: Applies the defined augmentation operations to the input image and returns the processed image.
*/

class Stage{
public:
    Stage(int initial_index, double brightness, double rotation)
    :initial_index(initial_index), brightness(brightness), rotation(rotation){};

    cv::Mat process(const cv::Mat& image) const{
        cv::Mat processedImage = increaseBrightness(image, brightness);
        processedImage = rotate(processedImage, rotation);
        return processedImage;
    }

private:
    int initial_index;
    double brightness;
    double rotation;
};

#endif // STAGE_HPP
