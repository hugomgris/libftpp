/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_bonus.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:23:24 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_BONUS_HPP
# define TIMER_BONUS_HPP

#include <chrono>

/*
Timer class for setting durations and checking timeouts

Allows you to set a duration and check if it has timed out using system time.
Useful for game loops, timeouts, and periodic tasks.
*/
class Timer {
private:
	std::chrono::steady_clock::time_point _startTime;
	std::chrono::milliseconds _duration;
	bool _isRunning;

public:
	// Constructors
	Timer();
	Timer(long long durationMs);
	Timer(const Timer& other);
	Timer& operator=(const Timer& other);
	~Timer();

	// Core functionality
	void start();
	void start(long long durationMs);
	void stop();
	void reset();
	void restart();
	void restart(long long durationMs);

	// Status checks
	bool isRunning() const;
	bool hasTimedOut() const;
	long long getElapsedMs() const;
	long long getRemainingMs() const;
	double getProgress() const;

	// Duration management
	void setDuration(long long durationMs);
	long long getDuration() const;

	// Utility
	void waitUntilTimeout();
	static void sleep(long long milliseconds);
};

#endif
