/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:19:19 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/22 14:39:56 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_HPP
# define THREAD_HPP

# include <thread>
# include <string>
# include <functional>
# include <atomic>

# include "../IOStream/thread_safe_iostream.hpp"
# include "../colors.h"

extern ThreadSafeIOStream threadSafeCout;

enum class ThreadState { NOT_STARTED, RUNNING, STOPPED };

class Thread {
	private:
		std::string _name;
		std::function<void()> _user_function;
		std::thread _actual_thread;
		std::atomic<ThreadState> _state;

		void _internalThreadFunction() {
			threadSafeCout.setPrefix("[" + _name + "]");
			_user_function();
		}

	public:
		Thread(const std::string &thread_name, std::function<void()> function): _name(thread_name), _user_function(function), _state(ThreadState::NOT_STARTED) {
			threadSafeCout << "Thread with name " << _name << RED << " created!" << RESET << std::endl;
		}

		// Need to delete the copy constructor because the base (std::thread) is not copyable
		Thread(const Thread &) = delete;
		Thread &operator=(const Thread&) = delete;

		// Need to enable move constructor and assigment operator to manage Thread in a Vector (for reallocations)
		Thread(Thread &&other) noexcept:
			_name(std::move(other._name)),
			_user_function(std::move(other._user_function)),
			_actual_thread(std::move(other._actual_thread)),
			_state(other._state.load()) {
			other._state = ThreadState::STOPPED;
		}

		Thread &operator=(Thread &&other) noexcept {
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

		~Thread() {
			if (_state == ThreadState::RUNNING) {
				stop();
			}
		}

		void start() {
			ThreadState expected = ThreadState::NOT_STARTED;
			if(_state.compare_exchange_strong(expected, ThreadState::RUNNING)) {
				threadSafeCout << "Thread with name " << _name << YEL << " started!" << RESET << std::endl;
				_actual_thread = std::thread(&Thread::_internalThreadFunction, this);
			}
		}

		void stop() {
			ThreadState expected = ThreadState::RUNNING;
			if (_state.compare_exchange_strong(expected, ThreadState::STOPPED)) {
				if (_actual_thread.joinable()) {
					_actual_thread.join();
				}
			}
		}

		const std::string &getName() const { return _name; }
		bool isRunning() const { return _state == ThreadState::RUNNING ? true : false; }
};
#endif