/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:24:25 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/22 17:19:39 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERSISTENT_WORKER_HPP
# define PERSISTENT_WORKER_HPP

# include <map>
# include <string>
# include <functional>
# include <mutex>
# include <atomic>
# include <vector>

# include "thread.hpp"
# include "../IOStream/thread_safe_iostream.hpp"

extern ThreadSafeIOStream threadSafeCout;

class PersistentWorker {
	private:
		std::map<std::string, std::function<void()>> _tasks;
		Thread _workerThread;
		std::atomic<bool> _running;
		mutable std::mutex _tasksMutex;

		void workerLoop() {
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

		void executeTaskSafely(const std::string &name, const std::function<void()> &task) {
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

	public:
		PersistentWorker(): _workerThread("PersistentWorker", [this]() { this->workerLoop(); }), _running(false) {}
		~PersistentWorker() { stop(); }

		void start() {
			if (!_running.exchange(true)) {
				_workerThread.start();
			}
		}

		void stop() {
			_running = false;
			_workerThread.stop();
		}

		void addTask(const std::string &name, const std::function<void()> &jobToExecute) {
			std::lock_guard<std::mutex> lock(_tasksMutex);
			_tasks[name] = jobToExecute;
			threadSafeCout << YEL << "Task '" << name << "' added to persistent worker" << RESET << std::endl;
		}

		void removeTask(const std::string &name) {
			std::lock_guard<std::mutex> lock(_tasksMutex);
			if (_tasks.erase(name) > 0) {
				threadSafeCout << MAG << "Task '" << name << "' removed from persistent worker" << RESET << std::endl;
			}
		}

		std::vector<std::string> getTaskNames() const{
			std::vector<std::string> names;

			auto it = _tasks.begin();
			while (it != _tasks.end()) {
				names.push_back(it->first);
			}

			return (names);
		}

		size_t getTaskCount() const { return _tasks.size(); }
		bool isRunning() const { return (_running); }
		bool hasTask(const std::string &name) const { return (_tasks.count(name)) ? true : false; }
};

#endif