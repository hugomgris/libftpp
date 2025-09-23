/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 12:31:54 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 14:06:41 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_SAFE_IOSTREAM
# define THREAD_SAFE_IOSTREAM

# include <iostream>
# include <sstream>
# include <mutex>
# include <string>

class ThreadSafeIOStream {
	private:
		static std::mutex output_mutex;
		static thread_local std::string prefix;
		static thread_local std::ostringstream buffer;
	
	public:
		void setPrefix(const std::string &newPrefix);

		template<typename T>
		ThreadSafeIOStream &operator<<(const T &value) {
			buffer << value;
			return *this;
		}

		// Overload of std::endl to manage mutexing and flushing
		ThreadSafeIOStream &operator<<(std::ostream &(*manip)(std::ostream&));

		template<typename T>
		void prompt(const std::string &question, T &dest);

	private:
		void flushBuffer();
};

// Template method implementations
template<typename T>
void ThreadSafeIOStream::prompt(const std::string &question, T &dest){
	{
		std::lock_guard<std::mutex> loc(output_mutex);
		std::cout << prefix << question;
		std::cout.flush();
	}
	std::cin >> dest;
}

// Global instance required by subject
extern ThreadSafeIOStream threadSafeCout;

#endif