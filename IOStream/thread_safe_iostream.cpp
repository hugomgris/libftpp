/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:00:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 13:57:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_safe_iostream.hpp"
#include <iostream>
#include <sstream>
#include <mutex>

// Static member definitions
std::mutex ThreadSafeIOStream::output_mutex;
thread_local std::string ThreadSafeIOStream::prefix = "";
thread_local std::ostringstream ThreadSafeIOStream::buffer;

// Global instance definition
ThreadSafeIOStream threadSafeCout;

// Method implementations
void ThreadSafeIOStream::setPrefix(const std::string &newPrefix) {
	prefix = newPrefix;
}

ThreadSafeIOStream &ThreadSafeIOStream::operator<<(std::ostream &(*manip)(std::ostream&)) {
	if (manip == static_cast<std::ostream &(*)(std::ostream&)>(std::endl)) {
		flushBuffer();
	} else {
		buffer << manip;
	}
	return (*this);
}

void ThreadSafeIOStream::flushBuffer() {
	std::lock_guard<std::mutex> lock(output_mutex);
	std::cout << prefix << buffer.str() << std::endl;
	buffer.str("");
	buffer.clear();
}
