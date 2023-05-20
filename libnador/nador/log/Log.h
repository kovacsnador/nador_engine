#ifndef __NADOR_LOG_H__
#define __NADOR_LOG_H__

#include <memory>
#include <mutex>
#include <cstdarg>
#include <functional>

#define NADOR_LOG_MAX_MESSAGE_SIZE 2000

#ifndef __PRETTY_FUNCTION__
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define __FUNCTION_NAME__ __PRETTY_FUNCTION__

namespace nador
{
	enum class ELogLevel : uint32_t
	{
		DEBUG = 0,
		WARNING,
		ERROR,
		FATAL,
	};
}


#if defined(DISABLE_NADOR_LOG)
#define NADOR_DEBUG(...)
#define NADOR_WARNING(...)
#define NADOR_ERROR(...)

#define ENGINE_DEBUG(...)
#define ENGINE_WARNING(...)
#define ENGINE_ERROR(...)
#else
#define NADOR_DEBUG(...)	nador::Log::Get().LogWrapper(nador::ELogType::DEBUG, __FILE__, __LINE__, __FUNCTION_NAME__ , nador::ELogLevel::DEBUG, __VA_ARGS__)
#define NADOR_WARNING(...) nador::Log::Get().LogWrapper(nador::ELogType::WARNING, __FILE__, __LINE__, __FUNCTION_NAME__ , nador::ELogLevel::WARNING, __VA_ARGS__)
#define NADOR_ERROR(...)	nador::Log::Get().LogWrapper(nador::ELogType::ERROR, __FILE__, __LINE__, __FUNCTION_NAME__ , nador::ELogLevel::ERROR, __VA_ARGS__)

#define ENGINE_DEBUG(...)	nador::Log::Get().LogWrapper(nador::ELogType::ENGINE_DEBUG, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::DEBUG, __VA_ARGS__)
#define ENGINE_WARNING(...) nador::Log::Get().LogWrapper(nador::ELogType::ENGINE_WARNING, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::WARNING, __VA_ARGS__)
#define ENGINE_ERROR(...)	nador::Log::Get().LogWrapper(nador::ELogType::ENGINE_ERROR, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::ERROR, __VA_ARGS__)

#endif // DISABLE_NADOR_LOG

// Fatal always enabled
#define ENGINE_FATAL(...)	nador::Log::Get().LogWrapper(nador::ELogType::ENGINE_FATAL, __FILE__, __LINE__, __FUNCTION_NAME__, nador::ELogLevel::FATAL, __VA_ARGS__)
#define NADOR_FATAL(...)	nador::Log::Get().LogWrapper(nador::ELogType::FATAL, __FILE__, __LINE__, __FUNCTION_NAME__ , nador::ELogLevel::FATAL, __VA_ARGS__)

#define NADOR_LOG nador::Log::Get()

#ifdef DISABLE_NADOR_ASSERT

#define NADOR_ASSERT(condition)
#define NADOR_ASSERT_MSG(condition, msg)

#else

#define NADOR_ASSERT(condition)													\
		do {																	\
            if(!(condition)) {													\
                ENGINE_FATAL("ASSERT FAILED! Condition: ( %s )", #condition);	\
			}																	\
        } while(false)

#define NADOR_ASSERT_MSG(condition, msg)														\
		do {																					\
            if(!(condition)) {																	\
                ENGINE_FATAL("ASSERT FAILED! Condition: ( %s ), Message: %s", #condition, msg);	\
			}																					\
        } while(false)

#endif // DISABLE_NADOR_ASSERT

namespace nador
{
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

		/*!
		 * Registers a callback touched to a log type.
		 *
		 * \param type The log type.
		 * \param callback The callback.
		 * 
		 * \return  True if inserted.
		 */
		bool RegisterCallback(ELogType type, log_cb callback);

		/*!
		 * Set the log level.
		 *
		 * \param level The level.
		 */
		void SetLevel(nador::ELogLevel level) noexcept;

		/*!
		 * Enable funtion name to add to log message.
		 *
		 * \param enable	The enable flag.
		 */
		void EnableFuncName(bool enable) noexcept;

		/*!
		 * Enable file name to add to log message.
		 *
		 * \param enable	The enable flag.
		 */
		void EnableFileName(bool enable) noexcept;

		/*!
		 * Enable line count to add to log message.
		 *
		 * \param enable	The enable flag.
		 */
		void EnableLine(bool enable) noexcept;

		template<typename... Args>
		void LogMessage(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, const std::string& msg, const Args&... args)
		{
			LogMessageImpl(type, file, line, func, level, msg.c_str(), args...);
		}

		template<typename... Args>
		void LogMessage(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, const char* msg, const Args&... args)
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
		void LogMessageImpl(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, const char* msg, ...);


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
		void LogWrapper(ELogType type, const char* file, int32_t line, const char* func, nador::ELogLevel level, const Args&... args)
		{
			LogMessage(type, file, line, func, level, args...);
		}

		// delete copy stuffs
        Log(const Log&) = delete;
        Log& operator=(const Log&) = delete;

	private:
		/*!
		 * Log constructor
		 */
		Log();

		std::mutex _mtx;

		ELogLevel	_level{ELogLevel::DEBUG};

		std::unordered_map<ELogType, log_cb> _logCallbacks;

		bool _enableFuncName = true;
		bool _enableFileName = true;
		bool _enableLine = true;
	};
}

#endif //!__NADOR_LOG_H__