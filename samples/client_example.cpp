#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <arpa/inet.h>

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1"; 

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr));

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting to the server" << std::endl;
        close(clientSocket);
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    // Load an example image (replace with your image loading code)
    cv::Mat inputImage = cv::imread("/Users/elifiamuthia/Desktop/test-image.jpg");
    if (inputImage.empty()) {
        std::cerr << "Error loading image" << std::endl;
        close(clientSocket);
        return -1;
    }

    // // Encode the image to JPEG format
    // std::vector<uchar> imageData;
    // cv::imencode(".jpg", inputImage, imageData);

    // Send image data to the server
    // inputImage = (inputImage.reshape(0,1));
    int imgSize = inputImage.total()*inputImage.elemSize();

    ssize_t bytesSent = send(clientSocket, inputImage.data, imgSize, 0);
    if (bytesSent < 0) {
        std::cerr << "Error sending image data" << std::endl;
        close(clientSocket);
        return -1;
    }

    std::cout << "Image sent!" << std::endl;

    // Receive landmark data from the server
    const size_t bufferSize = 1024; // Adjust as needed
    std::vector<char> landmarkBuffer(bufferSize);
    ssize_t bytesRead = recv(clientSocket, landmarkBuffer.data(), bufferSize, 0);
    if (bytesRead < 0) {
        std::cerr << "Error receiving landmark data" << std::endl;
        close(clientSocket);
        return -1;
    }

    // Process the received landmark data (replace with your processing code)
    std::string receivedLandmarkData(landmarkBuffer.data(), bytesRead);
    std::cout << "Received Landmark Data:\n" << receivedLandmarkData << std::endl;

    close(clientSocket);

    return 0;
}
