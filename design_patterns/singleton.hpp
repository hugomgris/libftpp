/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 16:31:13 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/16 17:21:23 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SINGLETON_HPP
# define SINGLETON_HPP

# include <memory>
# include <stdexcept>

class SingletonException : public std::exception {
	public:
		const char *what() const noexcept override {
			return "Singleton already instantiated";
		}
};

template<typename TType>
class Singleton {
	private:
		static std::unique_ptr<TType> instance_ptr;

	public:
		template<typename... TArgs>
		static void instantiate(TArgs&&... args) {
			if (instance_ptr) {
				throw SingletonException();
			}
			instance_ptr = std::unique_ptr<TType>(new TType(std::forward<TArgs>(args)...));
		}

		static TType *instance() {
			if (!instance_ptr) {
				throw std::runtime_error("Singleton not instantiated");
			}
			return (instance_ptr.get());
		}
};

template<typename TType>
std::unique_ptr<TType> Singleton<TType>::instance_ptr = nullptr;

#endif