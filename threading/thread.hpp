/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 11:19:19 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 13:57:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_HPP
# define THREAD_HPP

# include <thread>
# include <string>
# include <functional>
# include <atomic>

enum class ThreadState { NOT_STARTED, RUNNING, STOPPED };

class Thread {
	private:
		std::string _name;
		std::function<void()> _user_function;
		std::thread _actual_thread;
		std::atomic<ThreadState> _state;

		void _internalThreadFunction();

	public:
		Thread(const std::string &thread_name, std::function<void()> function);

		// Need to delete the copy constructor because the base (std::thread) is not copyable
		Thread(const Thread &) = delete;
		Thread &operator=(const Thread&) = delete;

		// Need to enable move constructor and assigment operator to manage Thread in a Vector (for reallocations)
		Thread(Thread &&other) noexcept;
		Thread &operator=(Thread &&other) noexcept;

		~Thread();

		void start();
		void stop();
		const std::string &getName() const;
		bool isRunning() const;
};
#endif