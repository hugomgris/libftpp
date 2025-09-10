/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:31:58 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/10 15:11:44 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_HPP
# define POOL_HPP

#include <vector>
#include <memory>
#include <stdexcept>

// Forward declaration
class IntPool;

// The pointer wrapper that users interface with
class IntPoolObject {
	private:
		int *ptr;
		IntPool *pool;
		size_t index;

	public:
		IntPoolObject(int *p, IntPool *owner, size_t idx);
		~IntPoolObject();
		
		// Move constructor/assignment
		IntPoolObject(IntPoolObject &&other) noexcept;
		IntPoolObject &operator=(IntPoolObject &&other) noexcept;

		// Deletion of copy constructor/assignment
		IntPoolObject(const IntPoolObject&) = delete;
		IntPoolObject &operator=(const IntPoolObject &) = delete;

		int *operator->() { return ptr; }
		int &operator*() { return *ptr; }
};

// The pool class that manages objects
class IntPool {
	private:
		std::unique_ptr<int[]> storage;
		std::vector<size_t> free_indices;
		size_t capacity;

		friend class IntPoolObject;

	public:
		IntPool(): capacity(0) {}
		~IntPool() = default;

		inline void returnObject(size_t index) {
			free_indices.push_back(index);
		}

		inline void resize(size_t numberOfObjects) {
			storage = std::make_unique<int[]>(numberOfObjects);
			capacity = numberOfObjects;

			free_indices.clear();
			free_indices.reserve(numberOfObjects);
			for (size_t i = 0; i < numberOfObjects; ++i) {
				free_indices.push_back(i);
			}
		}

		IntPoolObject acquire(int value) {
			if (free_indices.empty()) {
				throw std::runtime_error("Pool is empty");
			}

			size_t index = free_indices.back();
			free_indices.pop_back();

			new(&storage[index]) int(value);

			return IntPoolObject(&storage[index], this, index);
		}
};

// Implementation after definition is needed for class interaction
inline IntPoolObject::IntPoolObject(int *p, IntPool *owner, size_t idx) 
	: ptr(p), pool(owner), index(idx) {}

inline IntPoolObject::~IntPoolObject() {
	if (pool && ptr) {
		// For int, destructor is trivial, so we can skip explicit call
		pool->returnObject(index);
	}
}

inline IntPoolObject::IntPoolObject(IntPoolObject &&other) noexcept 
	: ptr(other.ptr), pool(other.pool), index(other.index) {
	other.ptr = nullptr;
	other.pool = nullptr;
}

inline IntPoolObject &IntPoolObject::operator=(IntPoolObject &&other) noexcept {
	if (this != &other) {
		if (pool && ptr) {
			// No explicit destructor call needed for int
			pool->returnObject(index);
		}
		
		ptr = other.ptr;
		pool = other.pool;
		index = other.index;
		
		other.ptr = nullptr;
		other.pool = nullptr;
	}
	return *this;
}

#endif