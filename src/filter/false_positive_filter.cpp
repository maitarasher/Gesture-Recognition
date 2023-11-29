/*
 * @file false_positive_filter

 * @brief Filters out landmarks that are not close enough to any class.
 * First, creates the representative samples of each class by taking the mean landmarks of each class.
 * Then compares every incoming image using euclidean or cosine distance to the representative.
 * If any of the representatives are withing confidence, the sample is marked as relevant and saved further.
 * 
 * @version 1.0
 * @date updated Nov 27, 2023
 */
#include <tuple>
#include "false_positive_filter.h"
//do we need to include here opencv gain if its already inside the header file?

void constructRepresentatives() {

    }
return tuple<float,itn> closest_representative;

static float cosineSimilarity(const Point& point_test, const Point& point_check) const {
    float x = point_test.x *point_check.x;
    float y = point_test.y *point_check.y;
    float z = point_test.z * point_check.z;
    float dot = x+y+z;
    float norm_1 = 0.0;
    norm_1 = sqrt(point_test.x*point_test.x+point_test.y*point_test.y+point_test.z*point_test.z);
    float norm_2 = 0.0;
    norm_2 = sqrt(point_check.x*point_check.x+point_check.y*point_check.y+point_check.z*point_check.z);

    return dot/(norm_1*norm_2)
}

static float cosineSimilarity_vector(const vector<T>& point_test, const vector<T>& point_check) const {
    assert(point_test.size()==point_check.size());
    float dot = 0.0;
    float norm_1 =0;
    float norm_2=0;
    for(int i=0 ; i <point_check.size(); i++) {
        dot +=point_test[i]*point_check[i];
        norm_1 +=point_test*point_test[i];
        norm_2+=point_check[i]*point_check[i];
    }
    norm_1=sqrt(norm_1);
    norm_2=sqrt(norm_2);
    return dot/(norm_1*norm_2);
}

static float euclideanSimilarity(const Point& point_test, const Point& point_check) const {
    double x = point_test.x - point_check.x;
    double y = point_test.y - point_check.y;
    double z = point_test.z - point_check.z
    return 1/(1+sqrt(x*x+y*y+z*z));
}

static float euclideanSimilarity_vector(const vector<T>& point_test, const vector<T>& point_check) const {
    assert(point_test.size()==point_check.size())
    float sum = 0.0;
    for (int i =0; i< point_check.size();i++){
        sum += pow(point_test[i] - point_check[i], 2);
    }
    return 1/(1+sqrt(x*x+y*y+z*z));
}