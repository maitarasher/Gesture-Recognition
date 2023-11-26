/*
 * @file base_loader.h
 * @author
 * @brief A class for getting landmarks from dynamic gestures
 * @version 1.0
 * @date updated Nov 25, 2023
 */

#pragma once
#include <vector>
#include <iostream>
#include <filesystem>
#include <string>
#include "./base_loader.hpp"

using namespace std;
namespace fs = filesystem;

/*
@brief: Retrieves landmarks from images, inherits base_loader/ takes it as a parameter

*/
class DynamicGestureLoafer {
    string fpath ;
public:
//pass by value or by reference?

/*
@brief: Retrieves landmarks from images, inherits base_loader/ takes it as a parameter
@param fpath: path to the main dataset's folder
@param key_frames: amount of key frames
@param g

*/
DynamicGestureLoafer(string fpath, int key_frames, float traj_zero_precision,
                    char frame_separator ="_")
{
    self.fpath = fpath //can you even do it?

}

}

