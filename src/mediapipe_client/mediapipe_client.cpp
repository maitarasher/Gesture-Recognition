#include "./mediapipe_client.hpp"

int connectToServer(const int PORT, const char* SERVER_IP) {
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
    return clientSocket;
}

Hand_Landmarks parseLandmarks(const std::string& input) {
    std::string test_input = input;
    test_input.erase(std::remove_if(test_input.begin(), test_input.end(), [](unsigned char c) {
        return std::isspace(c) || !std::isprint(c);
    }), test_input.end());

    std::regex landmarkRegex(R"regex(x:\s*([\d.-]+)\s*y:\s*([\d.-]+)\s*z:\s*([\d.-]+))regex");
    std::smatch match;
    Hand_Landmarks landmarks;

    auto it = test_input.cbegin();
    while (std::regex_search(it, test_input.cend(), match, landmarkRegex)) {
        double x = std::stod(match[1].str());
        double y = std::stod(match[2].str());
        double z = std::stod(match[3].str());

        landmarks.landmarks.push_back(Landmark(x, y, z));
        it = match.suffix().first;
    }

    return landmarks;
}

bool getLandmarksFromServer(int clientSocket, const cv::Mat& inputImage, std::vector<Hand_Landmarks>& landmarks) {
    // Encode the image to JPEG format
    std::vector<uchar> imageData;
    cv::imencode(".jpg", inputImage, imageData);

    size_t imageSize = imageData.size();
    ssize_t sizeSent = send(clientSocket, &imageSize, sizeof(size_t), 0);

    if (sizeSent != sizeof(size_t)) {
        std::cerr << "Error sending image size" << std::endl;
        close(clientSocket);
        return false;
    }

    // Send the image data
    ssize_t bytesSent = send(clientSocket, imageData.data(), imageSize, 0);

    if (bytesSent < 0) {
        std::cerr << "Error sending image data" << std::endl;
        close(clientSocket);
        return false;
    }

    // std::cout << "Image sent!" << std::endl;

    // Receive the size of the landmarks data
    size_t landmarksSize;
    ssize_t sizeReceived = recv(clientSocket, &landmarksSize, sizeof(size_t), 0);

    if (sizeReceived != sizeof(size_t)) {
        std::cerr << "Error receiving landmarks size" << std::endl;
        close(clientSocket);
        return false;
    }

    // Receive the concatenated landmarks string
    std::vector<char> landmarksBuffer;
    landmarksBuffer.resize(landmarksSize);
    size_t totalBytesReceived = 0;

    while (totalBytesReceived < landmarksSize) {
        ssize_t bytesRead = recv(clientSocket, landmarksBuffer.data() + totalBytesReceived, landmarksSize - totalBytesReceived, 0);

        if (bytesRead <= 0) {
            std::cerr << "Error receiving landmarks data" << std::endl;
            close(clientSocket);
            return false;
        }

        totalBytesReceived += bytesRead;
    }

    // Convert the received data to a string
    std::string receivedData(landmarksBuffer.data(), landmarksBuffer.size());

    // Split the concatenated string into individual landmark strings using the custom delimiter
    size_t delimiterPos = receivedData.find("##LANDMARK_DELIMITER##");
    while (delimiterPos != std::string::npos) {
        std::string landmarkString = receivedData.substr(0, delimiterPos);

        // Process each individual landmark string
        Hand_Landmarks landmarks_parsed = parseLandmarks(landmarkString);
        landmarks.emplace_back(landmarks_parsed);

        // Move to the next delimiter
        receivedData.erase(0, delimiterPos + 20);  // Assuming "##LANDMARK_DELIMITER##" has a length of 20
        delimiterPos = receivedData.find("##LANDMARK_DELIMITER##");
    }

    // std::cout << "Number of Landmarks: " << landmarks.size();

    return true;
}