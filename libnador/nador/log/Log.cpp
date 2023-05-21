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

    auto GetCurrentTimestamp()
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
        std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);



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
        RegisterCallback(nador::ELogType::ENGINE_DEBUG, [standardLogger](const char* msg) { standardLogger.Debug(msg); });
        RegisterCallback(nador::ELogType::ENGINE_WARNING, [standardLogger](const char* msg) { standardLogger.Warning(msg); });
        RegisterCallback(nador::ELogType::ENGINE_ERROR, [standardLogger](const char* msg) { standardLogger.Error(msg); });
        RegisterCallback(nador::ELogType::ENGINE_FATAL, [standardLogger](const char* msg) { standardLogger.Fatal(msg); });

        RegisterCallback(nador::ELogType::DEBUG, [standardLogger](const char* msg) { standardLogger.Debug(msg); });
        RegisterCallback(nador::ELogType::WARNING, [standardLogger](const char* msg) { standardLogger.Warning(msg); });
        RegisterCallback(nador::ELogType::ERROR, [standardLogger](const char* msg) { standardLogger.Error(msg); });
        RegisterCallback(nador::ELogType::FATAL, [standardLogger](const char* msg) { standardLogger.Fatal(msg); });
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

    void Log::EnableFuncName(bool enable) noexcept
    {
        _enableFuncName = enable;
    }

    void Log::EnableFileName(bool enable) noexcept
    {
        _enableFileName = enable;
    }

    void Log::EnableLine(bool enable) noexcept
    {
        _enableLine = enable;
    }

    void Log::LogMessageImpl(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, const char* msg, ...)
    {
        if (level >= _level)
        {
            const size_t buffSize = NADOR_LOG_MAX_MESSAGE_SIZE;
            char         logTemp[buffSize];

            const char* functionName = _enableFuncName ? func : "";
            const char* fileName     = _enableFileName ? file : "";

            int32_t next = 0;

            auto timeString  = GetCurrentTimestamp();
            auto logTypeName = GetLogTypeName(type);

            if (_enableLine)
            {
                next = snprintf(logTemp, buffSize, "(%s) [%s] {%s %s: %d} => ", timeString.data(), logTypeName, fileName, functionName, line);
            }
            else
            {
                next = snprintf(logTemp, buffSize, "(%s) [%s] {%s %s} => ", timeString.data(), logTypeName, fileName, functionName);
            }

            if (next < 0)
            {
                throw std::runtime_error("Encoding error occurs");
            }

            va_list va;
            va_start(va, msg);
            next += vsnprintf(logTemp + next, buffSize - next, msg, va);
            va_end(va);

            if (next < 0)
            {
                throw std::runtime_error("Message encoding error occurs");
            }

            // add new line
            if(snprintf(logTemp + next, buffSize - next, "\n") < 0)
            {
                throw std::runtime_error("New line encoding error occurs");
            }

            std::scoped_lock<std::mutex> lock(_mtx);

            auto iter = _logCallbacks.find(type);
            if (iter != _logCallbacks.end())
            {
                // call the message callback
                if(iter->second)
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