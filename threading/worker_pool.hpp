/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:16:59 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 13:57:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKER_POOL_HPP
# define WORKER_POOL_HPP

# include <vector>
# include <functional>
# include <atomic>
# include <memory>

# include "thread.hpp"
# include "thread_safe_queue.hpp"

class IJobs {
	public:
		virtual void execute() = 0;
		virtual ~IJobs() = default;
};

class WorkerPool {
	private:
		std::vector<Thread> _workers;
		ThreadSafeQueue<std::function<void()>> _jobQueue;
		std::atomic<bool> _shutdown;

		void _workerFunction(int workerId);

	public:
		explicit WorkerPool(size_t numWorkers = std::thread::hardware_concurrency());
		~WorkerPool();

		void addJob(const std::function<void()> & jobToExecute);
		void addJob(std::unique_ptr<IJobs> job);
		void shutdownPool();
		size_t getQueueSize() const;
		bool isShutdown() const;
};

#endif