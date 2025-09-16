/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:54:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/16 15:23:39 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include <string>
#include <vector>

struct SaveData {
	int health;
	int level;
};

class GameCharacter: public Memento {
	private:
		std::string name;	
		
		int health;
		int level;

		// Concrete implementation of virtual methods
		void _saveToSnapshot(Snapshot &snapshot) const override {
			SaveData data{health, level};
			snapshot << data;
		}

		void _loadFromSnapshot(Snapshot &snapshot) override {
			SaveData data;
			snapshot >> data;
			health = data.health;
			level = data.level;
		}

	public:
		GameCharacter(const std::string &n, int h, int l): name(n), health(h), level(l) {}
		
		int getHealth() const { return health; }
		int getLevel() const { return level; }
		const std::string &getName() const { return name; }

		void takeDamage(int damage) { health -= damage; }
		void levelUp() { level++; }

};

void testBasicPoolOperations() {
	std::cout << YEL "\n=== Testing Basic Pool Operations ===" << RESET << RESET << std::endl;
	
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
		std::cout << GRN << "✓ Correctly caught exception: " << e.what() << RESET << std::endl;
	}
}

void testObjectLifetime() {
	std::cout << YEL "\n=== Testing Object Lifetime & RAII (pool size = 2) ===" << RESET << RESET << std::endl;
	
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
	std::cout << GRN << "✓ After scope - reacquired obj3: " << *obj3 << ", obj4: " << *obj4 << RESET << std::endl;
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
			std::cout << BLU << "Constructing TestClass(" << v << ", '" << n << "')" << RESET << std::endl;
		}
		
		~TestClass() {
			std::cout << BLU << "Destroying TestClass(" << value << ", '" << name << "')" << RESET << std::endl;
		}
	};
	
	Pool<TestClass> complexPool;
	complexPool.resize(2);
	
	{
		auto obj1 = complexPool.acquire(42, "Hello");
		auto obj2 = complexPool.acquire(100, "World");
		
		std::cout << "obj1: value=" << obj1->value << ", name='" << obj1->name << "'" << std::endl;
		std::cout << "obj2: value=" << obj2->value << ", name='" << obj2->name << "'" << std::endl;
		
		std::cout << "Leaving scope - destructors should be called..." << std::endl;
	}
	std::cout << GRN << "Objects properly destroyed and returned to pool" << RESET << std::endl;
}

void testBasicDataBuffer() {
	std::cout << YEL << "\n=== Testing Basic DataBuffer management ===" << RESET << std::endl;

	DataBuffer buffer;
	int original = 42;
	buffer << original;

	int restored;
	buffer >> restored;

	std::cout << "original: " << original << " - " << restored << " :restored" << std::endl;
	if (original == restored) {
		std::cout << GRN << "Data coincides!" << RESET << std::endl;
	} else {
		std::cout << RED << "Data mismatch" << RESET << std::endl;
	}

	std::cout << YEL << "\n=== Testing type mismatch ===" << RESET << std::endl;
	try {
		double wrong;
		buffer >> wrong;
	} catch (std::exception &e) {
		std::cout << RED << "Caught exception: " << e.what() << RESET << std::endl;
	}
}

void testAdvancedDataBuffer() {
	std::cout << YEL << "\n=== Testing Advanced DataBuffer management ===" << RESET << std::endl;

	DataBuffer buffer;
	struct Point { int x, y; };
	Point p1{10, 20};
	buffer << p1;
	Point p2;
	buffer >> p2;

	std::cout << YEL << "Testing with POD struct Point { int x, y; }" << RESET << std::endl;
	std::cout << "Original Point - x:" << p1.x <<" ,y:" << p1.y << std::endl;
	std::cout << "Restored point - x:" << p2.x <<" ,y:" << p2.y << std::endl;
	if (p1.x == p2.x && p1.y == p2.y) {
		std::cout << GRN << "Data coincides!" << RESET << std::endl;
	} else {
		std::cout << RED << "Data mismatch" << RESET << std::endl;
	}

	std::cout << YEL << "\n=== Overwriting test ===" << RESET << std::endl;
	buffer << 3.14159;
	double restored;
	buffer >> restored;
	std::cout << "Buffer now has now a stored " << typeid(restored).name() << " (typename of double): " << restored << std::endl;
	std::cout << GRN << "Overwriting successful" << RESET << std::endl;

	std::cout << YEL << "\n=== Edge cases test ===" << RESET << std::endl;
	buffer << 0;
	int restored2;
	buffer >> restored2;
	std::cout << "Buffer overwritten with int = 0 has value: " << restored2 << std::endl;
	buffer << -1;
	buffer >> restored2;
	std::cout << "Buffer changed to -1 has value: " << restored2 << std::endl;
	std::cout << GRN << "Incredible success in this self-made and self-ran tests" << RESET << std::endl;
}

void testMultiDataBuffer() {
	std::cout << YEL << "\n=== Testing DataBuffer management with multiple stored items ===" << RESET << std::endl;
	DataBuffer buffer;
	int eyes = 2;
	int limbs = 4;
	int hairs = 24509;
	buffer << eyes << limbs << hairs;
	int restoredEyes, restoredLimbs, restoredHairs;
	buffer >> restoredEyes >> restoredLimbs >> restoredHairs;
	if (eyes == restoredEyes && limbs == restoredLimbs && hairs == restoredHairs) {
		std::cout << "Tested with " << restoredEyes << " eyes, " << restoredLimbs << " limbs, " << restoredHairs << " hairs." << std::endl;
		std::cout << GRN << "Multi item test passed" << std::endl;
	} else {
		std::cout << RED << "Multi item test failed" << std::endl;
	}

	std::cout << YEL << "\n=== Testing out of range read position ===" << RESET << std::endl;
	try {
		int restoredFingers;
		buffer >> restoredEyes >> restoredLimbs >> restoredHairs >> restoredFingers;
	} catch (const std::exception &e) {
		std::cout << RED << "Caught exception: " << e.what() << RESET << std::endl;
	}
}

void testMemento() {
	std::cout << YEL << "\n=== Testing MEMENTO pattern with GameCharacter inheriting class ===" << RESET << std::endl;

	GameCharacter player("Hugo" ,100, 5);

	// Initial save state
	auto checkpoint = player.save();
	std::cout << "Saved initial status for player " << player.getName() << ": Health=" << player.getHealth() << ", Level=" << player.getLevel() << std::endl;

	player.takeDamage(50);
	player.levelUp();
	std::cout << "After changes: Health=" << player.getHealth() << ", Level=" << player.getLevel() << std::endl;

	// State restoring
	player.load(checkpoint);
	std::cout << "After state restoring: Health=" << player.getHealth() << ", Level=" << player.getLevel() << std::endl;
}

int main(void) {
	// Pool tests
	std::cout << CYN << "====== POOL data structure tests ======" << RESET << std::endl;
	testBasicPoolOperations();
	testObjectLifetime();
	testMoveSemantics();
	testVariadicTemplates();
	testComplexObjects();

	std::cout << CYN << "\n====== DATABUFFER data structure tests ======" << RESET << std::endl;
	testBasicDataBuffer();
	testAdvancedDataBuffer();
	testMultiDataBuffer();

	std::cout << CYN << "\n====== DESIGN PATTERNS tests ======" << RESET << std::endl;
	testMemento();
	
	std::cout << GRN << "\nAll tests completed!" << RESET << std::endl;
	return 0;
}