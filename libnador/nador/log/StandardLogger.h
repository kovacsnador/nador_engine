#ifndef __NADOR_STANDARD_LOGGER_H__
#define __NADOR_STANDARD_LOGGER_H__

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
    };

} // namespace nador

#endif //!__NADOR_STANDARD_LOGGER_H__