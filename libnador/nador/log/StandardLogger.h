#ifndef __NADOR_STANDARD_LOGGER_H__
#define __NADOR_STANDARD_LOGGER_H__

namespace nador
{
    class StandardLogger
	{
    public:
		void Debug(const char* logMsg) const noexcept;
		void Warning(const char* logMsg) const noexcept;
		void Error(const char* logMsg) const noexcept;
		void Fatal(const char* logMsg) const noexcept;

        void EngineDebug(const char* logMsg) const noexcept;
		void EngineWarning(const char* logMsg) const noexcept;
		void EngineError(const char* logMsg) const noexcept;
		void EngineFatal(const char* logMsg) const noexcept;
	};

}

#endif //!__NADOR_STANDARD_LOGGER_H__