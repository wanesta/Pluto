//
// Created by root on 10/18/22.
//
#pragma once
#include <LightGBM/c_api.h>
#include <iostream>
#include <vector>

using namespace std;
class LightGBMPredict {
public:
    LightGBMPredict();
    const float predict(std::vector<float> &row);
    std::vector<double> predictVec(std::vector<double> &row);
    void LoadModel(const std::string filename);

private:
    BoosterHandle handle;
};
