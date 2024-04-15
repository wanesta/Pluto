//
// Created by gaosm on 21/12/23.
//
#include "YamlParse.h"
#include <yaml-cpp/yaml.h>
#include <iostream>


YamlParse::YamlParse(){
    try {
        YAML::Node node = YAML::LoadFile("../../config/yamlConf/conf.yaml");
        std::cout << node << std::endl; //输出yaml数据
    } catch (...) { //文件为非yaml格式抛出异常
        std::cout << "error" << std::endl;
    }
};

YamlParse::~YamlParse() = default;