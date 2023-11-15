#pragma once
#include "./stage.hpp"
#include <list>

class Pipeline
{
    int num_hands;
    list<Stage> stages ;
public:
    Pipeline(int num_hands) {
        self.num_hands=num_hands;
        stages = [];

    }
    void add_stage();
    //void optimize();
    




    //threadpool?
}
