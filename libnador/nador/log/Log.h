#ifndef __NADOR_LOG_H__
#define __NADOR_LOG_H__

#include <memory>
#include <mutex>
#include <cstdarg>

#include "nador/log/ILog.h"

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
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::DEBUG, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::DEBUG, __VA_ARGS__)
#define NADOR_WARNING(...)                                                                                                                           \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::WARNING, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::WARNING, __VA_ARGS__)
#define NADOR_ERROR(...)                                                                                                                             \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::ERROR, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::ERROR, __VA_ARGS__)

#define ENGINE_DEBUG(...)                                                                                                                            \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::ENGINE_DEBUG, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::DEBUG, __VA_ARGS__)
#define ENGINE_WARNING(...)                                                                                                                          \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::ENGINE_WARNING, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::WARNING, __VA_ARGS__)
#define ENGINE_ERROR(...)                                                                                                                            \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::ENGINE_ERROR, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::ERROR, __VA_ARGS__)

#endif // DISABLE_NADOR_LOG

// Fatal always enabled
#define ENGINE_FATAL(...)                                                                                                                            \
    nador::GetLoggingInterface()->LogMessage(nador::ELogType::ENGINE_FATAL, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::FATAL, __VA_ARGS__)
#define NADOR_FATAL(...)                                                                                                                             \
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
        }                                                                                                                                            \
    } while (false)

#define NADOR_ASSERT_MSG(condition, msg)                                                                                                             \
    do                                                                                                                                               \
    {                                                                                                                                                \
        if (!(condition))                                                                                                                            \
        {                                                                                                                                            \
            ENGINE_FATAL("ASSERT FAILED! Condition: ( %s ), Message: %s", #condition, msg);                                                          \
        }                                                                                                                                            \
    } while (false)

#endif // DISABLE_NADOR_ASSERT

namespace nador
{
    class Log : public ILog
    {
    public:
        Log();

        bool RegisterCallback(ELogType type, log_cb callback) override;        
        void SetLevel(nador::ELogLevel level) noexcept override;
        void SetLogFormatStrategy(logFormatStrategy_cb strategy) noexcept override;
        void SetTimeFormat(std::string_view timeFormat) noexcept override;

    private:
        void LogMessageImpl(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, std::string_view msg, ...) override;

        int32_t DefaultLogFormat(char*            buff,
                                 size_t           size,
                                 std::string_view time,
                                 std::string_view type,
                                 std::string_view file,
                                 std::string_view function,
                                 int32_t          line);

        std::mutex _mtx;

        ELogLevel _level { ELogLevel::DEBUG };

        std::unordered_map<ELogType, log_cb> _logCallbacks;

        logFormatStrategy_cb _logFormatStrategy { nullptr };
        std::string          _timeFormat { "%Y-%m-%d %H:%M:%S" };
    };
} // namespace nador

#endif //!__NADOR_LOG_H__