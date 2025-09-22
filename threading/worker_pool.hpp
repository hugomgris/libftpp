/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker_pool.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:16:59 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/22 16:00:45 by hmunoz-g         ###   ########.fr       */
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

		void _workerFunction(int workerId) {
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

	public:
		explicit WorkerPool(size_t numWorkers = std::thread::hardware_concurrency()): _shutdown(false) {
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

		~WorkerPool() { shutdownPool(); }

		void addJob(const std::function<void()> & jobToExecute) {
			if (!_shutdown) {
				_jobQueue.push_back(jobToExecute);
			}
		}

		void addJob(std::unique_ptr<IJobs> job) {
			if (!_shutdown) {
				std::shared_ptr<IJobs> sharedJob = std::move(job);
				
				_jobQueue.push_back([sharedJob](){
					sharedJob->execute();
				});
			}
		}

		void shutdownPool() {
			_shutdown = true;

			for (auto &worker : _workers) {
				worker.stop();
			}
		}

		size_t getQueueSize() const { return _jobQueue.size(); }

		bool isShutdown() const { return _shutdown; }
};

#endif