#pragma once
#include <vector>
#include <filesystem>
#include <iostream>
#include <unordered_map>
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
    if (!file.is_open()) {
        std::cerr << "Error file not found " << filename << std::endl;
        //return labelStringMap; (This variable is not declared(?))
    }

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

    if (!file.is_open()) {
        std::cerr << "Error file not found " << filename << std::endl;
        //return labelStringMap; Not Declared
    }
    std::string line;

    while (std::getline(file, line)) {
        labels.push_back(std::stof(line));
    }
    return labels;
}

//CHECK saveMaptoCSV
void saveMapToCSV(const std::unordered_map<float, std::string>& stringLabelMap, const std::string& filename) {
    std::ofstream file(filename);   
    //check the file is open
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& pair : stringLabelMap) {
        file << pair.first << ","<<pair.second<<"\n";
    }
    file.close();
}

//ReadMapfromCSV
std::unordered_map<float,std::string> readMapFromCSV(const std::string& filename) {
    std::unordered_map<float, std::string> stringLabelMap;
    std::ifstream file(filename);
    //check that the file is open
    if (!file.is_open()) {
        std::cerr << "Error file not found " << filename << std::endl;
        //return labelStringMap; (Not declared yet)
    }
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float label_key;
        std::string value;
        
        if(std::getline(iss,value, ',')) {
            try {
                float key = std::stof(value);
                if (std::getline(iss,value, ',')) {
                    stringLabelMap[key] = value;
                }
            }
            catch (const std::invalid_argument& e) {
                std::cerr <<"Invalid csv format f or label: "<<value<<std::endl;
            }
        }
    }
    return stringLabelMap;
}
//don't we need to close all files before returning ?
