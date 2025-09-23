/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   persistent_worker.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 16:24:25 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 13:57:44 by hmunoz-g         ###   ########.fr       */
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

class PersistentWorker {
	private:
		std::map<std::string, std::function<void()>> _tasks;
		Thread _workerThread;
		std::atomic<bool> _running;
		mutable std::mutex _tasksMutex;

		void workerLoop();
		void executeTaskSafely(const std::string &name, const std::function<void()> &task);

	public:
		PersistentWorker();
		~PersistentWorker();

		void start();
		void stop();
		void addTask(const std::string &name, const std::function<void()> &jobToExecute);
		void removeTask(const std::string &name);
		std::vector<std::string> getTaskNames() const;
		size_t getTaskCount() const;
		bool isRunning() const;
		bool hasTask(const std::string &name) const;
};

#endif