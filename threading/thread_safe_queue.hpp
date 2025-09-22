/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_queue.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 09:09:51 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/22 11:03:23 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_QUEUE_HPP
# define THREAD_SAFE_QUEUE_HPP

# include <deque>
# include <mutex>
# include <stdexcept>

template<typename TType>
class ThreadSafeQueue {
	private:
		std::deque<TType> _queue;
		mutable std::mutex _mutex;

	public:
		// Required
		void push_back(const TType &element) {
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push_back(element);
		}

		void push_front(const TType &element) {
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.push_front(element);
		}

		TType pop_back() {
			std::lock_guard<std::mutex> lock(_mutex);
			if (_queue.empty()) {
				throw std::runtime_error("Queue is empty");
			}

			TType value = _queue.back();
			_queue.pop_back();
			return (value);
		}

		TType pop_front() {
			std::lock_guard<std::mutex> lock(_mutex);
			if (_queue.empty()) {
				throw std::runtime_error("Queue is empty");
			}

			TType value = _queue.front();
			_queue.pop_front();
			return (value);
		}

		// Utility
		bool empty() const {
			std::lock_guard<std::mutex> lock(_mutex);
			return (_queue.empty());
		}

		size_t size() const {
			std::lock_guard<std::mutex> lock(_mutex);
			return (_queue.size());
		}

		void clear(){
			std::lock_guard<std::mutex> lock(_mutex);
			_queue.clear();
		}
};

#endif