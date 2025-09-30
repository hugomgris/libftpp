/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_bonus.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:22:50 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timer_bonus.hpp"
#include <thread>
#include <algorithm>

// Constructors
Timer::Timer() : _duration(1000), _isRunning(false) {}

Timer::Timer(long long durationMs) : _duration(durationMs), _isRunning(false) {}

Timer::Timer(const Timer& other) 
	: _startTime(other._startTime), _duration(other._duration), _isRunning(other._isRunning) {}

Timer& Timer::operator=(const Timer& other) {
	if (this != &other) {
		_startTime = other._startTime;
		_duration = other._duration;
		_isRunning = other._isRunning;
	}
	return *this;
}

Timer::~Timer() {}

// Core functionality
void Timer::start() {
	_startTime = std::chrono::steady_clock::now();
	_isRunning = true;
}

void Timer::start(long long durationMs) {
	_duration = std::chrono::milliseconds(durationMs);
	start();
}

void Timer::stop() {
	_isRunning = false;
}

void Timer::reset() {
	_isRunning = false;
}

void Timer::restart() {
	start();
}

void Timer::restart(long long durationMs) {
	_duration = std::chrono::milliseconds(durationMs);
	start();
}

// Status checks
bool Timer::isRunning() const {
	return _isRunning;
}

bool Timer::hasTimedOut() const {
	if (!_isRunning) {
		return false;
	}
	return getElapsedMs() >= _duration.count();
}

long long Timer::getElapsedMs() const {
	if (!_isRunning) {
		return 0;
	}
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _startTime);
	return elapsed.count();
}

long long Timer::getRemainingMs() const {
	if (!_isRunning) {
		return _duration.count();
	}
	long long remaining = _duration.count() - getElapsedMs();
	return std::max(0LL, remaining);
}

double Timer::getProgress() const {
	if (!_isRunning || _duration.count() == 0) {
		return 0.0;
	}
	double progress = static_cast<double>(getElapsedMs()) / static_cast<double>(_duration.count());
	return std::min(1.0, std::max(0.0, progress));
}

// Duration management
void Timer::setDuration(long long durationMs) {
	_duration = std::chrono::milliseconds(durationMs);
}

long long Timer::getDuration() const {
	return _duration.count();
}

// Utility
void Timer::waitUntilTimeout() {
	if (!_isRunning) {
		return;
	}
	
	while (!hasTimedOut()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void Timer::sleep(long long milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
