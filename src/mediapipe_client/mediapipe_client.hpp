#ifndef MEDIAPIPE_CLIENT_H
#define MEDIAPIPE_CLIENT_H

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <regex>
#include <opencv2/opencv.hpp>
#include "../data_classes.hpp"

// Function to connect to the server
int connectToServer(const int PORT, const char* SERVER_IP);

// Function to parse landmarks from a string
Hand_Landmarks parseLandmarks(const std::string& input);

// Function to get landmarks from the server
bool getLandmarksFromServer(int clientSocket, const cv::Mat& inputImage, std::vector<Hand_Landmarks>& landmarks);

#endif // NETWORK_FUNCTIONS_H