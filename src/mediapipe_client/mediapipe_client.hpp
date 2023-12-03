/**
 * @file mediapipe_client.hpp
 * @author Elifia Muthia (em3308@barnard.edu)
 * @brief Mediapipe Client Interface
 * @version 0.1
 * @date 2023-12-03
 *
 * @addtogroup Gesture-Recognition
 * @{
 */
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

/**
 * @brief Connects client to Mediapipe server
 *
 * @param PORT port number the server is connected to
 * @param SERVER_IP server IP address to connect to
 * @return client socket if successful, -1 if unsuccessful
 */
int connectToServer(const int PORT, const char* SERVER_IP);

/**
 * @brief Helper function for getLandmarksFromServer(), 
 * parses landmarks as a string from server into a Hand_Landmarks struct
 *
 * @param input string containing landmarks from server
 * @return landmarks parsed as a Hand_Landmarks struct
 */
Hand_Landmarks parseLandmarks(const std::string& input);

/**
 * @brief Sends an image to the server, receives landmarks as a response
 *
 * @param clientSocket to communicate with the server
 * @param inputImage to be processed
 * @param landmarks to be returned, a vector of Hand_Landmarks found in the inputImage
 * @return true if obtaining landmarks was successful, false if an error occurred
 */
bool getLandmarksFromServer(int clientSocket, const cv::Mat& inputImage, std::vector<Hand_Landmarks>& landmarks);

#endif