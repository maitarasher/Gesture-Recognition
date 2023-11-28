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

#include "false_positive_filter.h"
//do we need to include here opencv gain if its already inside the header file?

    void constructRepresentatives() {
        
    }

