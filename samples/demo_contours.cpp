#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    // open camera feed
    VideoCapture capture(0);  // Open the default camera
    if (!capture.isOpened()) {
        return -1;
    }

    // make a window and variable to store frame
    Mat frame;
    namedWindow("Hand Detection", WINDOW_AUTOSIZE);

    // background subtraction
   Ptr<BackgroundSubtractor> bgSubtractor = createBackgroundSubtractorMOG2();

    while (true) {
        capture >> frame;  // capture a frame
        if (frame.empty()) {
            break;
        }

        // apply background subtraction
        Mat fgMask;
        bgSubtractor->apply(frame, fgMask);

        // BGR -> HSV
        Mat hsvFrame;
        cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

        // tracking hand using the result
        bitwise_and(hsvFrame, hsvFrame, hsvFrame, fgMask);

        // tracking hand
        Mat binary;
        inRange(hsvFrame,Scalar(96, 23, 123), Scalar(256, 100, 256), binary);

        // noise reduction
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
        morphologyEx(binary, binary, MORPH_OPEN, kernel);

        // find contours
        std::vector<std::vector<cv::Point>> contours;
        findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // loop through contours and find hand
        for (const auto& contour : contours) {
            double area = contourArea(contour);

            // Add any additional criteria to identify the hand
            if (area > 8000) {
                Rect boundingBox = boundingRect(contour);
                rectangle(frame, boundingBox, Scalar(0, 0, 255), 2);
            }
        }

        // hit esc button to exit
        imshow("Hand Detection", frame);
        if (waitKey(10) == 27) {
            break;
        }
    }

    capture.release();
    destroyAllWindows();
    return 0;

}
