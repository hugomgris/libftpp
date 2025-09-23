/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_bonus.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:36:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logger_bonus.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

// Static member definition
std::unique_ptr<Logger> Logger::_defaultLogger = nullptr;

// Constructors
Logger::Logger(const std::string& name)
    : _name(name), _minLevel(Level::INFO), _output(Output::CONSOLE)
    , _maxHistorySize(1000), _enableHistory(false)
    , _useColorOutput(true), _showTimestamp(true), _showCategory(true)
    , _showLevel(true), _showLocation(false) {}

Logger::Logger(const std::string& name, Level minLevel)
    : _name(name), _minLevel(minLevel), _output(Output::CONSOLE)
    , _maxHistorySize(1000), _enableHistory(false)
    , _useColorOutput(true), _showTimestamp(true), _showCategory(true)
    , _showLevel(true), _showLocation(false) {}

Logger::Logger(const std::string& name, Level minLevel, Output output)
    : _name(name), _minLevel(minLevel), _output(output)
    , _maxHistorySize(1000), _enableHistory(false)
    , _useColorOutput(true), _showTimestamp(true), _showCategory(true)
    , _showLevel(true), _showLocation(false) {}

Logger::Logger(const std::string& name, Level minLevel, const std::string& logFile)
    : _name(name), _minLevel(minLevel), _output(Output::FILE), _logFile(logFile)
    , _maxHistorySize(1000), _enableHistory(false)
    , _useColorOutput(false), _showTimestamp(true), _showCategory(true)
    , _showLevel(true), _showLocation(false) {
    _fileStream.open(_logFile, std::ios::app);
}

Logger::~Logger() {
    close();
}

// Basic logging methods
void Logger::trace(const std::string& message, const std::string& category) {
    log(Level::TRACE, message, category);
}

void Logger::debug(const std::string& message, const std::string& category) {
    log(Level::DEBUG, message, category);
}

void Logger::info(const std::string& message, const std::string& category) {
    log(Level::INFO, message, category);
}

void Logger::warn(const std::string& message, const std::string& category) {
    log(Level::WARN, message, category);
}

void Logger::error(const std::string& message, const std::string& category) {
    log(Level::ERROR, message, category);
}

void Logger::fatal(const std::string& message, const std::string& category) {
    log(Level::FATAL, message, category);
}

// Advanced logging with location
void Logger::log(Level level, const std::string& message, const std::string& category,
                 const std::string& file, int line, const std::string& function) {
    if (!shouldLog(level, category)) {
        return;
    }

    LogEntry entry;
    entry.level = level;
    entry.message = message;
    entry.category = category;
    entry.timestamp = std::chrono::system_clock::now();
    entry.file = file;
    entry.line = line;
    entry.function = function;

    std::lock_guard<std::mutex> lock(_mutex);
    writeLog(entry);
    
    if (_enableHistory) {
        addToHistory(entry);
    }
}

// Template logging for any type
template<typename T>
void Logger::log(Level level, const T& value, const std::string& category) {
    std::ostringstream oss;
    oss << value;
    log(level, oss.str(), category);
}

// Configuration
void Logger::setMinLevel(Level level) {
    std::lock_guard<std::mutex> lock(_mutex);
    _minLevel = level;
}

Logger::Level Logger::getMinLevel() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _minLevel;
}

void Logger::setOutput(Output output) {
    std::lock_guard<std::mutex> lock(_mutex);
    _output = output;
}

Logger::Output Logger::getOutput() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _output;
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_fileStream.is_open()) {
        _fileStream.close();
    }
    _logFile = filename;
    _fileStream.open(_logFile, std::ios::app);
}

const std::string& Logger::getLogFile() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _logFile;
}

void Logger::setCustomOutput(std::function<void(const LogEntry&)> customOutput) {
    std::lock_guard<std::mutex> lock(_mutex);
    _customOutput = customOutput;
}

// Formatting options
void Logger::enableColorOutput(bool enable) {
    std::lock_guard<std::mutex> lock(_mutex);
    _useColorOutput = enable;
}

void Logger::enableTimestamp(bool enable) {
    std::lock_guard<std::mutex> lock(_mutex);
    _showTimestamp = enable;
}

void Logger::enableCategory(bool enable) {
    std::lock_guard<std::mutex> lock(_mutex);
    _showCategory = enable;
}

void Logger::enableLevel(bool enable) {
    std::lock_guard<std::mutex> lock(_mutex);
    _showLevel = enable;
}

void Logger::enableLocation(bool enable) {
    std::lock_guard<std::mutex> lock(_mutex);
    _showLocation = enable;
}

// Category filtering
void Logger::addCategoryFilter(const std::string& category) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (std::find(_categoryFilters.begin(), _categoryFilters.end(), category) == _categoryFilters.end()) {
        _categoryFilters.push_back(category);
    }
}

void Logger::removeCategoryFilter(const std::string& category) {
    std::lock_guard<std::mutex> lock(_mutex);
    _categoryFilters.erase(
        std::remove(_categoryFilters.begin(), _categoryFilters.end(), category),
        _categoryFilters.end());
}

void Logger::clearCategoryFilters() {
    std::lock_guard<std::mutex> lock(_mutex);
    _categoryFilters.clear();
}

bool Logger::isCategoryFiltered(const std::string& category) const {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_categoryFilters.empty()) {
        return false; // No filters means all categories are allowed
    }
    return std::find(_categoryFilters.begin(), _categoryFilters.end(), category) != _categoryFilters.end();
}

// History management
void Logger::enableHistory(bool enable, size_t maxSize) {
    std::lock_guard<std::mutex> lock(_mutex);
    _enableHistory = enable;
    _maxHistorySize = maxSize;
    if (!enable) {
        _logHistory.clear();
    }
}

void Logger::clearHistory() {
    std::lock_guard<std::mutex> lock(_mutex);
    _logHistory.clear();
}

std::vector<Logger::LogEntry> Logger::getHistory() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _logHistory;
}

std::vector<Logger::LogEntry> Logger::getHistory(Level minLevel) const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<LogEntry> filtered;
    for (const auto& entry : _logHistory) {
        if (entry.level >= minLevel) {
            filtered.push_back(entry);
        }
    }
    return filtered;
}

std::vector<Logger::LogEntry> Logger::getHistory(const std::string& category) const {
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<LogEntry> filtered;
    for (const auto& entry : _logHistory) {
        if (entry.category == category) {
            filtered.push_back(entry);
        }
    }
    return filtered;
}

size_t Logger::getHistorySize() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _logHistory.size();
}

// Utility methods
void Logger::flush() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_fileStream.is_open()) {
        _fileStream.flush();
    }
    std::cout.flush();
}

void Logger::close() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_fileStream.is_open()) {
        _fileStream.close();
    }
}

const std::string& Logger::getName() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _name;
}

void Logger::setName(const std::string& name) {
    std::lock_guard<std::mutex> lock(_mutex);
    _name = name;
}

// Static utilities
std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::TRACE: return "TRACE";
        case Level::DEBUG: return "DEBUG";
        case Level::INFO:  return "INFO";
        case Level::WARN:  return "WARN";
        case Level::ERROR: return "ERROR";
        case Level::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

Logger::Level Logger::stringToLevel(const std::string& levelStr) {
    if (levelStr == "TRACE") return Level::TRACE;
    if (levelStr == "DEBUG") return Level::DEBUG;
    if (levelStr == "INFO")  return Level::INFO;
    if (levelStr == "WARN")  return Level::WARN;
    if (levelStr == "ERROR") return Level::ERROR;
    if (levelStr == "FATAL") return Level::FATAL;
    return Level::INFO; // Default
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

Logger& Logger::getDefault() {
    if (!_defaultLogger) {
        _defaultLogger = std::make_unique<Logger>("Default");
    }
    return *_defaultLogger;
}

void Logger::setDefault(std::unique_ptr<Logger> logger) {
    _defaultLogger = std::move(logger);
}

// LogStream implementation
Logger::LogStream::LogStream(Logger& logger, Level level, const std::string& category)
    : _logger(logger), _level(level), _category(category) {}

Logger::LogStream::~LogStream() {
    _logger.log(_level, _stream.str(), _category);
}

template<typename T>
Logger::LogStream& Logger::LogStream::operator<<(const T& value) {
    _stream << value;
    return *this;
}

Logger::LogStream Logger::stream(Level level, const std::string& category) {
    return LogStream(*this, level, category);
}

// Private methods
void Logger::writeLog(const LogEntry& entry) {
    std::string formatted = formatLogEntry(entry);
    
    switch (_output) {
        case Output::CONSOLE:
            std::cout << formatted << std::endl;
            break;
        case Output::FILE:
            if (_fileStream.is_open()) {
                _fileStream << formatted << std::endl;
            }
            break;
        case Output::BOTH:
            std::cout << formatted << std::endl;
            if (_fileStream.is_open()) {
                _fileStream << formatted << std::endl;
            }
            break;
        case Output::CUSTOM:
            if (_customOutput) {
                _customOutput(entry);
            }
            break;
    }
}

std::string Logger::formatLogEntry(const LogEntry& entry) const {
    std::ostringstream oss;
    
    // Color codes
    if (_useColorOutput && _output != Output::FILE) {
        oss << getColorCode(entry.level);
    }
    
    // Timestamp
    if (_showTimestamp) {
        auto time_t = std::chrono::system_clock::to_time_t(entry.timestamp);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            entry.timestamp.time_since_epoch()) % 1000;
        oss << "[" << std::put_time(std::localtime(&time_t), "%H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count() << "] ";
    }
    
    // Level
    if (_showLevel) {
        oss << "[" << levelToString(entry.level) << "] ";
    }
    
    // Category
    if (_showCategory && !entry.category.empty()) {
        oss << "[" << entry.category << "] ";
    }
    
    // Message
    oss << entry.message;
    
    // Location info
    if (_showLocation && !entry.file.empty()) {
        oss << " (" << entry.file;
        if (entry.line > 0) {
            oss << ":" << entry.line;
        }
        if (!entry.function.empty()) {
            oss << " in " << entry.function;
        }
        oss << ")";
    }
    
    // Reset color
    if (_useColorOutput && _output != Output::FILE) {
        oss << "\033[0m";
    }
    
    return oss.str();
}

std::string Logger::getColorCode(Level level) const {
    switch (level) {
        case Level::TRACE: return "\033[37m"; // White
        case Level::DEBUG: return "\033[36m"; // Cyan
        case Level::INFO:  return "\033[32m"; // Green
        case Level::WARN:  return "\033[33m"; // Yellow
        case Level::ERROR: return "\033[31m"; // Red
        case Level::FATAL: return "\033[35m"; // Magenta
        default: return "\033[0m"; // Reset
    }
}

bool Logger::shouldLog(Level level, const std::string& category) const {
    if (level < _minLevel) {
        return false;
    }
    
    if (!_categoryFilters.empty() && !category.empty()) {
        return isCategoryFiltered(category);
    }
    
    return true;
}

void Logger::addToHistory(const LogEntry& entry) {
    _logHistory.push_back(entry);
    if (_logHistory.size() > _maxHistorySize) {
        _logHistory.erase(_logHistory.begin());
    }
}

// Explicit template instantiations for common types
template void Logger::log<int>(Level level, const int& value, const std::string& category);
template void Logger::log<float>(Level level, const float& value, const std::string& category);
template void Logger::log<double>(Level level, const double& value, const std::string& category);
template Logger::LogStream& Logger::LogStream::operator<<<int>(const int& value);
template Logger::LogStream& Logger::LogStream::operator<<<float>(const float& value);
template Logger::LogStream& Logger::LogStream::operator<<<double>(const double& value);
template Logger::LogStream& Logger::LogStream::operator<<<std::string>(const std::string& value);
template Logger::LogStream& Logger::LogStream::operator<<<const char*>(const char* const& value);
