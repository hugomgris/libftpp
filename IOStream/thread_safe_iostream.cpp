/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_safe_iostream.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 12:48:39 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/22 10:42:12 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <thread>

#include "thread_safe_iostream.hpp"
#include "../colors.h"

ThreadSafeIOStream threadSafeCout;

void testBasicFunctionality () {
	std::cout << YEL << "\n=== Testing Basic Functionality ===" << RESET << std::endl;
	threadSafeCout.setPrefix("[TEST]");
	threadSafeCout << "Basic output test" << std::endl;
	threadSafeCout << "Multiple values: " << 42 << " " << 3.14 << " " << true << std::endl;

	std::string text = "Hello World";
	threadSafeCout << "String: " << text << std::endl;

	threadSafeCout << "Chained: " << "A" << "B" << "C" << std::endl;

	std::cout << GRN << "Basic functionality achieved!" << RESET << std::endl;
}

void testThreadSafety() {
	std::cout << YEL << "\n=== Testing Thread Safety ===" << RESET << std::endl;

	const int NUM_THREADS = 5;
	const int MESSAGES_PER_THREAD = 3;

	std::vector<std::thread> threads;
	
	for (int i = 0; i < NUM_THREADS; ++i) {
		// Lambda function sent as task for threads
		threads.emplace_back([i]() {
			std::stringstream ss;
			
			std::string color;
			switch (i) {
				case 0: color = BLU; break;
				case 1: color = CYN; break;
				case 2: color = YEL; break;
				case 3: color = GRN; break;
				case 4: color = RED; break;
			}

			ss << color << "[THREAD-" << i << "] ";
			threadSafeCout.setPrefix(ss.str());

			for (int j = 0; j < MESSAGES_PER_THREAD; ++j) {
				threadSafeCout << "Message " << j << " from thread " << i << RESET << std::endl;

				// Delay to increase interleaving chances
				std::this_thread::sleep_for(std::chrono::milliseconds(10 + (i * 5)));
			}
		});
	}

	for (auto &thread : threads) {
		thread.join();
	}

	std::cout << BGRN << "Thread safety test completed - Lines clean as a whistle!" << RESET << std::endl;
}

void testDifferentPrefixes() {
	std::cout << YEL << "\n=== Testing different thread prefixes ===" << RESET << std::endl;

	std::vector<std::thread> threads;
	std::vector<std::string> prefixes = {
		"[DATABASE] ",
		"[NETWORK] ",
		"[FILE-IO] ",
		"[CACHE] "
	};

	for (size_t i = 0; i < prefixes.size(); ++i) {
		threads.emplace_back([i, &prefixes]() {
			std::string color;
			switch (i) {
				case 0: color = BLU; break;
				case 1: color = CYN; break;
				case 2: color = YEL; break;
				case 3: color = GRN; break;
				case 4: color = RED; break;
			}

			std::stringstream ss;
			ss << color << prefixes[i] << RESET;

			threadSafeCout.setPrefix(ss.str());

			threadSafeCout << "Initializing subsystem..." << std::endl;
			threadSafeCout << "Status: READY" << std::endl;
			threadSafeCout << "Operations completed: " << (i + 1) * 10 << std::endl;
		});
	}

	for (auto &thread : threads) {
		thread.join();
	}

	std::cout << GRN << "Different prefixes test completed!" << RESET << std::endl;
}

void testLongMessages() {
	std::cout << YEL << "\n=== Testing long messages ===" << RESET << std::endl;

	std::vector<std::thread> threads;

	for (int i = 0; i < 3; ++i) {
		threads.emplace_back([i](){
			std::stringstream ss;
			ss << "[LONG-" << i << "] ";
			threadSafeCout.setPrefix(ss.str());

			threadSafeCout << "This is a very long message with many parts: "
						<< "part1 " << "part2 " << "part3 " << "part4 " 
						<< "and numbers: " << 1 << " " << 2 << " " << 3
						<< " and more text at the end" << std::endl;
		});
	}

	for (auto &thread : threads) {
        thread.join();
    }

	std::cout << GRN << "Long messages test completed!" << RESET << std::endl;
}

void testMixedOperations() {
	std::cout << YEL << "\n=== Testing mixed operations ===" << RESET << std::endl;

	std::vector<std::thread> threads;

	// Thread 1: Regular Logging
	threads.emplace_back([](){
		std::stringstream ss;
		ss << RED << "[LOG] " << RESET;
		threadSafeCout.setPrefix(ss.str());
		for (int i = 0; i < 5; ++i) {
			threadSafeCout << "Log entry: " << i << " timestamp: " << std::time(nullptr) << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
	});

	// Thread 2: Error Reporting
	threads.emplace_back([](){
		std::stringstream ss;
		ss << BLU << "[ERROR] " << RESET;

		threadSafeCout.setPrefix(ss.str());
		threadSafeCout << "Critical error detected!!" << std::endl;
		threadSafeCout << "Error Code: " << 404 << std::endl;
		threadSafeCout << "Stack trace: main() -> function() -> error()" << std::endl;
	});

	// Thread 3: Progress reporting
	threads.emplace_back([](){
		std::stringstream ss;
		ss << MAG << "[PROGRESS] " << RESET;
		
		threadSafeCout.setPrefix(ss.str());
		for (int i = 0; i <= 100; i += 25) {
			threadSafeCout << "Processing..." << i << "% complete" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
	});

	for (auto &thread: threads) {
		thread.join();
	}

	std::cout << GRN << "Mixed operations test completed" << RESET << std::endl;
}

void testPromptFunctionality() {
	std::cout << YEL << "\n=== Testing prompt functionality ===" << RESET << std::endl;

	threadSafeCout.setPrefix("[INPUT] ");

	int number;
	threadSafeCout.prompt("Enter a number: ", number);
	threadSafeCout << "You entered: " << GRN << number << RESET << std::endl;

	std::string text;
	threadSafeCout.prompt("Enter some text: ", text);
	threadSafeCout << "You entered: " << GRN << text << RESET << std::endl;

	std::cout << GRN << "Prompt functionality test completed" << RESET << std::endl;
}

int main(void) {
	std::cout << CYN << "====== THREAD SAFE IOSTREAM tests ======" << RESET << std::endl;

	testBasicFunctionality();
	testThreadSafety();
	testDifferentPrefixes();
	testLongMessages();
	testMixedOperations();
	
	std::cout << CYN << "\n====== INTERACTIVE tests ======" << RESET << std::endl;
	testPromptFunctionality();

	std::cout << GRN << "\nAll tests completed successfully!" << std::endl;
	std::cout << GRN << "\nCheck the output above - all lines should be clean and properly prefixed!!" << RESET << std::endl;
}