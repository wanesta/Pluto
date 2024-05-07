//
// Created by root on 9/22/22.
//
#include "workflow/WFFacilities.h"
#include <csignal>
#include "wfrest/HttpServer.h"
#include "wfrest/json.hpp"
#include "wfrest/CodeUtil.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <nlohmann/json.hpp>
#include "PLog.h"
#include "LightGBMPredict.h"

//using namespace boost;
using namespace wfrest;

using Json = nlohmann::json;
static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo){
    wait_group.done();
}

static LightGBMPredict lightgbm = LightGBMPredict();

void run(){
    signal(SIGINT, sig_handler);
    char *buffer;
    buffer = getcwd(NULL, 0);
    cout << "文件路径" << buffer << endl;
    //将需要调用的模块使用 strcat 作拼接;
    const char *model_path = strcat(buffer,"");

    std::cout<< "hhhhhhhhhhhhhhhhhhhhhhhhhhhh" << model_path << std::endl;
    // /root/inforefiner-model/model-data/LightGBM_model.txt
    const std::string model_file = "../model-data/LightGBM_model.txt";
    lightgbm.LoadModel(model_file);

    HttpServer svr;
    //Log.init("/home/gaosm/Downloads/dev-1/inforefiner-model/config/slog.properties");
    Log.init("../config/LogConf/log.properties");
    // curl -v http://0.0.0.0:8888/json3
    svr.GET("/json3", [](const HttpReq *req, HttpResp *resp){
        std::string invalid_text = R"(
        {
            "strings": ["extra", "comma", ]
        }
        )";
        resp->Json(invalid_text);
    });

    Log.Info("Load model path %s ...... ", model_file.c_str());
    svr.POST("/json4", [](const HttpReq *req, HttpResp *resp){

        Json json_result;
        try {
            Json req_context = req->json();
            time_t t = time(NULL);
            struct tm *stime = localtime(&t);
            Log.Info("predict begin ******** %4d-%02d-%02d %02d/%02d/%2d\n",
                     stime->tm_year + 1900, stime->tm_mon + 1,
                     stime->tm_mday, stime->tm_hour, stime->tm_min, stime->tm_sec);
            if (req_context.contains("scenesClass") && req_context.contains("data") && req_context["scenesClass"] == "forecast") {
                Json json_data = req_context["data"];
                std::vector<std::vector<float>> res_vec;
                std::cout << "---------------------------------------------------> " << req_context["data"].size()
                          << std::endl;
                for (Json::iterator it = json_data.begin(); it != json_data.end(); ++it) {
                    std::vector<float> row_data = *it;
                    row_data.push_back(lightgbm.predict(row_data));
                    res_vec.push_back(row_data);
                    //std::cout << "                 " << lightgbm.predict(row_data) << '\n';
                }
                json_result["scenesClass"] = "forecast";
                json_result["model"] = "failure-rate";
                json_result["result"] = res_vec;
            }else if (req_context.contains("scenesClass") && req_context.contains("data") && req_context["scenesClass"] == "explore") {
                Json json_data = req_context["data"];
                std::vector<std::vector<double>> dataVec;
                for(auto & it : json_data) {
                    std::vector<double> row_data = it;
                    dataVec.push_back(row_data);
                }
                int rowcnt = dataVec.size(), colcnt = dataVec[0].size();
            }else{
                throw 400;
            }
            Log.Info("predict finish ******** %4d-%02d-%02d %02d/%02d/%2d\n",stime->tm_year+1900,stime->tm_mon+1,stime->tm_mday,stime->tm_hour,stime->tm_min,stime->tm_sec);
            std::string str = to_string(req_context);
            Log.Info("json string : %s", str.c_str());
        }catch(std::string &e){
            json_result["status"] = 400;
            Log.Error("json string exception!!! %s", e.c_str());
        }catch (int i){
            if(i == 400) json_result["status"] = 400;
        }
        if (req->content_type() != APPLICATION_JSON){
            resp->String("NOT APPLICATION_JSON");
            Log.Debug("Debug log[%d]", 10000);
            return;
        }
        if(!json_result.contains("status")) json_result["status"] = 200;
        resp->Json(json_result);
    });
    const char *addr = "0.0.0.0";
    int port = 8888;
    if (svr.start(addr,port) == 0){
        Log.Info("server start ip : %s port : %d",addr,port);
        wait_group.wait();
        svr.stop();
    } else {
        fprintf(stderr, "Cannot start server");
        exit(1);
    }
}

int main(){
    run();
    return 0;
}
