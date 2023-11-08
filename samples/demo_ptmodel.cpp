#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>


// using pretrained model 

int main() {

    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }

    // get the full path 
    cv::String palm_model = "../samples/models/open_palm.xml"; 
    cv::String fist_model = "../samples/models/closed_palm.xml"; 
    
    cv::CascadeClassifier palmCascade;
	cv::CascadeClassifier fistCascade;
	palmCascade.load(palm_model);
	fistCascade.load(fist_model);

    if (!palmCascade.load(palm_model)) {
        std::cerr << "Error: Could not load the palm detection model." << std::endl;
        return -1;
    }

     if (!fistCascade.load(fist_model)) {
        std::cerr << "Error: Could not load the palm detection model." << std::endl;
        return -1;
    }


    while (true) {
        cv::Mat frame;
        cap >> frame;  // Capture a frame from the camera

        if (frame.empty()) {
            std::cerr << "Error: Empty frame." << std::endl;
            break;
        }

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, gray);

        std::vector<cv::Rect> palms;
        palmCascade.detectMultiScale(gray, palms, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(50, 50));

        std::vector<cv::Rect> fists;
        fistCascade.detectMultiScale(gray, fists, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(50, 50));

        // Filter detected palms and fists by size
        const int minPalmSize = 200; // Tweak this value
        const int minFistSize = 200; // Tweak this value

        std::vector<cv::Rect> filteredPalms;
        std::vector<cv::Rect> filteredFists;

        for (const cv::Rect& palm : palms) {
            if (palm.area() >= minPalmSize) {
                filteredPalms.push_back(palm);
            }
        }

        for (const cv::Rect& fist : fists) {
            if (fist.area() >= minFistSize) {
                filteredFists.push_back(fist);
            }
        }

         std::vector<cv::Rect> finalPalms;
        std::vector<cv::Rect> finalFists;

        for (const cv::Rect& palm : filteredPalms) {
            bool isOverlap = false;
            for (const cv::Rect& other : finalPalms) {
                if ((palm & other).area() > 0) {
                    isOverlap = true;
                    break;
                }
            }
            if (!isOverlap) {
                finalPalms.push_back(palm);
            }
        }

        for (const cv::Rect& fist : filteredFists) {
            bool isOverlap = false;
            for (const cv::Rect& other : finalFists) {
                if ((fist & other).area() > 0) {
                    isOverlap = true;
                    break;
                }
            }
            if (!isOverlap) {
                finalFists.push_back(fist);
            }
        }


        for (const cv::Rect& palm : palms) {
            cv::rectangle(frame, palm, cv::Scalar(0, 255, 0), 2);
        }

        for (const cv::Rect& fist : fists) {
            cv::rectangle(frame, fist, cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Gesture Recognition", frame);

        char c = (char)cv::waitKey(10);
        if (c == 27)  // ESC key to exit
            break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
