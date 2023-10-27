#ifndef __NADOR_LOG_H__
#define __NADOR_LOG_H__

#include <memory>
#include <mutex>
#include <cstdarg>
#include <queue>
#include <condition_variable>
#include <thread>
#include <variant>
#include <functional>
#include <chrono>

#include "nador/log/ILog.h"
#include "nador/log/StandardLogger.h"

namespace nador
{
    using namespace std::chrono_literals;

    template <size_t BufferSize = 2000>
    class Log : public ILog
    {
        struct TerminalSignal
        {
        };

        using logMessage_t   = std::pair<ELogType, std::string>;
        using queueElement_t = std::variant<logMessage_t, TerminalSignal>;

    public:
        using LogCallbackMsg_t = std::variant<std::string_view, FlushRequest>;

        using LogCallback_t = std::function<void(ELogType, LogCallbackMsg_t)>;
        using LogFormatStrategyCb_t
            = std::function<int32_t(char*, size_t, std::string_view, std::string_view, std::string_view, std::string_view, int32_t)>;

        Log()
        : _workerThread(Log::WorkerThread, this)
        {
            nador::StandardLogger standardLogger;

            auto logCallback = [standardLogger](ELogType type, LogCallbackMsg_t event) mutable {
                if (std::holds_alternative<std::string_view>(event))
                {
                    auto msg = std::get<std::string_view>(event);
                    standardLogger.Write(type, msg);
                }
            };

            // setup default standard logging
            RegisterCallback(nador::ELogType::ENGINE_DEBUG, logCallback);
            RegisterCallback(nador::ELogType::ENGINE_WARNING, logCallback);
            RegisterCallback(nador::ELogType::ENGINE_ERROR, logCallback);
            RegisterCallback(nador::ELogType::ENGINE_FATAL, logCallback);

            RegisterCallback(nador::ELogType::DEBUG, logCallback);
            RegisterCallback(nador::ELogType::WARNING, logCallback);
            RegisterCallback(nador::ELogType::ERROR, logCallback);
            RegisterCallback(nador::ELogType::FATAL, logCallback);
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

        bool RegisterCallback(ELogType type, LogCallback_t callback)
        {
            std::scoped_lock<std::mutex> lock(_mtx);

            auto [_, inserted] = _logCallbacks.insert_or_assign(type, callback);
            return inserted;
        }

        void SetLevel(nador::ELogLevel level) noexcept { _level = level; }

        void SetLogFormatStrategy(LogFormatStrategyCb_t strategy) noexcept { _logFormatStrategy = strategy; }

        void SetTimeFormat(std::string_view timeFormat) noexcept { _timeFormat = timeFormat; }

    private:
        void LogMessageImpl(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, std::string_view msg, ...)
            override
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
            auto lastFlush = std::chrono::system_clock::now();

            auto flushRequestCallback = [this]()
            {
                for (const auto& it : _logCallbacks)
                {
                    it.second(ELogType::DEBUG, FlushRequest {});
                }
            };

            constexpr auto flushTimeout = 500ms;

            for (;;)
            {
                std::unique_lock<std::mutex> lock(_mtx);
                _cv.wait_for(lock, flushTimeout, [this] { return _logQueue.size(); });

                auto now = std::chrono::system_clock::now();
                // send only flush event trough callback
                if (_logQueue.empty() || now - flushTimeout > lastFlush)
                {
                    flushRequestCallback();
                    lastFlush = now;
                }
                
                if(_logQueue.empty() == false)
                {
                    // queue is not empty, sending real message
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
                            iter->second(logType, msg);
                        }
                    }
                }
            }
        }

        std::mutex _mtx;

        ELogLevel _level { ELogLevel::DEBUG };

        std::unordered_map<ELogType, LogCallback_t> _logCallbacks;

        std::condition_variable _cv;
        std::thread             _workerThread;

        std::queue<queueElement_t> _logQueue;

        LogFormatStrategyCb_t _logFormatStrategy { nullptr };
        std::string           _timeFormat { "%Y-%m-%d %H:%M:%S" };
    };
} // namespace nador

#endif //!__NADOR_LOG_H__