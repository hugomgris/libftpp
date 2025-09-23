/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:35:25 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 14:05:53 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread.hpp"
#include "../colors.h"
#include <iostream>

// Thread method implementations
Thread::Thread(const std::string &thread_name, std::function<void()> function)
	: _name(thread_name), _user_function(function), _state(ThreadState::NOT_STARTED) {
	std::cout << "Thread with name " << _name << RED << " created!" << RESET << std::endl;
}

Thread::Thread(Thread &&other) noexcept
	: _name(std::move(other._name)),
	  _user_function(std::move(other._user_function)),
	  _actual_thread(std::move(other._actual_thread)),
	  _state(other._state.load()) {
	other._state = ThreadState::STOPPED;
}

Thread &Thread::operator=(Thread &&other) noexcept {
	if (this != &other) {
		if (_state == ThreadState::RUNNING) {
			stop();
		}

		_name = std::move(other._name);
		_user_function = std::move(other._user_function);
		_actual_thread = std::move(other._actual_thread);
		_state = other._state.load();
		other._state = ThreadState::STOPPED;
	}

	return (*this);
}

Thread::~Thread() {
	if (_state == ThreadState::RUNNING) {
		stop();
	}
}

void Thread::_internalThreadFunction() {
	std::cout << "[" << _name << "] Thread executing..." << std::endl;
	_user_function();
}

void Thread::start() {
	ThreadState expected = ThreadState::NOT_STARTED;
	if(_state.compare_exchange_strong(expected, ThreadState::RUNNING)) {
		std::cout << "Thread with name " << _name << YEL << " started!" << RESET << std::endl;
		_actual_thread = std::thread(&Thread::_internalThreadFunction, this);
	}
}

void Thread::stop() {
	ThreadState expected = ThreadState::RUNNING;
	if (_state.compare_exchange_strong(expected, ThreadState::STOPPED)) {
		if (_actual_thread.joinable()) {
			_actual_thread.join();
		}
	}
}

const std::string &Thread::getName() const { 
	return _name; 
}

bool Thread::isRunning() const { 
	return _state == ThreadState::RUNNING ? true : false; 
}
