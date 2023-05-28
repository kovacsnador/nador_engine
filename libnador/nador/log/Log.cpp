#include <iomanip>
#include <ctime>
#include <cstring>
#include <iostream>

#include "nador/log/Log.h"
#include "nador/log/StandardLogger.h"

namespace nador
{
    static const char* GetLogTypeName(ELogType type)
    {
        switch (type)
        {
        case ELogType::DEBUG:
            return "DEBUG";
        case ELogType::WARNING:
            return "WARNING";
        case ELogType::ERROR:
            return "ERROR";
        case ELogType::FATAL:
            return "FATAL";
        case ELogType::ENGINE_DEBUG:
            return "ENGINE_DEBUG";
        case ELogType::ENGINE_WARNING:
            return "ENGINE_WARNING";
        case ELogType::ENGINE_ERROR:
            return "ENGINE_ERROR";
        case ELogType::ENGINE_FATAL:
            return "ENGINE_FATAL";
        default:
            return "UNKNOW";
        }
    }

    auto GetCurrentTimestamp(std::string_view format)
    {
        auto currentTime = std::chrono::system_clock::now();

        // Convert the time to milliseconds since epoch
        auto duration     = currentTime.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        // Convert the milliseconds to a time_point representing the local time
        std::time_t currentTimeMillis = std::chrono::system_clock::to_time_t(currentTime);
        std::tm*    localTime         = std::localtime(&currentTimeMillis);

        // Format the time string including milliseconds
        char timeString[100];
        std::strftime(timeString, sizeof(timeString), format.data(), localTime);

        char millisecondsString[4];
        std::snprintf(millisecondsString, sizeof(millisecondsString), "%.3d", static_cast<int>(milliseconds % 1000));

        std::array<char, sizeof(timeString) + sizeof(millisecondsString) + 1> result;

        std::strcpy(result.data(), timeString);
        strcat(result.data(), ".");
        strcat(result.data(), millisecondsString);

        return result;
    }

    Log::Log()
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

    int32_t Log::DefaultLogFormat(char*            buff,
                                  size_t           size,
                                  std::string_view time,
                                  std::string_view type,
                                  std::string_view file,
                                  std::string_view function,
                                  int32_t          line)
    {
        return snprintf(buff, size, "(%s) [%s] {%s %s: %d} => ", time.data(), type.data(), file.data(), function.data(), line);
    }

    bool Log::RegisterCallback(ELogType type, log_cb callback)
    {
        std::scoped_lock<std::mutex> lock(_mtx);

        auto [_, inserted] = _logCallbacks.insert_or_assign(type, callback);
        return inserted;
    }

    void Log::SetLevel(nador::ELogLevel level) noexcept
    {
        _level = level;
    }

    void Log::SetLogFormatStrategy(logFormatStrategy_cb strategy) noexcept
    {
        _logFormatStrategy = strategy;
    }

    void Log::SetTimeFormat(std::string_view timeFormat) noexcept
    {
        _timeFormat = timeFormat;
    }

    void Log::LogMessageImpl(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, std::string_view msg, ...)
    {
        if (level >= _level)
        {
            const size_t buffSize = NADOR_LOG_MAX_MESSAGE_SIZE;
            char         logTemp[buffSize];

            int32_t next = 0;

            auto timeString  = GetCurrentTimestamp(_timeFormat);
            auto logTypeName = GetLogTypeName(type);

            if(_logFormatStrategy)
            {
                next = _logFormatStrategy(logTemp, buffSize, timeString.data(), logTypeName, file, func, line);
            }
            else
            {
                next = DefaultLogFormat(logTemp, buffSize, timeString.data(), logTypeName, file, func, line);
            }

            if (next < 0)
            {
                throw std::runtime_error("Encoding error occurs");
            }

            va_list va;
            va_start(va, msg);
            next += vsnprintf(logTemp + next, buffSize - next, msg.data(), va);
            va_end(va);

            if (next < 0)
            {
                throw std::runtime_error("Message encoding error occurs");
            }

            // add new line
            if (snprintf(logTemp + next, buffSize - next, "\n") < 0)
            {
                throw std::runtime_error("New line encoding error occurs");
            }

            std::scoped_lock<std::mutex> lock(_mtx);

            auto iter = _logCallbacks.find(type);
            if (iter != _logCallbacks.end())
            {
                // call the message callback
                if (iter->second)
                {
                    iter->second(logTemp);
                }
            }

            // throw exception on FATAL always
            if (type == ELogType::ENGINE_FATAL || type == ELogType::FATAL)
            {
                throw std::runtime_error(logTemp);
            }
        }
    }

} // namespace nador