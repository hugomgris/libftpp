/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:00:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 00:00:00 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "worker_pool.hpp"
#include "../colors.h"
#include "../IOStream/thread_safe_iostream.hpp"

extern ThreadSafeIOStream threadSafeCout;

void WorkerPool::_workerFunction(int workerId) {
	while(!_shutdown) {
		try {
			threadSafeCout << MAG << "Worker " << workerId << " assigned to job when queue has size " << _jobQueue.size() << "!" << RESET << std::endl;
			auto job = _jobQueue.pop_front();

			job();
		} catch (const std::runtime_error &e) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		} catch (...) {
			threadSafeCout << "Job execution failed" << std::endl;
		}
	}
}

WorkerPool::WorkerPool(size_t numWorkers): _shutdown(false) {
	if (numWorkers == 0) {
		numWorkers = std::thread::hardware_concurrency();
	}
	
	_workers.reserve(numWorkers);

	for (size_t i = 0; i < numWorkers; ++i) {
		std::string workerName = "Worker-" + std::to_string(i);

		_workers.emplace_back(workerName, [this, i](){
			this->_workerFunction(i);
		});
	}

	for (auto &worker : _workers) {
		worker.start();
	}
}

WorkerPool::~WorkerPool() { 
	shutdownPool(); 
}

void WorkerPool::addJob(const std::function<void()> & jobToExecute) {
	if (!_shutdown) {
		_jobQueue.push_back(jobToExecute);
	}
}

void WorkerPool::addJob(std::unique_ptr<IJobs> job) {
	if (!_shutdown) {
		std::shared_ptr<IJobs> sharedJob = std::move(job);
		
		_jobQueue.push_back([sharedJob](){
			sharedJob->execute();
		});
	}
}

void WorkerPool::shutdownPool() {
	_shutdown = true;

	for (auto &worker : _workers) {
		worker.stop();
	}
}

size_t WorkerPool::getQueueSize() const { 
	return _jobQueue.size(); 
}

bool WorkerPool::isShutdown() const { 
	return _shutdown; 
}
