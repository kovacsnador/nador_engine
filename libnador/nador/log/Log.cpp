#include <iomanip>
#include <ctime>
#include <cstring>
#include <iostream>

#include "nador/log/Log.h"
#include "Log.h"

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
        // setup default standard logging
        RegisterCallback(nador::ELogType::ENGINE_DEBUG, [](const char* msg) { std::cout << "\033[1;32m" << msg << "\033[0m\n"; });
        RegisterCallback(nador::ELogType::ENGINE_WARNING, [](const char* msg) { std::cout << "\033[1;33m" << msg << "\033[0m\n"; });
        RegisterCallback(nador::ELogType::ENGINE_ERROR, [](const char* msg) { std::cerr << "\033[1;31m" << msg << "\033[0m\n"; });
        RegisterCallback(nador::ELogType::ENGINE_FATAL, [](const char* msg) { std::cerr << "\033[1;35m" << msg << "\033[0m\n"; });

        RegisterCallback(nador::ELogType::DEBUG, [](const char* msg) { std::cout << "\033[1;32m" << msg << "\033[0m\n"; });
        RegisterCallback(nador::ELogType::WARNING, [](const char* msg) { std::cout << "\033[1;33m" << msg << "\033[0m\n"; });
        RegisterCallback(nador::ELogType::ERROR, [](const char* msg) { std::cerr << "\033[1;31m" << msg << "\033[0m\n"; });
        RegisterCallback(nador::ELogType::FATAL, [](const char* msg) { std::cerr << "\033[1;35m" << msg << "\033[0m\n"; });
    }

    bool Log::RegisterCallback(ELogType type, log_cb callback)
    {
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
            const size_t buff_size = NADOR_LOG_MAX_MESSAGE_SIZE;
            char         log_temp[buff_size];

            const char* functionName = _enableFuncName ? func : "";
            const char* fileName     = _enableFileName ? file : "";

            int32_t next = 0;

            auto timeString  = GetCurrentTimestamp();
            auto logTypeName = GetLogTypeName(type);

            if (_enableLine)
            {
                next = snprintf(log_temp, buff_size, "(%s) [%s] {%s %s: %d} => ", timeString.data(), logTypeName, fileName, functionName, line);
            }
            else
            {
                next = snprintf(log_temp, buff_size, "(%s) [%s] {%s %s} => ", timeString.data(), logTypeName, fileName, functionName);
            }

            if (next < 0)
            {
                throw std::runtime_error("Encoding error occurs");
            }

            va_list va;
            va_start(va, msg);
            vsnprintf(log_temp + next, buff_size - next, msg, va);
            va_end(va);

            std::lock_guard<std::mutex> lock(_mtx);

            auto iter = _logCallbacks.find(type);
            if (iter != _logCallbacks.end())
            {
                // call the message callback
                iter->second(log_temp);
            }

            // throw exception on FATAL always
            if (type == ELogType::ENGINE_FATAL || type == ELogType::FATAL)
            {
                throw std::runtime_error(log_temp);
            }
        }
    }

} // namespace nador