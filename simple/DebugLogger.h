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
  * Date: 23th of June 2021
  *
 * ------------------
 * CSMV1.1 - Codesmith License
 * Copyright(c) 1999 - 2021 Erno Pakarinen
 *
 *This licence is based on the MIT license model with very few exceptions.
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this softwareand associated documentation files(the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and /or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.Also the original
 * author shall be credited of the work related to this Software in all
 * software based on or using this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

  */
#ifndef __DEBUGLOGGER_DEFINED_H__
#define __DEBUGLOGGER_DEFINED_H__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <mutex>

namespace codesmith
{
	namespace debug
	{
		/**
		 * This mutex is to make logging thread safe.
		 * Mutex is locked in DebugLogger constructor and released in
		 * the destructor. Since the usage of the debug logger is through
		 * the provided macros (see bottom of this header) there should
		 * not be a possibility of a lock down.
		 * 
		 * Feel free to improve if you need to and send me comments please :)
		 */
		static std::mutex g_debuglogger_mutex;

		/**
		 * Defines the supported warning levels for the macro.
		 * The level defines a warning level text displayed by the macro
		 */
		enum class DebugLogLevel : int
		{
			EINFO = 0,
			EWARN = 1,
			EERROR = 2
		};

		/**
		 * DebugLogger class definition
		 * Class has stream output operation with operator <<
		 * Constructor creates the initial output with (or without) a time stamp
		 * and the desired warning level (severity).
		 * 
		 * Default warning level is ERROR
		 * 
		 * When class instance is destructed
		 */
		class DebugLogger
		{
		private: // constants
			static constexpr const char* KTextLevelInfo = "INFO";
			static constexpr const char* KTextLevelWarn = "WARN";
			static constexpr const char* KTextLevelError = "ERROR";

		public:
			DebugLogger(DebugLogLevel severity = DebugLogLevel::EERROR, bool showtime = true) 
				: m_buffer(), m_stm{}
			{	
				g_debuglogger_mutex.lock();

				if (showtime) {
					make_time();
					m_buffer << "[" << std::put_time(&m_stm, "%F %T") << "] ";
				}

				switch(severity) {
					case DebugLogLevel::EWARN:
					{
						m_buffer << KTextLevelWarn;
						break;
					}
				case DebugLogLevel::EERROR:
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

			// Destructor, causes the debug info to be outputted with new line
			virtual ~DebugLogger() {
				std::cerr << m_buffer.str() << std::endl;
				g_debuglogger_mutex.unlock();
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
				std::time_t t_now = std::chrono::system_clock::to_time_t(
					std::chrono::system_clock::now());
				localtime_s(&m_stm, &t_now);
			}

		private:
			std::ostringstream m_buffer;
			struct std::tm m_stm;
		};
	} // debug
} // codesmith

using namespace codesmith::debug;

/**
 * Helper macros/defines for using the DebugLogger
 */

// Default logger, severity level is ERROR, with a time stamp
#define LOG() DebugLogger()

// Default logger, severity level is ERROR, without a time stamp
#define LOG_NT() DebugLogger()

// These variants show system time
#define LOG_INFO() DebugLogger(DebugLogLevel::EINFO)
#define LOG_WARN() DebugLogger(DebugLogLevel::EWARN)
#define LOG_ERROR() DebugLogger(DebugLogLevel::EERROR)

// These variants omit the system time and only show the warning level
#define LOG_INFO_NT() DebugLogger(DebugLogLevel::EINFO, false)
#define LOG_WARN_NT() DebugLogger(DebugLogLevel::EWARN, false)
#define LOG_ERROR_NT() DebugLogger(DebugLogLevel::EERROR, false)

#endif // __DEBUGLOGGER_DEFINED_H__
