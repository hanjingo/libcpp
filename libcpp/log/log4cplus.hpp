#ifndef LOG4CPLUS_HPP
#define LOG4CPLUS_HPP

namespace libcpp
{
namespace log
{

#include <log4cplus/config.hxx>
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/helpers/socket.h>
#include <log4cplus/spi/loggerimpl.h>
#include <log4cplus/spi/loggingevent.h>
#include <log4cplus/log4cplus.h>
#include <log4cplus/loggingmacros.h>

#define LOG_BUF_SIZE 10240

class Logger
{
public:
    Logger() {}
    virtual ~Logger() {};

    static Logger& Instance()
    {
        static Logger instance;
        return instance;
    }

public:
    void Write(log4cplus::LogLevel lvl, const char* fmt, ...)
    {
        if (!log4cplus::Logger::getRoot().isEnabledFor(lvl)) {
            return;
        }

        va_list args;
        va_start(args, fmt);
        char buf[LOG_BUF_SIZE] = {0};
        snprintf(buf, sizeof(buf), fmt, args);
        va_end(args);

        log4cplus::tostringstream ss;
        ss << buf;
        log4cplus::Logger::getRoot().forcedLog(lvl, ss.str());
    }
    void Trace(const char* fmt, ...)
    {
        Write(log4cplus::TRACE_LOG_LEVEL, fmt, ...);
    }
    // void Debug();
    // void Info();
    // void Warn();
    // void Error();
    // void Fatal();
}

#if defined(_MSC_VER) && _MSC_VER > 1400
#define LOG_TRACE(...)      Logger::Instance().Trace(__FILE__, __LINE__, __VA_ARGS__);
// #define LOG_DEBUG(...)      Logger::Instance().Debug(__FILE__, __LINE__, __VA_ARGS__);
// #define LOG_INFO(...)       Logger::Instance().Info(__FILE__, __LINE__, __VA_ARGS__);
// #define LOG_WARN(...)       Logger::Instance().Warn(__FILE__, __LINE__, __VA_ARGS__);
// #define LOG_ERROR(...)      Logger::Instance().Error(__FILE__, __LINE__, __VA_ARGS__);
// #define LOG_FATAL(...)      Logger::Instance().Fatal(__FILE__, __LINE__, __VA_ARGS__);
#endif

}
}

#endif
