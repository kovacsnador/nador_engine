#include <chrono>
#include <ctime>
#include <cstring>
#include <stdio.h>

#include "nador/log/ILog.h"

namespace nador
{
    static std::unique_ptr<ILog> g_logInterface {nullptr};
    static std::function<void(uint32_t)> g_crashHandler {};

#ifdef PLATFORM_WINDOWS
#include <Windows.h>

    static LONG WINAPI CrashHandler([[maybe_unused]] EXCEPTION_POINTERS* ExceptionInfo)
    {
        if(g_crashHandler)
        {
            int32_t exceptionCode = {0};

            if(ExceptionInfo && ExceptionInfo->ExceptionRecord)
            {
                exceptionCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
            }

            g_crashHandler(exceptionCode);
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }
#endif


    ILog* GetLoggingInterface()
    {
        return g_logInterface.get();
    }

    void SetLoggingInterface(std::unique_ptr<ILog> log)
    {
        g_logInterface = std::move(log);
    }

    void SetCrashHandler(std::function<void(uint32_t)> callback)
    {
        g_crashHandler = callback;

#ifdef PLATFORM_WINDOWS
        SetUnhandledExceptionFilter(CrashHandler);
#endif
    }

    TimestampArray GetCurrentTimestamp(std::string_view format)
    {
        auto currentTime = std::chrono::system_clock::now();

        // Convert the time to milliseconds since epoch
        auto duration     = currentTime.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        // Convert the milliseconds to a time_point representing the local time
        std::time_t currentTimeMillis = std::chrono::system_clock::to_time_t(currentTime);
        std::tm*    localTime         = std::localtime(&currentTimeMillis);

        // Format the time string including milliseconds
        char timeString[s_timeStringLenght];
        std::strftime(timeString, sizeof(timeString), format.data(), localTime);

        char millisecondsString[s_millisecondsStringLenght];
        ::snprintf(millisecondsString, sizeof(millisecondsString), "%.3d", static_cast<int>(milliseconds % 1000));

        TimestampArray result;

        std::strcpy(result.data(), timeString);
        strcat(result.data(), ".");
        strcat(result.data(), millisecondsString);

        return result;
    }
}
