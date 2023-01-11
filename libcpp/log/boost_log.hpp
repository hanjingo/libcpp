#ifndef BOOST_LOG_HPP
#define BOOST_LOG_HPP

namespace libcpp
{
namespace log
{

#include <string>
#include <iostream>
#include <memory>
#include <fstream>

#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

namespace sinks    = boost::log::sinks;
namespace src      = boost::log::sources;
namespace expr     = boost::log::expressions;
namespace attrs    = boost::log::attributes;
namespace keywords = boost::log::keywords;
namespace file     = boost::log::sinks::file;
namespace trivial  = boost::log::trivial;

class Logger;

enum Lvl { LVL_DEBUG = 0, LVL_INFO = 1, LVL_WARN = 2, LVL_ERROR = 3, LVL_FATAL = 4 };
static std::ostream& operator<<(std::ostream& stream, Lvl lvl)
{
    static const char* const LvlStr[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

    if (static_cast<std::size_t>(lvl) < sizeof(LvlStr) / sizeof(*LvlStr)) {
        stream << LvlStr[lvl];
    } else {
        stream << static_cast<int>(lvl);
    }

    return stream;
};

template<typename... Args, typename = typename std::enable_if<sizeof...(Args) == 0>::type>
static inline std::ostream & _output(std::ostream& stream, Args && ... args)
{
    return stream;
}

template<typename ArgType, typename... ArgTypes>
static inline std::ostream& _output(std::ostream& stream, ArgType&& arg, ArgTypes&& ... args)
{
    stream << arg;
    return _output(stream, std::forward<ArgTypes>(args)...);
}

template<typename... ArgTypes>
static inline std::string _format(ArgTypes&& ... args)
{
    std::ostringstream stream;
    cpplib::log::_output(stream, std::forward<ArgTypes>(args)...);
    return stream.str();
}

class Logger
{
public:
    typedef sinks::synchronous_sink<sinks::text_file_backend> file_sink;
    Logger();
    ~Logger();

    static Logger& Instance()
    {
        static Logger instance_;
        return instance_;
    };

    static src::severity_logger<Lvl>& Slg()
    {
        static src::severity_logger<Lvl> slg_;
        return slg_;
    }

    void AddFileSink(const std::string& dir,
                     const int max_file_size_mb,
                     const int max_backup,
                     boost::log::formatter fmt)
    {
        boost::shared_ptr<file_sink> fileSink(new file_sink(
                keywords::target_file_name    = "%Y%m%d_%H%M%S_%N.log",
                keywords::time_based_rotation = file::rotation_at_time_point(0, 0, 0),
                keywords::rotation_size       = max_file_size_mb * 1024 * 1024,
                keywords::open_mode           = std::ios::out | std::ios::app));

        fileSink->locked_backend()->set_file_collector(file::make_collector(
                    keywords::target         = dir,
                    keywords::max_size       = max_backup * max_file_size_mb * 1024 * 1024,
                    keywords::min_free_space = max_backup * max_file_size_mb * 1024 * 1024));

        fileSink->set_formatter(fmt);
        fileSink->locked_backend()->scan_for_files();
        fileSink->locked_backend()->auto_flush(true);
        boost::log::core::get()->add_sink(fileSink);
    }

    void AddConsoleSink(boost::log::formatter fmt)
    {
        auto consoleSink = boost::log::add_console_log();
        consoleSink->set_formatter(fmt);
        boost::log::core::get()->add_sink(consoleSink);
    }

    void SetLvl(Lvl min)
    {
        auto lvl = static_cast<int>(min);
        boost::log::core::get()->set_filter(trivial::severity >= lvl);
    }

    static inline boost::log::formatter DefaultFormatter()
    {
        boost::log::formatter fmt = expr::stream
                                    << "[" << expr::format_date_time<boost::posix_time::ptime>(
                                        "TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                                    << "|" << expr::attr<Lvl>("Severity") << "]"
                                    << " " << expr::smessage;
        return fmt;
    };

    template<typename... ArgTypes>
    static inline void Debug(const std::string& fmt, ArgTypes&& ... args)
    {
        BOOST_LOG_SEV(Slg(), LVL_DEBUG) << cpplib::log::_format(fmt, args...) << std::endl;
    }

    template<typename... ArgTypes>
    static inline void Info(const std::string& fmt, ArgTypes&& ... args)
    {
        BOOST_LOG_SEV(Slg(), LVL_INFO) << cpplib::log::_format(fmt, args...) << std::endl;
    }

    template<typename... ArgTypes>
    static inline void Warn(const std::string& fmt, ArgTypes&& ... args)
    {
        BOOST_LOG_SEV(Slg(), LVL_WARN) << cpplib::log::_format(fmt, args...) << std::endl;
    }

    template<typename... ArgTypes>
    static inline void Error(const std::string& fmt, ArgTypes&& ... args)
    {
        BOOST_LOG_SEV(Slg(), LVL_ERROR) << cpplib::log::_format(fmt, args...) << std::endl;
    }

    template<typename... ArgTypes>
    static inline void Fatal(const std::string& fmt, ArgTypes&& ... args)
    {
        BOOST_LOG_SEV(Slg(), LVL_ERROR) << cpplib::log::_format(fmt, args...) << std::endl;
    }
};

}
}

#endif
