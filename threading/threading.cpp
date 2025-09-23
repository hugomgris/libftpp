/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 10:37:13 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 13:57:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>

#include "threading.hpp"
#include "../IOStream/thread_safe_iostream.hpp"
#include "../colors.h"

extern ThreadSafeIOStream threadSafeCout;

void testThreadSafeQueue() {
	std::cout << YEL << "\n=== Testing thread safe queue ===" << RESET << std::endl;

	ThreadSafeQueue<int> safe_queue;
	
	const int NUM_THREADS = 5;
	const int NUM_OPERATIONS = 5;
	std::vector<std::thread> threads;

	for (int i = 0; i < NUM_THREADS; ++i) {
		threads.emplace_back([i, &safe_queue](){
			std::stringstream ss;

			std::string color;
			switch (i) {
				case 0: color = BLU; break;
				case 1: color = CYN; break;
				case 2: color = YEL; break;
				case 3: color = GRN; break;
				case 4: color = RED; break;
			}

			ss << color << "[THREAD-" << i << "] " << RESET;
			threadSafeCout.setPrefix(ss.str());

			for (int j = 0; j < NUM_OPERATIONS; ++j) {
				int value;
				value = i * 10 + j;
				threadSafeCout << "Pushing value " << color << value << RESET << " into front" << std::endl;
				
				safe_queue.push_front(value);

				std::this_thread::sleep_for(std::chrono::milliseconds(10 + (i * 5)));
			}

			for (int k = 0; k < NUM_OPERATIONS; ++k) {
				int value;
				value = i * 10 + k + 5;

				threadSafeCout << "Pushing value " << color << value << RESET << " into back" << std::endl;
				
				safe_queue.push_front(value);
				std::this_thread::sleep_for(std::chrono::milliseconds(10 + (i * 5)));
			}

			for (int l = 0; l < NUM_OPERATIONS; ++l) {
				threadSafeCout << "Popping from front" << std::endl;
				
				safe_queue.pop_front();
				std::this_thread::sleep_for(std::chrono::milliseconds(10 + (i * 5)));
			}

			for (int m = 0; m < NUM_OPERATIONS; ++m) {
				threadSafeCout << "Popping from back" << std::endl;
				
				safe_queue.pop_back();
				std::this_thread::sleep_for(std::chrono::milliseconds(10 + (i * 5)));
			}
		});
	}

	for (auto &thread : threads) {
		thread.join();
	}
}

void testThreadSafeQueueException() {
	std::cout << YEL << "\n=== Testing thread safe queue exception (empty popping) ===" << RESET << std::endl;

	ThreadSafeQueue<int> safe_queue;
	
	try {
		std::cout << "Popping from back" << std::endl;
		
		safe_queue.pop_back();
	} catch (const std::runtime_error &e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
	}

	try {
		std::cout << "Popping from front" << std::endl;
		
		safe_queue.pop_front();
	} catch (const std::runtime_error &e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
	}
}

void testThreadWrapper() {
	std::cout << YEL << "\n=== Testing thread wrapper ===" << RESET << std::endl;

	std::vector<Thread> threads;
	int THREAD_NUMBER = 5;

	for (int i = 0; i < THREAD_NUMBER; ++i) {
		std::string threadName = "WORKER-" + std::to_string(i);

		threads.emplace_back(threadName, [threadName](){
			threadSafeCout << BLU << "Working on task " << rand() % 100 << RESET << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			threadSafeCout << CYN << "Task completed!" << RESET << std::endl;
		});
	}

	for (auto &thread : threads) {
		thread.start();
	}

	for (auto &thread : threads) {
		thread.stop();
	}

	std::cout << GRN << "Thread wrapper test completed!" << RESET << std::endl;
}

void testWorkerPool() {
	std::cout << YEL << "\n=== Testing worker pool ===" << RESET << std::endl;

	WorkerPool pool(4);

	for (int i = 0; i < 20; ++i) {
		pool.addJob([i](){
			threadSafeCout << CYN << "Job " << i << " done!" << RESET << std::endl;
		});
		threadSafeCout << BLU << "Job " << i << " added to the pool!" << RESET << std::endl;
	}

	while (pool.getQueueSize() > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << "Queue size: " << pool.getQueueSize() << std::endl;
	}
	
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	
	std::cout << GRN << "Worker pool test completed!" << RESET << std::endl;
}

void testPersistentWorker() {
	std::cout << YEL << "\n=== Testing worker pool ===" << RESET << std::endl;

	PersistentWorker persistentWorker;

	persistentWorker.addTask("PrintLetters", [](){
		threadSafeCout << "Printing letters: ABCDE" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	});

	persistentWorker.addTask("PrintNumbers", [](){
		threadSafeCout << "Printing numbers: 12345" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	});

	persistentWorker.start();

	std::this_thread::sleep_for(std::chrono::seconds(3));

	persistentWorker.addTask("PrintSymbols", [](){
		threadSafeCout << "Printing symbols: @*#*%" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	});

	std::this_thread::sleep_for(std::chrono::seconds(3));

	persistentWorker.removeTask("PrintNumbers");
	
	std::this_thread::sleep_for(std::chrono::seconds(3));

	persistentWorker.stop();
}

int main(void) {
	std::cout << CYN << "====== THREAD SAFE IOSTREAM tests ======" << RESET << std::endl;

	testThreadSafeQueue();
	testThreadSafeQueueException();
	testThreadWrapper();
	testWorkerPool();
	testPersistentWorker();

	std::cout << GRN << "\nAll tests completed successfully!" << std::endl;
}