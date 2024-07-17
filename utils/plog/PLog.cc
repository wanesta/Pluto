/**********************************************************************
* Copyright (C) 2018 -  - All Rights Reserved
*
* 文件名称:        plog.cc
* 摘    要:        日志包裹类,日志模块的接口
*
* 作    者:       root
* 修    改:       查看文件最下方.
*
***********************************************************************/

#include "PLog.h"


using namespace log4cplus;
using namespace log4cplus::helpers;

PLog::PLog(){

}

PLog::~PLog(){
    log4cplus::Logger _logger = log4cplus::Logger::getInstance("global");
    LOG4CPLUS_INFO(_logger, "Slog System Stop Finish...");
    _logger.shutdown();
}



#define DO_LOGGER(logLevel, pFormat, bufSize)\
    log4cplus::Logger _logger = log4cplus::Logger::getInstance("global");\
    \
    if(_logger.isEnabledFor(logLevel))\
{                \
    va_list args;            \
    va_start(args, pFormat);        \
    char buf[bufSize] = {0};        \
    VSPRINTF(buf, sizeof(buf), pFormat, args);    \
    va_end(args);           \
    _logger.forcedLog(logLevel, buf); \
}

void PLog::Debug(const char* pFormat,  ...){
    DO_LOGGER(log4cplus::DEBUG_LOG_LEVEL, pFormat, 1024);
}

void PLog::Error(const char* pFormat,  ...){
    DO_LOGGER(log4cplus::ERROR_LOG_LEVEL, pFormat, 256);
}

void PLog::Fatal(const char* pFormat, ... ){
    DO_LOGGER(log4cplus::FATAL_LOG_LEVEL, pFormat, 256);
}

void PLog::Info( const char* pFormat,  ...){
    std::string strTime = "";
    this->getTime(strTime);
    char c[1000];
    strcat(c, strTime.c_str());
    strcat(c, pFormat);
    DO_LOGGER(log4cplus::INFO_LOG_LEVEL, c, 512);
}

void PLog::Warn( const char* pFormat,  ...){
    DO_LOGGER(log4cplus::WARN_LOG_LEVEL, pFormat, 256);
}

void PLog::Trace(const char* pFormat,  ...){
    DO_LOGGER(log4cplus::TRACE_LOG_LEVEL, pFormat, 1024);
}

std::string PLog::getTime(std::string & str){
    std::time_t t = std::time(NULL);
    struct tm *stime = localtime(&t);
    const char* format = "%Y-%m-%d %H:%M:%S";
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), format, std::localtime(&t));
    str = buffer;
    return str;
}


void PLog::Info( const std::string &pFormat,  ...){
    //std::string strTime = "";
    //this->getTime(strTime);
    //strTime += pFormat;
    DO_LOGGER(log4cplus::INFO_LOG_LEVEL, pFormat.c_str(), 512);
}

void PLog::Debug(const std::string &pFormat,  ...){
    DO_LOGGER(log4cplus::DEBUG_LOG_LEVEL, pFormat.c_str(), 1024);
}

void PLog::Error(const std::string &pFormat,  ...){
    DO_LOGGER(log4cplus::ERROR_LOG_LEVEL, pFormat.c_str(), 256);
}

void PLog::Fatal(const std::string &pFormat, ... ){
    DO_LOGGER(log4cplus::FATAL_LOG_LEVEL, pFormat.c_str(), 256);
}

void PLog::Warn( const std::string &pFormat,  ...){
    DO_LOGGER(log4cplus::WARN_LOG_LEVEL, pFormat.c_str(), 256);
}

void PLog::Trace(const std::string &pFormat,  ...){
    DO_LOGGER(log4cplus::TRACE_LOG_LEVEL, pFormat.c_str(), 1024);
}

void PLog::init( const char* properties_filename )
{
    if (properties_filename==NULL) return;
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(properties_filename);
    log4cplus::Logger _logger = log4cplus::Logger::getInstance("root");
    LOG4CPLUS_INFO(_logger, "Logger system start finish.");
}
