/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:40:57 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/16 16:06:02 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBSERVER_HPP
# define OBSERVER_HPP

# include <iostream>
# include <map>
# include <functional>

// Careful the TEvent passed MUST be comparable to work as a map key (i.e. support operator<)
// Observer with data catching capabilities
template<typename TEvent, typename TData = void>
class Observer {
	private:
		std::multimap<TEvent, std::function<void(const TData&)>> suscribers;

	public:
		void subscribe(const TEvent &event, const std::function<void(const TData&)> &callback) {
			suscribers.emplace(event, callback);
		}

		void notify(const TEvent &event, const TData &data) {
			auto range = suscribers.equal_range(event);
			for (auto it = range.first; it != range.second; ++it) {
				try {
					it->second(data);
				} catch (...) {
					std::cerr << "A suscribed callback threw an exception. Continuing with the rest" << std::endl;
				}
			}
		}
};

// void specialization (i.e. no data)
template<typename TEvent>
class Observer<TEvent, void> {
private:
	std::multimap<TEvent, std::function<void()>> subscribers;

public:
	void subscribe(const TEvent &event, const std::function<void()> &callback) {
		subscribers.emplace(event, callback);
	}

	void notify(const TEvent &event) {
		auto range = subscribers.equal_range(event);
		for (auto it = range.first; it != range.second; ++it) {
			try {
				it->second();
			} catch (...) {
				std::cerr << "A subscribed callback threw an exception. Continuing with the rest" << std::endl;
			}
		}
	}
};

#endif