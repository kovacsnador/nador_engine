#ifndef __NADOR_I_LOG_H__
#define __NADOR_I_LOG_H__

#include <memory>
#include <cstdint>
#include <functional>
#include <string_view>
#include <chrono>
#include <ctime>
#include <cstring>

#define NADOR_LOG nador::GetLogInterface()

namespace nador
{
    enum class ELogLevel : uint32_t
    {
        DEBUG = 0,
        WARNING,
        ERROR,
        FATAL,
    };

    enum class ELogType
    {
        DEBUG,
        WARNING,
        ERROR,
        FATAL,

        ENGINE_DEBUG,
        ENGINE_WARNING,
        ENGINE_ERROR,
        ENGINE_FATAL,
    };

    class ILog
    {
    public:
        using log_cb = std::function<void(std::string_view)>;
        using logFormatStrategy_cb
            = std::function<int32_t(char*, size_t, std::string_view, std::string_view, std::string_view, std::string_view, int32_t)>;

        /*!
         * Registers a callback touched to a log type.
         *
         * \param type The log type.
         * \param callback The callback.
         *
         * \return  True if inserted.
         */
        virtual bool RegisterCallback(ELogType type, log_cb callback) = 0;

        /*!
         * Set the log level.
         *
         * \param level The level.
         */
        virtual void SetLevel(nador::ELogLevel level) noexcept = 0;

        /*!
         * Set the log format strategy.
         *
         * \param strategy The strategy callback.
         */
        virtual void SetLogFormatStrategy(logFormatStrategy_cb strategy) noexcept = 0;

        /*!
         * Set the time format.
         *
         * \param timeFormat The time format string.
         */
        virtual void SetTimeFormat(std::string_view timeFormat) noexcept = 0;

        /*!
         * Wrapper function to log messages.
         *
         * \param type	The type of the log.
         * \param file	The name of the file.
         * \param line	Line count.
         * \param func	The function name.
         * \param level	The log level.
         * \param ...args	The specific arguments.
         */
        template <typename... Args>
        void LogMessage(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, const Args&... args)
        {
            LogMessageImpl(type, file, line, func, level, args...);
        }

        virtual ~ILog() = default;

    private:
        /*!
         * Log the message with the additional callback.
         *
         * \param type	The log type.
         * \param file  Name of the file.
         * \param line	Line count.
         * \param func	Name of the function.
         * \param level	The log level.
         * \param msg	The message to log.
         * \param ...	Additional params.
         */
        virtual void LogMessageImpl(ELogType         type,
                                    const char*      file,
                                    int32_t          line,
                                    const char*      func,
                                    nador::ELogLevel level,
                                    std::string_view msg,
                                    ...) = 0;
    };

    ILog* GetLoggingInterface();
    void  SetLoggingInterface(std::unique_ptr<ILog> log);
    
    inline auto GetCurrentTimestamp(std::string_view format)
    {
        static constexpr size_t timeStringLenght = 100;
        static constexpr size_t millisecondsStringLenght = 4;

        auto currentTime = std::chrono::system_clock::now();

        // Convert the time to milliseconds since epoch
        auto duration     = currentTime.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        // Convert the milliseconds to a time_point representing the local time
        std::time_t currentTimeMillis = std::chrono::system_clock::to_time_t(currentTime);
        std::tm*    localTime         = std::localtime(&currentTimeMillis);

        // Format the time string including milliseconds
        char timeString[timeStringLenght];
        std::strftime(timeString, sizeof(timeString), format.data(), localTime);

        char millisecondsString[millisecondsStringLenght];
        std::snprintf(millisecondsString, sizeof(millisecondsString), "%.3d", static_cast<int>(milliseconds % 1000));

        std::array<char, timeStringLenght + millisecondsStringLenght + 1> result;

        std::strcpy(result.data(), timeString);
        strcat(result.data(), ".");
        strcat(result.data(), millisecondsString);

        return result;
    }

} // namespace nador

#endif //!__NADOR_I_LOG_H__