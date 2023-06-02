#ifndef __NADOR_LOG_H__
#define __NADOR_LOG_H__

#include <memory>
#include <mutex>
#include <cstdarg>
#include <queue>
#include <condition_variable>
#include <thread>
#include <variant>

#include "nador/log/ILog.h"
#include "nador/log/StandardLogger.h"

#define NADOR_LOG_MAX_MESSAGE_SIZE 2000

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define __FUNCTION_NAME__ __PRETTY_FUNCTION__

#if defined(DISABLE_NADOR_LOG)
#define NADOR_DEBUG(...)
#define NADOR_WARNING(...)
#define NADOR_ERROR(...)

#define ENGINE_DEBUG(...)
#define ENGINE_WARNING(...)
#define ENGINE_ERROR(...)
#else
#define NADOR_DEBUG(...)                                                                                                                             \
    if (nador::GetLoggingInterface())                                                                                                                \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::DEBUG, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::DEBUG, __VA_ARGS__)
#define NADOR_WARNING(...)                                                                                                                           \
    if (nador::GetLoggingInterface())                                                                                                                \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::WARNING, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::WARNING, __VA_ARGS__)
#define NADOR_ERROR(...)                                                                                                                             \
    if (nador::GetLoggingInterface())                                                                                                                \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::ERROR, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::ERROR, __VA_ARGS__)

#define ENGINE_DEBUG(...)                                                                                                                            \
    if (nador::GetLoggingInterface())                                                                                                                \
    nador::GetLoggingInterface()->LogMessage(                                                                                                        \
        nador::ELogType::ENGINE_DEBUG, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::DEBUG, __VA_ARGS__)
#define ENGINE_WARNING(...)                                                                                                                          \
    if (nador::GetLoggingInterface())                                                                                                                \
    nador::GetLoggingInterface()->LogMessage(                                                                                                        \
        nador::ELogType::ENGINE_WARNING, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::WARNING, __VA_ARGS__)
#define ENGINE_ERROR(...)                                                                                                                            \
    if (nador::GetLoggingInterface())                                                                                                                \
    nador::GetLoggingInterface()->LogMessage(                                                                                                        \
        nador::ELogType::ENGINE_ERROR, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::ERROR, __VA_ARGS__)

#endif // DISABLE_NADOR_LOG

// Fatal always enabled
#define ENGINE_FATAL(...)                                                                                                                            \
    if (nador::GetLoggingInterface())                                                                                                                \
    nador::GetLoggingInterface()->LogMessage(                                                                                                        \
        nador::ELogType::ENGINE_FATAL, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::FATAL, __VA_ARGS__)
#define NADOR_FATAL(...)                                                                                                                             \
    if (nador::GetLoggingInterface())                                                                                                                \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::FATAL, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::FATAL, __VA_ARGS__)

#ifdef DISABLE_NADOR_ASSERT

#define NADOR_ASSERT(condition)
#define NADOR_ASSERT_MSG(condition, msg)

#else

#define NADOR_ASSERT(condition)                                                                                                                      \
    do                                                                                                                                               \
    {                                                                                                                                                \
        if (!(condition))                                                                                                                            \
        {                                                                                                                                            \
            ENGINE_FATAL("ASSERT FAILED! Condition: ( %s )", #condition);                                                                            \
            std::abort();                                                                                                                            \
        }                                                                                                                                            \
    } while (false)

#define NADOR_ASSERT_MSG(condition, msg)                                                                                                             \
    do                                                                                                                                               \
    {                                                                                                                                                \
        if (!(condition))                                                                                                                            \
        {                                                                                                                                            \
            ENGINE_FATAL("ASSERT FAILED! Condition: ( %s ), Message: %s", #condition, msg);                                                          \
            std::abort();                                                                                                                            \
        }                                                                                                                                            \
    } while (false)

#endif // DISABLE_NADOR_ASSERT

namespace nador
{
    template <size_t BufferSize = 2000>
    class Log : public ILog
    {
        struct TerminalSignal
        {
        };

        using logMessage_t   = std::pair<ELogType, std::string>;
        using queueElement_t = std::variant<logMessage_t, TerminalSignal>;

    public:
        using log_cb = std::function<void(std::string_view)>;
        using logFormatStrategy_cb
            = std::function<int32_t(char*, size_t, std::string_view, std::string_view, std::string_view, std::string_view, int32_t)>;

        Log()
        : _workerThread(Log::WorkerThread, this)
        {
            nador::StandardLogger standardLogger;

            // setup default standard logging
            RegisterCallback(nador::ELogType::ENGINE_DEBUG, [standardLogger](std::string_view msg) { standardLogger.Debug(msg); });
            RegisterCallback(nador::ELogType::ENGINE_WARNING, [standardLogger](std::string_view msg) { standardLogger.Warning(msg); });
            RegisterCallback(nador::ELogType::ENGINE_ERROR, [standardLogger](std::string_view msg) { standardLogger.Error(msg); });
            RegisterCallback(nador::ELogType::ENGINE_FATAL, [standardLogger](std::string_view msg) { standardLogger.Fatal(msg); });

            RegisterCallback(nador::ELogType::DEBUG, [standardLogger](std::string_view msg) { standardLogger.Debug(msg); });
            RegisterCallback(nador::ELogType::WARNING, [standardLogger](std::string_view msg) { standardLogger.Warning(msg); });
            RegisterCallback(nador::ELogType::ERROR, [standardLogger](std::string_view msg) { standardLogger.Error(msg); });
            RegisterCallback(nador::ELogType::FATAL, [standardLogger](std::string_view msg) { standardLogger.Fatal(msg); });
        }

        ~Log()
        {
            {
                std::scoped_lock<std::mutex> lock(_mtx);
                _logQueue.emplace(TerminalSignal {});
            }

            _cv.notify_one();

            if (_workerThread.joinable())
            {
                _workerThread.join();
            }
        }

        bool RegisterCallback(ELogType type, log_cb callback)
        {
            std::scoped_lock<std::mutex> lock(_mtx);

            auto [_, inserted] = _logCallbacks.insert_or_assign(type, callback);
            return inserted;
        }

        void SetLevel(nador::ELogLevel level) noexcept { _level = level; }

        void SetLogFormatStrategy(logFormatStrategy_cb strategy) noexcept { _logFormatStrategy = strategy; }

        void SetTimeFormat(std::string_view timeFormat) noexcept { _timeFormat = timeFormat; }

    private:
        void LogMessageImpl(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, std::string_view msg, ...) override
        {
            if (level >= _level)
            {
                char logTemp[BufferSize];

                int32_t next = 0;

                auto timeString  = GetCurrentTimestamp(_timeFormat);
                auto logTypeName = GetLogTypeName(type);

                if (_logFormatStrategy)
                {
                    next = _logFormatStrategy(logTemp, BufferSize, timeString.data(), logTypeName, file, func, line);
                }
                else
                {
                    next = DefaultLogFormat(logTemp, BufferSize, timeString.data(), logTypeName, file, func, line);
                }

                if (next < 0)
                {
                    throw std::runtime_error("Encoding error occurs");
                }

                va_list va;
                va_start(va, msg);
                next += vsnprintf(logTemp + next, BufferSize - next, msg.data(), va);
                va_end(va);

                if (next < 0)
                {
                    throw std::runtime_error("Message encoding error occurs");
                }

                // add new line
                if (snprintf(logTemp + next, BufferSize - next, "\n") < 0)
                {
                    throw std::runtime_error("New line encoding error occurs");
                }

                {
                    std::scoped_lock<std::mutex> lock(_mtx);
                    _logQueue.emplace(logMessage_t { type, logTemp });
                }

                _cv.notify_one();
            }
        }

        int32_t DefaultLogFormat(char*            buff,
                                 size_t           size,
                                 std::string_view time,
                                 std::string_view type,
                                 std::string_view file,
                                 std::string_view function,
                                 int32_t          line)
        {
            return snprintf(buff, size, "(%s) [%s] {%s %s: %d} => ", time.data(), type.data(), file.data(), function.data(), line);
        }

        void WorkerThread()
        {
            for (;;)
            {
                std::unique_lock<std::mutex> lock(_mtx);
                _cv.wait(lock, [this] { return _logQueue.size(); });

                auto elem = _logQueue.front();
                _logQueue.pop();

                if (std::holds_alternative<TerminalSignal>(elem))
                {
                    break;
                }

                const auto& [logType, msg] = std::get<logMessage_t>(elem);

                auto iter = _logCallbacks.find(logType);
                if (iter != _logCallbacks.end())
                {
                    // call the message callback
                    if (iter->second)
                    {
                        iter->second(msg);
                    }
                }
            }
        }

        std::mutex _mtx;

        ELogLevel _level { ELogLevel::DEBUG };

        std::unordered_map<ELogType, log_cb> _logCallbacks;

        std::condition_variable _cv;
        std::thread             _workerThread;

        std::queue<queueElement_t> _logQueue;

        logFormatStrategy_cb _logFormatStrategy { nullptr };
        std::string          _timeFormat { "%Y-%m-%d %H:%M:%S" };
    };
} // namespace nador

#endif //!__NADOR_LOG_H__