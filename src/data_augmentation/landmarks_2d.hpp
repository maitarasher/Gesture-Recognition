#ifndef LANDMARKS_2D_HPP
#define LANDMARKS_2D_HPP

#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>

struct Point{
    double x, y;
};

Point rotate(const Point& point, double angle){
    // Rotate a point counterclockwise
    double px = point.x;
    double py = point.y;

    double qx = std::cos(angle) * px - std::sin(angle) * py;
    double qy = std::sin(angle) * px + std::cos(angle) * py;

    return {qx, qy};
}

std::vector<double> smallRotation(const std::vector<double>& data, double maxAngle = 0.25, int numHands = 2) {
    if (data.size() != numHands * 42) {
        throw std::invalid_argument("Data is not 2-dimensional");
    }

    std::vector<double> augmented(numHands * 42, 0.0);
    double angle = std::uniform_real_distribution<double>(-maxAngle, maxAngle)(std::default_random_engine()); // generate a random angle between -maxAngle and maxAngle

    for (int i = 0; i < numHands * 42; i += 2) {
        Point rotatedPoint = rotate({data[i], data[i + 1]}, angle);
        augmented[i] = rotatedPoint.x;
        augmented[i + 1] = rotatedPoint.y;
    }
    return augmented;
}

std::vector<double> scaling(const std::vector<double>& data, double maxFactor = 1.2){
    // Multiplies all data points by a value
    double factor = std::uniform_real_distribution<double>(1.0 / maxFactor, maxFactor)(std::default_random_engine());

    std::vector<double> scaledData = data;
    std::for_each(scaledData.begin(), scaledData.end(), [factor](double& val) { val *= factor; });

    return scaledData;
}

#endif // LANDMARKS_2D_HPP
