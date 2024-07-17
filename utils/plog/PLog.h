//
// Created by gaosm on 8/2/23.
//
#pragma once

/// 包裹类
/// @note 在程序启动的时候调用Logger::Start方法，例如：
///       Log.init("slog.properties");
///       打日志的示例如下：
///       Log.Debug("Debug log[%d]", 100);
///      [可以自定义不同格式]
///
///
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/config.hxx>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/socket.h>
#include <log4cplus/spi/loggerimpl.h>
#include <log4cplus/spi/loggingevent.h>

#if defined(__linux__)
#define VSPRINTF vsnprintf
#elif defined(_WIN32)
#define VSPRINTF _vsnprintf
#endif


class PLog
{
public:
    PLog();
    virtual ~PLog();

    /// 启动日志系统
    /// @param[in] properties_filename 日志系统配置文件文件名
    /// log种类和属性都在配置文件中设置
    void init(const char* properties_filename);

public:
    void Debug(const char* pFormat, ...);
    void Error(const char* pFormat, ...);
    void Fatal(const char* pFormat, ...);
    void Info(const char* pFormat, ...);
    void Warn(const char* pFormat, ...);
    void Trace(const char* pFormat, ...);

    void Debug(const std::string &pFormat, ...);
    void Error(const std::string &pFormat, ...);
    void Fatal(const std::string &pFormat, ...);
    void Info(const std::string &pFormat, ...);
    void Warn(const std::string &pFormat, ...);
    void Trace(const std::string &pFormat, ...);

    std::string getTime(std::string& str);

public:
    static inline PLog* getSingletonPtr(){
        return &getSingleton();
    }
    static inline PLog& getSingleton()
    {
        static PLog _instance;
        return _instance;
    }
};
#define Log PLog::getSingleton()
#define Plog ULog::getSingleton()


//
// 断言日志
//
#define ASSERT_LOG(expr)\
    if ( (expr) ) {;} else g_Logger.Error(__FILE__, __LINE__, #expr);


//
// 以下的宏只有VS2005以及之上的版本可以使用！因为VS2005之下的版本不支持可变参数宏
//
#if defined(_MSC_VER) && _MSC_VER > 1400
#define LOG_DEBUG(...)    g_Logger.Debug(__VA_ARGS__);
    #define LOG_ERROR(...)    g_Logger.Error(__VA_ARGS__);
    #define LOG_FATAL(...)    g_Logger.Fatal(__VA_ARGS__);
    #define LOG_INFO(...)     g_Logger.Info(__VA_ARGS__);
    #define LOG_WARN(...)     g_Logger.Warn(__VA_ARGS__);
    #define LOG_TRACE(...)    g_Logger.Trace(__VA_ARGS__);
#elif defined(__linux__)
#define LOG_DEBUG(...)    g_Logger.Debug(__VA_ARGS__);
#define LOG_ERROR(...)    g_Logger.Error(__VA_ARGS__);
#define LOG_FATAL(...)    g_Logger.Fatal(__VA_ARGS__);
#define LOG_INFO(...)     g_Logger.Info(__VA_ARGS__);
#define LOG_WARN(...)     g_Logger.Warn(__VA_ARGS__);
#define LOG_TRACE(...)    g_Logger.Trace(__VA_ARGS__);
#endif


