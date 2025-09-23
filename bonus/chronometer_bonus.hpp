/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chronometer_bonus.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:36:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHRONOMETER_BONUS_HPP
# define CHRONOMETER_BONUS_HPP

#include <chrono>
#include <vector>
#include <string>

/**
 * @brief Chronometer class for measuring durations using system time
 * 
 * Allows you to measure elapsed time with start/stop/pause functionality.
 * Supports lap times and multiple time format outputs.
 */
class Chronometer {
private:
    std::chrono::steady_clock::time_point _startTime;
    std::chrono::steady_clock::time_point _pauseTime;
    std::chrono::milliseconds _accumulatedTime;
    std::vector<std::chrono::milliseconds> _lapTimes;
    bool _isRunning;
    bool _isPaused;

public:
    // Constructors
    Chronometer();
    Chronometer(const Chronometer& other);
    Chronometer& operator=(const Chronometer& other);
    ~Chronometer();

    // Core functionality
    void start();
    void stop();
    void pause();
    void resume();
    void reset();
    void restart();

    // Lap functionality
    void recordLap();
    std::vector<long long> getLapTimesMs() const;
    long long getLastLapTimeMs() const;
    size_t getLapCount() const;
    void clearLaps();

    // Time measurement
    long long getElapsedMs() const;
    long long getElapsedSeconds() const;
    long long getElapsedMinutes() const;
    long long getElapsedHours() const;

    // Status checks
    bool isRunning() const;
    bool isPaused() const;
    bool isStopped() const;

    // Formatting
    std::string getFormattedTime() const; // HH:MM:SS.mmm
    std::string getFormattedTimeShort() const; // MM:SS.mmm
    std::string getFormattedTimeLong() const; // H hours, M minutes, S seconds, mmm milliseconds

    // Static utilities
    static std::string formatMilliseconds(long long ms);
    static long long getCurrentTimeMs();
};

#endif
