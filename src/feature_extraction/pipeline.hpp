#pragma once
#include "./stage.hpp"
#include <iostream>
#include <stdexcept> // Include for std::invalid_argument

/*
Type: class Pipeline
Description: Manages a sequence of image augmentation stages.
Functions:

-  Pipeline(int num_hands): Constructor, initializes the pipeline with the specified number of hands.
    Type: Constructor
    Parameters:
      num_hands: Number of hands (1).
    Why: Ensures the correct number of hands is provided for the pipeline.

- void add_stage(double brightness, double rotation): Adds a new stage to the pipeline with specified brightness and rotation.
  Type: Member Function
  Parameters:
    brightness: Degree of brightness adjustment.
    rotation: Degree of image rotation.
  Why: Facilitates the addition of customizable augmentation stages to the pipeline.

- const std::vector<Stage>& getStages() const: Retrieves the vector of stages in the pipeline.
  Type: Member Function
  Return Type: const std::vector<Stage>&
  Why: Allows external access to the stages for further usage.
*/

class Pipeline
{
public:
    Pipeline(int num_hands) : num_hands(num_hands), stages()
    {
      if (num_hands != 1)
      {
        throw std::invalid_argument("num_hands must be 1");
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

private:
  int num_hands;
  std::vector<Stage> stages;
};
