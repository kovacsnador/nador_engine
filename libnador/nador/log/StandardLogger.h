#ifndef __NADOR_STANDARD_LOGGER_H__
#define __NADOR_STANDARD_LOGGER_H__

#include <string_view>

#include "nador/log/ILog.h"

namespace nador
{
    class StandardLogger
    {
    public:
        void Debug(std::string_view logMsg) const noexcept;
        void Warning(std::string_view logMsg) const noexcept;
        void Error(std::string_view logMsg) const noexcept;
        void Fatal(std::string_view logMsg) const noexcept;

        void EngineDebug(std::string_view logMsg) const noexcept;
        void EngineWarning(std::string_view logMsg) const noexcept;
        void EngineError(std::string_view logMsg) const noexcept;
        void EngineFatal(std::string_view logMsg) const noexcept;

        StandardLogger& Write(ELogType type, std::string_view logMsg);
    };

} // namespace nador

#endif //!__NADOR_STANDARD_LOGGER_H__