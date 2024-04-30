//
// Created by root on 10/18/22.
//
#include "LightGBMPredict.h"

using namespace std;
LightGBMPredict::LightGBMPredict() {

}
void LightGBMPredict::LoadModel(const std::string filename) {
    int temp;
    int p = 1;
    temp = LGBM_BoosterCreateFromModelfile(filename.c_str(),&p,&this->handle);
    if(temp == 0){
        std::cout << "Load model success ... !" << std::endl;
    }
}
const float LightGBMPredict::predict(std::vector<float> &row){
    void *in_p = static_cast<void *>(row.data());
    std::vector<double> out(1, 0);
    std::vector<std::vector<double>> rVec;
    double *out_result = static_cast<double *>(out.data());
    int64_t out_len;
    LGBM_BoosterPredictForMat(this->handle, in_p, C_API_DTYPE_FLOAT32, 1, 28,
                              1, C_API_PREDICT_NORMAL, 0,
                              -1, "", &out_len, out_result);
    std::cout << "out &&&&&                                 ---- ---------------- "<< std::endl;
    for(auto& r : out){
        std::cout << "out &&&&&" << r << std::endl;
    }
    return out[0];
}

std::vector<double> LightGBMPredict::predictVec(std::vector<double> &row){

    void *in_p = static_cast<void *>(row.data());
    std::vector<double> out(1, 0);
    std::vector<std::vector<double>> rVec;
    double *out_result = static_cast<double *>(out.data());
    int64_t out_len;
    LGBM_BoosterPredictForMat(this->handle, in_p, C_API_DTYPE_FLOAT32, 1, 28, 1, C_API_PREDICT_NORMAL, 0, -1, "", &out_len, out_result);
    std::cout << "out &&&&& -------------------- "<< std::endl;
    for(auto& r : out){
        std::cout << "out &&&&&" << r << std::endl;
    }
    return out;
}