#ifndef __NADOR_I_LOG_H__
#define __NADOR_I_LOG_H__

#include <memory>
#include <cstdint>
#include <string_view>
#include <array>
#include <functional>

#define NADOR_LOG nador::GetLogInterface()

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
        ENGINE_FATAL
    };

    inline const char* GetLogTypeName(ELogType type)
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

    class ILog
    {
    public:
        struct FlushRequest{};

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

    void SetCrashHandler(std::function<void(uint32_t)> callback);

    static constexpr size_t s_timeStringLenght = 100;
    static constexpr size_t s_millisecondsStringLenght = 4;

    using TimestampArray = std::array<char, (s_timeStringLenght + s_millisecondsStringLenght)>;
    
    TimestampArray GetCurrentTimestamp(std::string_view format);

} // namespace nador

#endif //!__NADOR_I_LOG_H__