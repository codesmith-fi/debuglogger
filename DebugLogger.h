 /**
  * Simple console debug logger macro
  * 
  * This is a simple macro which implements a debug logger with or without time stamp.
  * The logging can be used in streams with the stream '<<' operator
  * 
  * Usage with the time stamp:
  *		LOG_INFO() << "You failed " << 20 << "Times!"
  *		LOG_WARN() << "You failed " << 20 << "Times!"
  *		LOG_ERROR() << "You failed " << 20 << "Times!"
  *
  * Usage without the time stamp:
  *		LOG_INFO() << "You failed " << 20 << "Times!"
  *		LOG_WARN() << "You failed " << 20 << "Times!"
  *		LOG_ERROR() << "You failed " << 20 << "Times!"
  *
  * Author: Erno Pakarinen
  * Email: codesmith.fi@gmail.com
  * Date: 25th of May 2021
  *
  * MIT License
  * Free to use, modify, copy, what ever. <3
  */
#ifndef DEBUGLOGGER_DEFINED_H
#define DEBUGLOGGER_DEFINED_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

namespace codesmith
{
	namespace Debug
	{
		/**
		 * Defines the supported warning levels for the macro.
		 * The level defines a warning level text displayed by the macro
		 */
		enum class DebugLogLevel : int
		{
			INFO = 0,
			WARN = 1,
			ERROR = 2
		};

		class DebugLogger
		{
		private: // constants
			static constexpr const char* KTextLevelInfo = "INFO";
			static constexpr const char* KTextLevelWarn = "WARN";
			static constexpr const char* KTextLevelError = "ERROR";
		public:
			DebugLogger(DebugLogLevel severity = DebugLogLevel::ERROR, bool showtime = true) 
				: m_buffer(), m_stm{}
			{				
				if (showtime) {
					make_time();
					m_buffer << "[" << std::put_time(&m_stm, "%F %T") << "] ";
				}

				switch(severity) {
					case DebugLogLevel::WARN:
					{
						m_buffer << KTextLevelWarn;
						break;
					}
				case DebugLogLevel::ERROR:
					{
						m_buffer << KTextLevelError;
						break;
					}
					default:
					{
						m_buffer << KTextLevelInfo;
						break;
					}
				}
				m_buffer << ": ";
			}

			// Destructor, causes the debug info to be outputted
			virtual ~DebugLogger() {
				std::cerr << m_buffer.str() << std::endl;
			}

			/**
			 * Stream output operator, appends a value to the debug output stream
			 */
			template <typename T>
			DebugLogger& operator<<(const T& value) {
				m_buffer << value;
				return *this;
			}
		private:
			void make_time() {
				std::time_t t_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
				localtime_s(&m_stm, &t_now);
			}

		private:
			std::ostringstream m_buffer;
			struct std::tm m_stm;
		};
	}
}

using namespace codesmith::Debug;

/**
 * Helper macros/defines for using the DebugLogger
 */

// These variants show system time
#define LOG_INFO() DebugLogger(DebugLogLevel::INFO)
#define LOG_WARN() DebugLogger(DebugLogLevel::WARN)
#define LOG_ERROR() DebugLogger(DebugLogLevel::ERROR)

// These variants omit the system time and only show the warning level
#define LOG_INFO_NT() DebugLogger(DebugLogLevel::INFO, false)
#define LOG_WARN_NT() DebugLogger(DebugLogLevel::WARN, false)
#define LOG_ERROR_NT() DebugLogger(DebugLogLevel::ERROR, false)


#endif // DEBUGLOGGER_DEFINED_H
