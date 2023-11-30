#pragma once
#include <vector>
#include <filesystem>
#include <iostream>
#include "../data_classes.hpp"
#include <fstream>

void saveToCSV(const std::vector<Hand_Landmarks>& landmarksVector, const std::string& filename) {
    std::ofstream file(filename);

    for (const auto& handLandmarks : landmarksVector) {
        for (const auto& landmark : handLandmarks.landmarks) {
            file << landmark.x << "," << landmark.y << "," << landmark.z << ";";
        }
        file << "\n";
    }
    file.close();
}

std::vector<Hand_Landmarks> readFromCSV(const std::string& filename) {
    std::vector<Hand_Landmarks> landmarksVector;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string landmarkData;
        std::vector<Landmark> landmarks;

        while (std::getline(ss, landmarkData, ';')) {
            std::stringstream ssLandmark(landmarkData);
            std::string coordinate;
            std::vector<float> coordinates;

            while (std::getline(ssLandmark, coordinate, ',')) {
                coordinates.push_back(std::stof(coordinate));
            }

            if (coordinates.size() == 3) {
                landmarks.emplace_back(coordinates[0], coordinates[1], coordinates[2]);
            }
        }

        landmarksVector.emplace_back(landmarks);
    }
    return landmarksVector;
}

void saveLabelsToCSV(const std::vector<float>& labels, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& label : labels) {
        file << label << "\n";
    }
    file.close();
}


std::vector<float> readLabelsFromCSV(const std::string& filename) {
    std::vector<float> labels;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        labels.push_back(std::stof(line));
    }
    return labels;
}

