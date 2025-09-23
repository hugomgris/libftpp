/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronometer_bonus.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:36:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chronometer_bonus.hpp"
#include <sstream>
#include <iomanip>

// Constructors
Chronometer::Chronometer() 
    : _accumulatedTime(0), _isRunning(false), _isPaused(false) {}

Chronometer::Chronometer(const Chronometer& other)
    : _startTime(other._startTime)
    , _pauseTime(other._pauseTime)
    , _accumulatedTime(other._accumulatedTime)
    , _lapTimes(other._lapTimes)
    , _isRunning(other._isRunning)
    , _isPaused(other._isPaused) {}

Chronometer& Chronometer::operator=(const Chronometer& other) {
    if (this != &other) {
        _startTime = other._startTime;
        _pauseTime = other._pauseTime;
        _accumulatedTime = other._accumulatedTime;
        _lapTimes = other._lapTimes;
        _isRunning = other._isRunning;
        _isPaused = other._isPaused;
    }
    return *this;
}

Chronometer::~Chronometer() {}

// Core functionality
void Chronometer::start() {
    _startTime = std::chrono::steady_clock::now();
    _isRunning = true;
    _isPaused = false;
    _accumulatedTime = std::chrono::milliseconds(0);
}

void Chronometer::stop() {
    if (_isRunning && !_isPaused) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _startTime);
        _accumulatedTime += elapsed;
    }
    _isRunning = false;
    _isPaused = false;
}

void Chronometer::pause() {
    if (_isRunning && !_isPaused) {
        _pauseTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(_pauseTime - _startTime);
        _accumulatedTime += elapsed;
        _isPaused = true;
    }
}

void Chronometer::resume() {
    if (_isRunning && _isPaused) {
        _startTime = std::chrono::steady_clock::now();
        _isPaused = false;
    }
}

void Chronometer::reset() {
    _isRunning = false;
    _isPaused = false;
    _accumulatedTime = std::chrono::milliseconds(0);
    _lapTimes.clear();
}

void Chronometer::restart() {
    reset();
    start();
}

// Lap functionality
void Chronometer::recordLap() {
    long long currentTime = getElapsedMs();
    _lapTimes.push_back(std::chrono::milliseconds(currentTime));
}

std::vector<long long> Chronometer::getLapTimesMs() const {
    std::vector<long long> lapTimes;
    for (const auto& lap : _lapTimes) {
        lapTimes.push_back(lap.count());
    }
    return lapTimes;
}

long long Chronometer::getLastLapTimeMs() const {
    if (_lapTimes.empty()) {
        return 0;
    }
    return _lapTimes.back().count();
}

size_t Chronometer::getLapCount() const {
    return _lapTimes.size();
}

void Chronometer::clearLaps() {
    _lapTimes.clear();
}

// Time measurement
long long Chronometer::getElapsedMs() const {
    if (!_isRunning) {
        return _accumulatedTime.count();
    }
    
    if (_isPaused) {
        return _accumulatedTime.count();
    }
    
    auto now = std::chrono::steady_clock::now();
    auto currentElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _startTime);
    return _accumulatedTime.count() + currentElapsed.count();
}

long long Chronometer::getElapsedSeconds() const {
    return getElapsedMs() / 1000;
}

long long Chronometer::getElapsedMinutes() const {
    return getElapsedSeconds() / 60;
}

long long Chronometer::getElapsedHours() const {
    return getElapsedMinutes() / 60;
}

// Status checks
bool Chronometer::isRunning() const {
    return _isRunning;
}

bool Chronometer::isPaused() const {
    return _isPaused;
}

bool Chronometer::isStopped() const {
    return !_isRunning;
}

// Formatting
std::string Chronometer::getFormattedTime() const {
    long long totalMs = getElapsedMs();
    long long hours = totalMs / 3600000;
    long long minutes = (totalMs % 3600000) / 60000;
    long long seconds = (totalMs % 60000) / 1000;
    long long milliseconds = totalMs % 1000;
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << "."
        << std::setfill('0') << std::setw(3) << milliseconds;
    return oss.str();
}

std::string Chronometer::getFormattedTimeShort() const {
    long long totalMs = getElapsedMs();
    long long minutes = totalMs / 60000;
    long long seconds = (totalMs % 60000) / 1000;
    long long milliseconds = totalMs % 1000;
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << "."
        << std::setfill('0') << std::setw(3) << milliseconds;
    return oss.str();
}

std::string Chronometer::getFormattedTimeLong() const {
    long long totalMs = getElapsedMs();
    long long hours = totalMs / 3600000;
    long long minutes = (totalMs % 3600000) / 60000;
    long long seconds = (totalMs % 60000) / 1000;
    long long milliseconds = totalMs % 1000;
    
    std::ostringstream oss;
    if (hours > 0) {
        oss << hours << " hours, ";
    }
    if (minutes > 0 || hours > 0) {
        oss << minutes << " minutes, ";
    }
    oss << seconds << " seconds, " << milliseconds << " milliseconds";
    return oss.str();
}

// Static utilities
std::string Chronometer::formatMilliseconds(long long ms) {
    long long hours = ms / 3600000;
    long long minutes = (ms % 3600000) / 60000;
    long long seconds = (ms % 60000) / 1000;
    long long milliseconds = ms % 1000;
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds << "."
        << std::setfill('0') << std::setw(3) << milliseconds;
    return oss.str();
}

long long Chronometer::getCurrentTimeMs() {
    auto now = std::chrono::steady_clock::now();
    auto epoch = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    return milliseconds.count();
}
