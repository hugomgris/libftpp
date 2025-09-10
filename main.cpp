/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:54:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/10 16:52:58 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <string>
#include <vector>

void testBasicPoolOperations() {
	std::cout << YEL << YEL << "\n=== Testing Basic Pool Operations ===" << RESET << RESET << std::endl;
	
	Pool<int> pool;
	pool.resize(3);
	
	// Test basic acquisition and access
	auto obj1 = pool.acquire(42);
	auto obj2 = pool.acquire(100);
	
	std::cout << "obj1: " << *obj1 << ", obj2: " << *obj2 << std::endl;
	std::cout << "obj1 via ->: " << *obj1.operator->() << std::endl;
	
	// Test pool full scenario
	auto obj3 = pool.acquire(200);
	std::cout << "obj3: " << *obj3 << std::endl;
	
	try {
		auto obj4 = pool.acquire(1);  // Should throw
		std::cout << RED << "ERROR: Pool should be full!" << RESET << std::endl;
	} catch (const std::runtime_error &e) {
		std::cout << GRN << GRN << "✓ Correctly caught exception: " << e.what() << RESET << std::endl;
	}
}

void testObjectLifetime() {
	std::cout << YEL << YEL << "\n=== Testing Object Lifetime & RAII ===" << RESET << RESET << std::endl;
	
	Pool<int> pool;
	pool.resize(2);
	
	{
		auto obj1 = pool.acquire(111);
		auto obj2 = pool.acquire(222);
		std::cout << "Inside scope - obj1: " << *obj1 << ", obj2: " << *obj2 << std::endl;
		// Objects should be automatically returned when leaving scope
	}
	
	// Should be able to acquire again after objects were returned
	auto obj3 = pool.acquire(333);
	auto obj4 = pool.acquire(444);
	std::cout << GRN << GRN << "✓ After scope - reacquired obj3: " << *obj3 << ", obj4: " << *obj4 << RESET << std::endl;
}

void testMoveSemantics() {
	std::cout << YEL << "\n=== Testing Move Semantics ===" << RESET << std::endl;
	
	Pool<int> pool;
	pool.resize(3);
	
	auto obj1 = pool.acquire(777);
	std::cout << "Original obj1: " << *obj1 << std::endl;
	
	// Test move constructor
	auto obj2 = std::move(obj1);
	std::cout << "After move constructor - obj2: " << *obj2 << std::endl;
	
	// Test move assignment
	auto obj3 = pool.acquire(888);
	std::cout << "obj3 before move assignment: " << *obj3 << std::endl;
	obj3 = std::move(obj2);
	std::cout << GRN << "✓ obj3 after move assignment: " << *obj3 << RESET << std::endl;
}

void testVariadicTemplates() {
	std::cout << YEL << "\n=== Testing Variadic Templates ===" << RESET << std::endl;
	
	// Test with string (multiple constructor overloads)
	Pool<std::string> stringPool;
	stringPool.resize(4);
	
	auto str1 = stringPool.acquire("Hello");                    // const char*
	auto str2 = stringPool.acquire(10, 'x');                   // size_t, char -> "xxxxxxxxxx"
	auto str3 = stringPool.acquire();                          // default constructor -> ""
	
	std::cout << "str1: '" << *str1 << "'" << std::endl;
	std::cout << "str2: '" << *str2 << "'" << std::endl;
	std::cout << "str3: '" << *str3 << "'" << std::endl;
	
	// Test with vector
	Pool<std::vector<int>> vecPool;
	vecPool.resize(2);
	
	auto vec1 = vecPool.acquire(5, 42);                        // size_t, value -> 5 elements of 42
	// For initializer_list, we need to be explicit about the type
	std::initializer_list<int> init_list = {1, 2, 3, 4, 5};
	auto vec2 = vecPool.acquire(init_list);                    // initializer_list
	
	std::cout << "vec1 size: " << vec1->size() << ", first element: " << (*vec1)[0] << std::endl;
	std::cout << "vec2 size: " << vec2->size() << ", elements: ";
	for (int val : *vec2) {
		std::cout << val << " ";
	}
	std::cout << std::endl;
}

void testComplexObjects() {
	std::cout << YEL << "\n=== Testing Complex Objects ===" << RESET << std::endl;
	
	struct TestClass {
		int value;
		std::string name;
		
		TestClass(int v, const std::string& n) : value(v), name(n) {
			std::cout << "  Constructing TestClass(" << v << ", '" << n << "')" << std::endl;
		}
		
		~TestClass() {
			std::cout << "  Destroying TestClass(" << value << ", '" << name << "')" << std::endl;
		}
	};
	
	Pool<TestClass> complexPool;
	complexPool.resize(2);
	
	{
		auto obj1 = complexPool.acquire(42, "Hello");
		auto obj2 = complexPool.acquire(100, "World");
		
		std::cout << "obj1: value=" << obj1->value << ", name='" << obj1->name << "'" << std::endl;
		std::cout << "obj2: value=" << obj2->value << ", name='" << obj2->name << "'" << std::endl;
		
		std::cout << "  Leaving scope - destructors should be called..." << std::endl;
	}
	std::cout << GRN << "✓ Objects properly destroyed and returned to pool" << RESET << std::endl;
}

int main(void) {
	testBasicPoolOperations();
	testObjectLifetime();
	testMoveSemantics();
	testVariadicTemplates();
	testComplexObjects();
	
	std::cout << GRN << "\nAll tests completed!" << RESET << std::endl;
	return 0;
}