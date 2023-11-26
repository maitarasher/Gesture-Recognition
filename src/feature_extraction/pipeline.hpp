#pragma once
#include "./stage.hpp"
#include <iostream>
#include <stdexcept> // Include for std::invalid_argument

class Pipeline
{
public:
    Pipeline(int num_hands) : num_hands(num_hands), stages()
    {
      if (num_hands > 2 || num_hands < 1)
      {
        throw std::invalid_argument("num_hands must be 1 or 2.");
      }
    }

    void add_stage(double brightness, double rotation)
    {
      Stage new_stage(stages.size(), brightness, rotation);
      stages.push_back(new_stage);
      std::cout << "Added stage with brightness: " << brightness << ", rotation: " << rotation << std::endl;
    }

    const std::vector<Stage>& getStages() const
    {
      return stages;
    }

    //void optimize();
private:
  int num_hands;
  std::vector<Stage> stages;  // Change the container type to std::vector
  //std::list<Stage> stages;
    //threadpool?
};
