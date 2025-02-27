#ifndef LOGGER_HPP
#define LOGGER_HPP

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <atomic>
#include <mutex>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#ifndef LOG_QUEUE_SIZE
#define LOG_QUEUE_SIZE 102400
#endif

#ifndef LOG_THREAD_NUM
#define LOG_THREAD_NUM 1
#endif

namespace libcpp
{

class logger;

enum log_lvl : int {
    log_lvl_trace = 0,
    log_lvl_debug,
    log_lvl_info,
    log_lvl_warn,
    log_lvl_err,
    log_lvl_critial,
};

static std::mutex log_mu;
static std::shared_ptr<libcpp::logger> log_inst = 
    std::make_shared<libcpp::logger>(spdlog::default_logger());

class logger
{
public:
    using sink_ptr_t = spdlog::sink_ptr;
    using base_logger_ptr_t = std::shared_ptr<spdlog::logger>;

public:
    logger() = delete;
    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;

    logger(base_logger_ptr_t base) : base_{base} {}

    logger(const std::string& id, 
           const bool async = false, 
           const uint64_t queue_size = LOG_QUEUE_SIZE, 
           const uint64_t thread_num = LOG_THREAD_NUM)
    {
        std::vector<sink_ptr_t> sinks{};

        if (async)
        {
            spdlog::init_thread_pool(queue_size, thread_num);

            base_ = std::make_shared<spdlog::async_logger>(
                id, 
                sinks.begin(), 
                sinks.end(),
                spdlog::thread_pool(), spdlog::async_overflow_policy::overrun_oldest);
        }
        else
        {
            base_ = std::make_shared<spdlog::logger>(id, sinks.begin(), sinks.end());
        }

        base_->set_level(spdlog::level::level_enum(log_lvl_trace));
    }

    ~logger()
    {
        flush();
        spdlog::drop_all();
    }

    static std::shared_ptr<libcpp::logger> instance()
    {
        return log_inst;
    }

    static void set_default(std::shared_ptr<libcpp::logger>&& inst)
    {
        std::lock_guard<std::mutex> lock(log_mu);
        log_inst = std::move(inst);
    }

    static sink_ptr_t create_sink()
    {
        return std::make_shared<spdlog::sinks::stdout_sink_mt>();
    }

    static sink_ptr_t create_sink(const std::string& filename,
                                  const std::size_t max_size,
                                  const std::size_t max_files,
                                  const bool rotate_on_open = false)
    {
        return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            filename, max_size, max_files, rotate_on_open);
    }

    inline void add_sink(const sink_ptr_t&& sink)
    {
        std::lock_guard<std::mutex> lock(log_mu);
        base_->sinks().push_back(std::move(sink));
    }

    inline void set_level(libcpp::log_lvl lvl)
    {
        base_->set_level(spdlog::level::level_enum(lvl));
    }

    inline log_lvl get_level()
    {
        return static_cast<log_lvl>(base_->level());
    }

    // see also: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    inline void set_pattern(const char* patterm)
    {
        base_->set_pattern(patterm);
    }

    inline void flush()
    {
        base_->flush();
    }

    inline void flush_on(const log_lvl lvl)
    {
        base_->flush_on(spdlog::level::level_enum(lvl));
    }

public:
    template<typename... Args>
    inline void trace(const char* fmt, Args&& ... args)
    {
        base_->trace(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void debug(const char* fmt, Args&& ... args)
    {
        base_->debug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void info(const char* fmt, Args&& ... args)
    {
        base_->info(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void warn(const char* fmt, Args&& ... args)
    {
        base_->warn(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline void error(const char* fmt, Args&& ... args)
    {
        base_->error(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(const char* fmt, Args&& ... args)
    {
        base_->critical(fmt, std::forward<Args>(args)...);
    }

private:
    base_logger_ptr_t base_;
};

}

#define LOG_TRACE(...) \
    libcpp::logger::instance()->trace(__VA_ARGS__)

#define LOG_DEBUG(...) \
    libcpp::logger::instance()->debug(__VA_ARGS__)

#define LOG_INFO(...) \
    libcpp::logger::instance()->info(__VA_ARGS__)

#define LOG_WARN(...) \
    libcpp::logger::instance()->warn(__VA_ARGS__)

#define LOG_ERROR(...) \
    libcpp::logger::instance()->error(__VA_ARGS__)

#define LOG_CRITICAL(...) \
    libcpp::logger::instance()->critical(__VA_ARGS__)

#define LOG_FLUSH() \
    libcpp::logger::instance()->flush();

#endif