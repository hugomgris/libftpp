/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_bonus.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:22:10 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_BONUS_HPP
# define LOGGER_BONUS_HPP

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <mutex>
#include <chrono>
#include <functional>
#include <sstream>

/*
Logger class for comprehensive logging functionality

Provides multi-level logging with file and console output, filtering,
formatting, and thread-safe operations. Useful for debugging and monitoring.
*/
class Logger {
	public:
		// Log levels
		enum class Level {
			TRACE = 0,
			DEBUG = 1,
			INFO = 2,
			WARN = 3,
			ERROR = 4,
			FATAL = 5
		};

		// Log output destinations
		enum class Output {
			CONSOLE,
			FILE,
			BOTH,
			CUSTOM
		};

	private:
		struct LogEntry {
			Level level;
			std::string message;
			std::string category;
			std::chrono::system_clock::time_point timestamp;
			std::string file;
			int line;
			std::string function;
		};

		std::string _name;
		Level _minLevel;
		Output _output;
		std::string _logFile;
		std::ofstream _fileStream;
		mutable std::mutex _mutex;
		
		std::vector<LogEntry> _logHistory;
		size_t _maxHistorySize;
		bool _enableHistory;
		
		std::function<void(const LogEntry&)> _customOutput;
		std::vector<std::string> _categoryFilters;
		bool _useColorOutput;
		bool _showTimestamp;
		bool _showCategory;
		bool _showLevel;
		bool _showLocation;

	public:
		// Constructors
		Logger(const std::string& name = "Logger");
		Logger(const std::string& name, Level minLevel);
		Logger(const std::string& name, Level minLevel, Output output);
		Logger(const std::string& name, Level minLevel, const std::string& logFile);
		Logger(const Logger& other) = delete;
		Logger& operator=(const Logger& other) = delete;
		~Logger();

		// Basic logging methods
		void trace(const std::string& message, const std::string& category = "");
		void debug(const std::string& message, const std::string& category = "");
		void info(const std::string& message, const std::string& category = "");
		void warn(const std::string& message, const std::string& category = "");
		void error(const std::string& message, const std::string& category = "");
		void fatal(const std::string& message, const std::string& category = "");
		
		// Advanced logging with location
		void log(Level level, const std::string& message, const std::string& category = "",
				const std::string& file = "", int line = 0, const std::string& function = "");
		
		// Template logging for any type
		template<typename T>
		void log(Level level, const T& value, const std::string& category = "");
		
		// Configuration
		void setMinLevel(Level level);
		Level getMinLevel() const;
		void setOutput(Output output);
		Output getOutput() const;
		void setLogFile(const std::string& filename);
		const std::string& getLogFile() const;
		void setCustomOutput(std::function<void(const LogEntry&)> customOutput);
		
		// Formatting options
		void enableColorOutput(bool enable = true);
		void enableTimestamp(bool enable = true);
		void enableCategory(bool enable = true);
		void enableLevel(bool enable = true);
		void enableLocation(bool enable = true);
		
		// Category filtering
		void addCategoryFilter(const std::string& category);
		void removeCategoryFilter(const std::string& category);
		void clearCategoryFilters();
		bool isCategoryFiltered(const std::string& category) const;
		
		// History management
		void enableHistory(bool enable = true, size_t maxSize = 1000);
		void clearHistory();
		std::vector<LogEntry> getHistory() const;
		std::vector<LogEntry> getHistory(Level minLevel) const;
		std::vector<LogEntry> getHistory(const std::string& category) const;
		size_t getHistorySize() const;
		
		// Utility methods
		void flush();
		void close();
		const std::string& getName() const;
		void setName(const std::string& name);
		
		// Static utilities
		static std::string levelToString(Level level);
		static Level stringToLevel(const std::string& levelStr);
		static std::string getCurrentTimestamp();
		static Logger& getDefault();
		static void setDefault(std::unique_ptr<Logger> logger);
		
		// Stream-like interface
		class LogStream {
			Logger& _logger;
			Level _level;
			std::string _category;
			std::ostringstream _stream;
			
			public:
				LogStream(Logger& logger, Level level, const std::string& category = "");
				~LogStream();
				
				template<typename T>
				LogStream& operator<<(const T& value);
		};
	
	LogStream stream(Level level, const std::string& category = "");

	private:
		void writeLog(const LogEntry& entry);
		std::string formatLogEntry(const LogEntry& entry) const;
		std::string getColorCode(Level level) const;
		bool shouldLog(Level level, const std::string& category) const;
		void addToHistory(const LogEntry& entry);
		
		static std::unique_ptr<Logger> _defaultLogger;
};

// Convenience macros for logging with location info
#define LOG_TRACE(logger, message) logger.log(Logger::Level::TRACE, message, "", __FILE__, __LINE__, __FUNCTION__)
#define LOG_DEBUG(logger, message) logger.log(Logger::Level::DEBUG, message, "", __FILE__, __LINE__, __FUNCTION__)
#define LOG_INFO(logger, message) logger.log(Logger::Level::INFO, message, "", __FILE__, __LINE__, __FUNCTION__)
#define LOG_WARN(logger, message) logger.log(Logger::Level::WARN, message, "", __FILE__, __LINE__, __FUNCTION__)
#define LOG_ERROR(logger, message) logger.log(Logger::Level::ERROR, message, "", __FILE__, __LINE__, __FUNCTION__)
#define LOG_FATAL(logger, message) logger.log(Logger::Level::FATAL, message, "", __FILE__, __LINE__, __FUNCTION__)

// Convenience macros for default logger
#define TRACE(message) LOG_TRACE(Logger::getDefault(), message)
#define DEBUG(message) LOG_DEBUG(Logger::getDefault(), message)
#define INFO(message) LOG_INFO(Logger::getDefault(), message)
#define WARN(message) LOG_WARN(Logger::getDefault(), message)
#define ERROR(message) LOG_ERROR(Logger::getDefault(), message)
#define FATAL(message) LOG_FATAL(Logger::getDefault(), message)

#endif
