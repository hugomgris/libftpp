/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:00:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 00:00:00 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "persistent_worker.hpp"
#include "../colors.h"
#include "../IOStream/thread_safe_iostream.hpp"

extern ThreadSafeIOStream threadSafeCout;

void PersistentWorker::workerLoop() {
	threadSafeCout << CYN << "PersistentWorker started and running..." << RESET << std::endl;
	
	while (_running) {
		std::vector<std::pair<std::string, std::function<void()>>> currentTasks;

		{
			std::lock_guard<std::mutex> lock(_tasksMutex);
			currentTasks.assign(_tasks.begin(), _tasks.end());
		}

		if (currentTasks.empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		for (const auto &[name, task] : currentTasks) {
			if (!_running) break;
			executeTaskSafely(name, task);
		}
	}

	threadSafeCout << CYN << "PersistentWorker stopped" << RESET << std::endl;
}

void PersistentWorker::executeTaskSafely(const std::string &name, const std::function<void()> &task) {
	try {
		threadSafeCout << BLU << "Executing persistent task: " << name << RESET << std::endl;
		task();
		threadSafeCout << GRN << "Task '" << name << "' completed succesfully" << RESET << std::endl;
	} catch (const std::exception &e) {
		threadSafeCout << RED << "Task '" << name << "; failed: " << e.what() << RESET << std::endl;
	} catch (...) {
		threadSafeCout << RED << "Task '" << name << "; failed with unknown error" << RESET << std::endl;
	}
}

PersistentWorker::PersistentWorker(): _workerThread("PersistentWorker", [this]() { this->workerLoop(); }), _running(false) {}

PersistentWorker::~PersistentWorker() { 
	stop(); 
}

void PersistentWorker::start() {
	if (!_running.exchange(true)) {
		_workerThread.start();
	}
}

void PersistentWorker::stop() {
	_running = false;
	_workerThread.stop();
}

void PersistentWorker::addTask(const std::string &name, const std::function<void()> &jobToExecute) {
	std::lock_guard<std::mutex> lock(_tasksMutex);
	_tasks[name] = jobToExecute;
	threadSafeCout << YEL << "Task '" << name << "' added to persistent worker" << RESET << std::endl;
}

void PersistentWorker::removeTask(const std::string &name) {
	std::lock_guard<std::mutex> lock(_tasksMutex);
	if (_tasks.erase(name) > 0) {
		threadSafeCout << MAG << "Task '" << name << "' removed from persistent worker" << RESET << std::endl;
	}
}

std::vector<std::string> PersistentWorker::getTaskNames() const{
	std::lock_guard<std::mutex> lock(_tasksMutex);
	std::vector<std::string> names;

	for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
		names.push_back(it->first);
	}

	return names;
}

size_t PersistentWorker::getTaskCount() const { 
	std::lock_guard<std::mutex> lock(_tasksMutex);
	return _tasks.size(); 
}

bool PersistentWorker::isRunning() const { 
	return _running; 
}

bool PersistentWorker::hasTask(const std::string &name) const { 
	std::lock_guard<std::mutex> lock(_tasksMutex);
	return _tasks.count(name) > 0; 
}
