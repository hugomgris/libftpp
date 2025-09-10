/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:31:58 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/10 16:29:17 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_HPP
# define POOL_HPP

# include <vector>
# include <memory>
# include <stdexcept>
# include <utility>

// The pool class that manages objects
template <typename TType>
class Pool {
	public:
		// Nested Object class - this is what users get
		class Object {
			private:
				TType *ptr;
				Pool<TType> *pool;
				size_t index;

			public:
				Object(TType *p, Pool<TType> *owner, size_t idx);
				~Object();
				
				// Move constructor/assignment
				Object(Object &&other) noexcept;
				Object &operator=(Object &&other) noexcept;

				// Deletion of copy constructor/assignment
				Object(const Object&) = delete;
				Object &operator=(const Object &) = delete;

				TType *operator->() { return ptr; }
				TType &operator*() { return *ptr; }
		};

	private:
		std::unique_ptr<std::byte[]> storage;  // Raw memory, not constructed objects
		std::vector<size_t> free_indices;
		size_t capacity;

	public:
		Pool(): capacity(0) {}
		~Pool() = default;

		inline void returnObject(size_t index) {
			free_indices.push_back(index);
		}

		inline void resize(const size_t& numberOfObjects) {
			// Allocate raw memory, don't construct objects yet
			storage = std::make_unique<std::byte[]>(numberOfObjects * sizeof(TType));
			capacity = numberOfObjects;

			free_indices.clear();
			free_indices.reserve(numberOfObjects);
			for (size_t i = 0; i < numberOfObjects; ++i) {
				free_indices.push_back(i);
			}
		}

		template<typename... TArgs>
		Object acquire(TArgs&&... args) {
			if (free_indices.empty()) {
				throw std::runtime_error("Pool is empty");
			}

			size_t index = free_indices.back();
			free_indices.pop_back();

			// Get pointer to the raw memory location
			TType* ptr = reinterpret_cast<TType*>(storage.get() + index * sizeof(TType));
			new(ptr) TType(std::forward<TArgs>(args)...);

			return Object(ptr, this, index);
		}
};

// Implementation after definition is needed for class interaction
template <typename TType>
inline Pool<TType>::Object::Object(TType *p, Pool<TType> *owner, size_t idx) 
	: ptr(p), pool(owner), index(idx) {}

template <typename TType>
inline Pool<TType>::Object::~Object() {
	if (pool && ptr) {
		ptr->~TType();
		pool->returnObject(index);
	}
}

template <typename TType>
inline Pool<TType>::Object::Object(Object &&other) noexcept 
	: ptr(other.ptr), pool(other.pool), index(other.index) {
	other.ptr = nullptr;
	other.pool = nullptr;
}

template <typename TType>
inline typename Pool<TType>::Object &Pool<TType>::Object::operator=(Object &&other) noexcept {
	if (this != &other) {
		if (pool && ptr) {
			ptr->~TType();
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