#include "./pipeline.hpp"
#include "./stage.hpp"

int main(){
    try {
        // Create a pipeline with 2 hands
        Pipeline my_pipeline(2);

        // Add stages to the pipeline
        my_pipeline.add_stage(30.0, 45.0);
        my_pipeline.add_stage(15.0, 90.0);

        // Add more stages as needed...

        // Create an image for testing
        cv::Mat image = cv::imread("/Users/maitarasher/Downloads/image.jpg");
        if (image.empty()) {
            std::cerr << "Error: Could not read the image file." << std::endl;
            return -1;
        }

        // Iterate through the stages in the pipeline and process the image with each stage
        for (const Stage &stage : my_pipeline.getStages()) {
            cv::Mat processedImage = stage.process(image);

            // Show the original and processed images for each stage
            cv::imshow("Original Image", image);
            cv::imshow("Brightened Image", processedImage);
            cv::waitKey(1000);
        }

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
}

/*
To TEST RUN: (this pkg-config --cflags --libs opencv4 provides the linker flags)
g++ -std=c++20 -I/opt/homebrew/opt/opencv/include/opencv4 -L/opt/homebrew/opt/opencv/lib -lopencv_gapi -lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_wechat_qrcode -lopencv_xfeatures2d -lopencv_shape -lopencv_ml -lopencv_ximgproc -lopencv_video -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_dnn -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core test.cpp
*/
