/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:54:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/17 11:09:49 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"
#include "colors.h"
#include <string>
#include <vector>

// Test class for Memento
class GameCharacter: public Memento {
	private:
		std::string name;			
		int health;
		int level;

		// Concrete implementation of virtual methods
		void _saveToSnapshot(Snapshot &snapshot) const override {
			snapshot << health << level;
		}

		void _loadFromSnapshot(Snapshot &snapshot) override {
			snapshot >> health >> level;
		}

	public:
		GameCharacter(const std::string &n, int h, int l): name(n), health(h), level(l) {}
		
		int getHealth() const { return health; }
		int getLevel() const { return level; }
		const std::string &getName() const { return name; }

		void takeDamage(int damage) { health -= damage; }
		void levelUp() { level++; }

};

// Test classes for Singleton
class GameConfig {
	friend class Singleton<GameConfig>;
	private:
		std::string gameName;
		int maxPlayers;
		bool debugMode;

		// Private constructor because only Singleton should be able to create it.
		GameConfig(const std::string &name, int players, bool debug)
			: gameName(name), maxPlayers(players), debugMode(debug) {
				std::cout << "GameConfig created: " << name << ", " << maxPlayers << ", " << debugMode << std::endl;
			}

	public:
		// Getters just for testing
		const std::string &getName() const { return gameName; }
		int getMaxPlayers() const { return maxPlayers; }
		bool isDebugMode() const {return debugMode; }
};

class Logger {
	friend class Singleton<Logger>;
	private:
		std::string logFile;
		Logger(const std::string &file): logFile(file) {
			std::cout << "Logger created for file: " << file << std::endl;
		}
	public:
		const std::string &getLogFile() const { return logFile; }
};

class NotInstantiated {
	friend class Singleton<NotInstantiated>;
	private:
		NotInstantiated() = default;
};

// TESTS

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
	std::cout << MAG << "\n=== Testing MEMENTO pattern with GameCharacter inheriting class ===" << RESET << std::endl;

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

void testObserver() {
	/*
	Lambda function cheatsheet:
		[&]        // Capture all by reference
		[=]        // Capture all by copy
		[&gameOver] // Capture only gameOver by reference
		[gameOver]  // Capture only gameOver by copy
		[]         // Capture nothing
	*/
	
	std::cout << MAG << "\n=== Testing Observer Pattern ===" << RESET << std::endl;	

	enum GameEvent { PlayerDied, LevelUp, ItemFound };
	Observer<GameEvent> basicObserver;
	Observer<GameEvent, std::string> itemObserver;

	int score = 100;
	int level = 1;
	bool gameOver = false;
	std::vector<std::string> inventory;

	// Multiple callback subscription
	basicObserver.subscribe(PlayerDied, [&gameOver]() {
		std::cout << "GAME OVER!!" << std::endl;
		gameOver = true;
	});

	basicObserver.subscribe(LevelUp, [&level, &score]() {
		level++;
		score += 50;
		std::cout << "Level up! New level: " << level << ", Score: " << score << std::endl;
	});

	itemObserver.subscribe(ItemFound, [&inventory](std::string item) {
		inventory.push_back(item);
		std::cout << "Found item: " << item << " (Inventory size: " << inventory.size() << ")" << std::endl;
	});

	basicObserver.subscribe(PlayerDied, [&score](){
		std::cout << "Final Score: " << score << std::endl;
	});

	// Event triggering
	std::cout << "Player finds items..." << std::endl;
	itemObserver.notify(ItemFound, "Health Potion");
	itemObserver.notify(ItemFound, "Magic Sword");
	
	std::cout << "Player levels up..." << std::endl;
	basicObserver.notify(LevelUp);
	
	std::cout << "Player dies..." << std::endl;
	basicObserver.notify(PlayerDied);
	
	std::cout << "Game Over Status: " << (gameOver ? "true" : "false") << std::endl;
	std::cout << "Dropped loot size: " << inventory.size() << std::endl;
}

void testSingleton() {
	std::cout << MAG << "\n=== Testing Singleton Pattern ===" << RESET << std::endl;

	std::cout << YEL << "\n=== Basic Instantiation Test ===" << RESET << std::endl;
	try {
		Singleton<GameConfig>::instantiate("Silksong", 1, true);
		std::cout << GRN << "First instantiation successful" << RESET << std::endl;

		GameConfig *config = Singleton<GameConfig>::instance();
		std::cout << "Game: " << config->getName() << ", Players: " << config->getMaxPlayers() << ", Debug: " << (config->isDebugMode() ? "True" : "False") << std::endl;
		std::cout << GRN << "Instance retrieve successful" << RESET << std::endl;
	} catch (const std::exception &e) {
		std::cout << RED << "Caught exception: " << e.what() << RESET << std::endl;
	}

	std::cout << YEL << "\n=== Multiple instance() calls test ===" << RESET << std::endl;
	try {
		GameConfig *config1 = Singleton<GameConfig>::instance();
		GameConfig *config2 = Singleton<GameConfig>::instance();

		if (config1 == config2) {
			std::cout << GRN << "Multiple instance() calls return same object" << RESET << std::endl;
			std::cout << "Address 1: " << config1 << ", Address 2: " << config2 << std::endl;
		} else {
			std::cout << RED << "Different objects returned!" << RESET << std::endl;
		}
	} catch (const std::exception &e) {
		std::cout << RED << "Caught exception: " << e.what() << RESET << std::endl;
	}

	std::cout << YEL << "\n=== Double instantiation test ===" << RESET << std::endl;
	try {
		Singleton<GameConfig>::instantiate("Silksong 2", 2, false);
		std::cout << RED << "Double instantiation s hould have thrown, you should never see this" << RESET << std::endl;
	} catch (const SingletonException &e) {
		std::cout << GRN << "Correctly threw SingletonException: " << e.what() << RESET << std::endl;
	} catch (const std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}

	std::cout << YEL << "\n=== Different types test ===" << RESET << std::endl;
	try {
		Singleton<Logger>::instantiate("game.log");
		Logger *logger = Singleton<Logger>::instance();
		std::cout << GRN "Different singleton type works: " << logger->getLogFile() << RESET << std::endl;

		//Independence verification
		GameConfig *config = Singleton<GameConfig>::instance();
		std::cout << "GameConfig still accessible: " << config->getName() << std::endl;
	} catch (const std::exception &e) {
		std::cout << RED << "Caught exception: " << e.what() << RESET << std::endl;
	}

	std::cout << YEL << "\n=== Double instantiation test ===" << RESET << std::endl;
	try {
		Singleton<GameConfig>::instantiate("Silksong 2", 2, false);
		std::cout << RED << "Double instantiation s hould have thrown, you should never see this" << RESET << std::endl;
	} catch (const SingletonException &e) {
		std::cout << GRN << "Correctly threw SingletonException: " << e.what() << RESET << std::endl;
	} catch (const std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}

	std::cout << YEL << "\n=== Access before instantiation test (should throw) ===" << RESET << std::endl;
	try {
		Singleton<NotInstantiated>::instance();
		std::cout << RED << "Should have thrown for non-instantiated singleton, you shold never see this" << RESET << std::endl;
	} catch (const std::runtime_error &e) {
		std::cout << GRN << "Correctly threw runtime_error: " << e.what() << RESET << std::endl;
	} catch (const std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}
}

void testStateMachine() {
	std::cout << MAG << "\n=== Testing StateMachine Pattern ===" << RESET << std::endl;

	enum playerStates { Walking, Running, Jumping, Fighting, Resting};

	StateMachine<playerStates> machine;
	machine.addState(Walking);
	machine.addState(Running);
	machine.addState(Jumping);
	machine.addState(Fighting);
	//machine.addState(Walking);

	std::cout << YEL << "\n=== Basic transition setup and execution test ===" << RESET << std::endl;
	machine.addTransition(Walking, Running, [](){
		std::cout << "Player started running!" << std::endl;
	});
	machine.addAction(Running, [](){
		std::cout << "Running..." << std::endl;
	});
	machine.transitionTo(Running);
	machine.update();

	std::cout << YEL << "\n=== Error conditions test (should show a handful of throws) ===" << RESET << std::endl;
	try {
		machine.addTransition(Walking, Resting, [](){
			std::cout << "Player stopped to rest!" << std::endl;
		});
	} catch (StateMachineException &e) {
		std::cout << GRN << "Correctly threw StateMachineException: " << e.what() << RESET << std::endl;
	} catch (std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}

	try {
		machine.addTransition(Resting, Walking, [](){
			std::cout << "Player went back to walking!" << std::endl;
		});
	} catch (StateMachineException &e) {
		std::cout << GRN << "Correctly threw StateMachineException: " << e.what() << RESET << std::endl;
	} catch (std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}

	try {
		machine.transitionTo(Resting);
	} catch (StateMachineException &e) {
		std::cout << GRN << "Correctly threw StateMachineException: " << e.what() << RESET << std::endl;
	} catch (std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}

	try {
		machine.transitionTo(Jumping);
	} catch (StateMachineException &e) {
		std::cout << GRN << "Correctly threw StateMachineException: " << e.what() << RESET << std::endl;
	} catch (std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}

	StateMachine<playerStates> machine2;
	try {
		machine2.transitionTo(Resting);
	} catch (StateMachineException &e) {
		std::cout << GRN << "Correctly threw StateMachineException: " << e.what() << RESET << std::endl;
	} catch (std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}

	machine.addTransition(Running, Walking, [](){
		std::cout << "Player slowed down!" << std::endl;
	});
	try {
		machine.transitionTo(Walking);
		machine.update();
	} catch (StateMachineException &e) {
		std::cout << GRN << "Correctly threw StateMachineException: " << e.what() << RESET << std::endl;
	} catch (std::exception &e) {
		std::cout << RED << "Wrong exception type: " << e.what() << RESET << std::endl;
	}
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
	testObserver();
	testSingleton();
	testStateMachine();
	
	std::cout << GRN << "\nAll tests completed!" << RESET << std::endl;
	return 0;
}