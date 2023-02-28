#ifndef __LOG_H__
#define __LOG_H__

#include <memory>
#include <mutex>
#include <cstdarg>
#include <functional>

#ifdef _DEBUG
#define ENABLE_NADOR_LOG
#define ENABLE_NADOR_ASSERT
#endif
#define NADOR_LOG_LEVEL 10
#define NADOR_LOG_MAX_MESSAGE_SIZE 2000

#define NADOR_ENGINE_LOG_LEVEL 10

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define __FUNCTION_NAME__ __PRETTY_FUNCTION__


#if defined(ENABLE_NADOR_LOG)
#define NADOR_DEBUG(LEVEL, ...)	nador::Log::Get().LogWrapper(nador::ELogType::DEBUG, __FILE__, __LINE__, __FUNCTION_NAME__ , LEVEL, __VA_ARGS__)
#define NADOR_WARNING(LEVEL, ...) nador::Log::Get().LogWrapper(nador::ELogType::WARNING, __FILE__, __LINE__, __FUNCTION_NAME__ , LEVEL, __VA_ARGS__)
#define NADOR_ERROR(LEVEL, ...)	nador::Log::Get().LogWrapper(nador::ELogType::ERROR, __FILE__, __LINE__, __FUNCTION_NAME__ , LEVEL, __VA_ARGS__)

#define ENGINE_DEBUG(...)	nador::Log::Get().LogWrapper(nador::ELogType::DEBUG, __FILE__, __LINE__, __FUNCTION_NAME__, NADOR_ENGINE_LOG_LEVEL, __VA_ARGS__)
#define ENGINE_WARNING(...) nador::Log::Get().LogWrapper(nador::ELogType::WARNING, __FILE__, __LINE__, __FUNCTION_NAME__, NADOR_ENGINE_LOG_LEVEL, __VA_ARGS__)
#define ENGINE_ERROR(...)	nador::Log::Get().LogWrapper(nador::ELogType::ERROR, __FILE__, __LINE__, __FUNCTION_NAME__, NADOR_ENGINE_LOG_LEVEL, __VA_ARGS__)
#else
#define NADOR_DEBUG(...)
#define NADOR_WARNING(...)
#define NADOR_ERROR(...)

#define ENGINE_DEBUG(...)
#define ENGINE_WARNING(...)
#define ENGINE_ERROR(...)
#endif

// Fatal always enabled
#define ENGINE_FATAL(...)	nador::Log::Get().LogWrapper(nador::ELogType::FATAL, __FILE__, __LINE__, __FUNCTION_NAME__, NADOR_ENGINE_LOG_LEVEL, __VA_ARGS__)
#define NADOR_FATAL(LEVEL, ...)	nador::Log::Get().LogWrapper(nador::ELogType::FATAL, __FILE__, __LINE__, __FUNCTION_NAME__ , LEVEL, __VA_ARGS__)

#define NADOR_LOG nador::Log::Get()

#ifdef ENABLE_NADOR_ASSERT

#define NADOR_ASSERT(condition)													\
		do {																	\
            if(!(condition)) {													\
                ENGINE_FATAL("ASSERT FAILED! Condition: ( %s )", #condition);	\
            }																	\
        } while(false)
#else

#define NADOR_ASSERT(condition)

#endif // ENABLE_NADOR_ASSERT

namespace nador
{
	enum class ELogType
	{
		DEBUG,
		WARNING,
		ERROR,
		FATAL,
	};

	inline const char* GetLogTypeName(ELogType type, int32_t level)
	{
		switch (type)
		{
		case ELogType::DEBUG:
			return level == NADOR_ENGINE_LOG_LEVEL ? "ENGINE_DEBUG" : "DEBUG";
		case ELogType::WARNING:
			return level == NADOR_ENGINE_LOG_LEVEL ? "ENGINE_WARNING" : "WARNING";
		case ELogType::ERROR:
			return level == NADOR_ENGINE_LOG_LEVEL ? "ENGINE_ERROR" : "ERROR";
		case ELogType::FATAL:
			return level == NADOR_ENGINE_LOG_LEVEL ? "ENGINE_FATAL" : "FATAL";
		default:
			return "UNKNOW";
		}
	}

	/*!
	 * Presents a simple logger class (singleton)
	 */
	class Log
	{
	public:

		using log_cb = std::function<void(const char* const)>;

		/*!
		 * Get the Log instance.
		 *
		 * \return The instance.
		 */
		static Log& Get()
		{
			static Log s_log;
			return s_log;
		}

		void RegisterCallback(ELogType type, log_cb callback)
		{
			switch (type)
			{
			case ELogType::DEBUG:
				_debug = callback;
				break;
			case ELogType::WARNING:
				_warning = callback;
				break;
			case ELogType::ERROR:
				_error = callback;
				break;
			case ELogType::FATAL:
				_fatal = callback;
				break;
			default:
				break;
			}
		}

		/*!
		 * Set the log level.
		 *
		 * \param level The level.
		 */
		void SetLevel(int32_t level) noexcept
		{
			_level = level;
		}

		/*!
		 * Enable funtion name to add to log message.
		 *
		 * \param enable	The enable flag.
		 */
		void EnableFuncName(bool enable) noexcept
		{
			_enableFuncName = enable;
		}

		/*!
		 * Enable file name to add to log message.
		 *
		 * \param enable	The enable flag.
		 */
		void EnableFileName(bool enable) noexcept
		{
			_enableFileName = enable;
		}

		/*!
		 * Enable line count to add to log message.
		 *
		 * \param enable	The enable flag.
		 */
		void EnableLine(bool enable) noexcept
		{
			_enableLine = enable;
		}

		template<typename... Args>
		void LogMessage(ELogType type, const char* file, int32_t line, const char* func, int32_t level, const std::string& msg, const Args&... args)
		{
			LogMessageImpl(type, file, line, func, level, msg.c_str(), args...);
		}

		template<typename... Args>
		void LogMessage(ELogType type, const char* file, int32_t line, const char* func, int32_t level, const char* msg, const Args&... args)
		{
			LogMessageImpl(type, file, line, func, level, msg, args...);
		}

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
		void LogMessageImpl(ELogType type, const char* file, int32_t line, const char* func, int32_t level, const char* msg, ...)
		{
			if(level <= _level)
			{
				const size_t buff_size = NADOR_LOG_MAX_MESSAGE_SIZE;
				char log_temp[buff_size];

				const char* functionName = _enableFuncName ? func : "";
				const char* fileName = _enableFileName ? file : "";
				
				int32_t next = 0;

				if (_enableLine)
				{
					next = snprintf(log_temp, buff_size, "[%s] {%s %s: %d} => ", GetLogTypeName(type, level), fileName, functionName, line);
				}
				else
				{
					next = snprintf(log_temp, buff_size, "[%s] {%s %s} => ", GetLogTypeName(type, level), fileName, functionName);
				}
				

				if(next < 0)
				{
					throw std::runtime_error("Encoding error occurs");
				}
				
				va_list va;
				va_start(va, msg);
				vsnprintf(log_temp + next, buff_size - next, msg, va);
				va_end(va);
				
				std::lock_guard<std::mutex> lock(_mtx);
				switch(type)
				{
				case ELogType::DEBUG:
					if(_debug)
					{
						_debug(log_temp);
					}
					break;
				case ELogType::WARNING:
					if(_warning)
					{
						_warning(log_temp);
					}
					break;
				case ELogType::ERROR:
					if(_error)
					{
						_error(log_temp);
					}
					break;
				case ELogType::FATAL:
					if (_fatal)
					{
						_fatal(log_temp);
					}
					throw std::runtime_error(log_temp);
					break;
				}
			}
		}


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
		template<typename... Args>
		void LogWrapper(ELogType type, const char* file, int32_t line, const char* func, int32_t level, const Args&... args)
		{
			LogMessage(type, file, line, func, level, args...);
		}


	private:
		/*!
		 * Log constructor
		 */
		Log()
		: _level(NADOR_LOG_LEVEL)
		{
		}

		std::mutex _mtx;

		int32_t	_level;

		log_cb	_debug;
		log_cb	_warning;
		log_cb	_error;
		log_cb	_fatal;

		bool _enableFuncName = true;
		bool _enableFileName = true;
		bool _enableLine = true;
	};
}

#endif //!__LOG_H__