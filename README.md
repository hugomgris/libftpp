# <h1 align="center">LIBFTPP</h1>

<p align="center">
   <b>A comprehensive C++ library implementation featuring modular design patterns, advanced data structures, thread-safe operations, networking capabilities, mathematical computations, and bonus utilities for modern C++ development and systems programming.</b><br>
</p>

---

<p align="center">
    <img alt="C++" src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
    <img alt="Systems Programming" src="https://img.shields.io/badge/Systems%20Programming-FF6B6B?style=for-the-badge" />
    <img alt="Network Programming" src="https://img.shields.io/badge/Network%20Programming-4ECDC4?style=for-the-badge" />
</p>
<p align="center">
    <img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/hugomgris/libftpp?color=lightblue" />
    <img alt="Standard" src="https://img.shields.io/badge/C++-17-blue" />
    <img alt="42 School" src="https://img.shields.io/badge/42-School-000000" />
</p>

## Table of Contents

- [General Description](#general-description)
- [Architectural Foundation](#architectural-foundation)
- [Build & Execution Instructions](#build--execution-instructions)
  - [Prerequisites](#prerequisites)
  - [Example Usage](#example-usage)
- [Project Structure](#project-structure)
- [Module Breakdown](#module-breakdown)
  - [Core Modules](#core-modules)
  - [Bonus Features](#bonus-features)
- [Class-by-Class Implementation Guide](#class-by-class-implementation-guide)
- [Implementation Details](#implementation-details)
- [Performance Considerations](#performance-considerations)
- [Applications](#applications)
- [Design Patterns](#design-patterns)
- [Advanced Features](#advanced-features)
- [Testing Framework](#testing-framework)
- [API Documentation](#api-documentation)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

---

## General Description

libftpp is a comprehensive C++ library implementation designed for the 42 School curriculum, showcasing modern C++17 programming practices and software engineering principles. Built with modularity, performance, and extensibility in mind, it provides a robust foundation for systems programming, network applications, mathematical computing, and concurrent operations.

The implementation spans multiple interconnected modules, each demonstrating different aspects of advanced C++ programming, from low-level memory management and threading to high-level design patterns and application frameworks. The library serves as both a practical toolkit and an educational resource for understanding modern C++ development.

## Architectural Foundation

libftpp is built upon several key architectural principles that form the backbone of modern C++ system design:

- **Modular Design**: Loosely coupled components with well-defined interfaces
- **Template Programming**: Generic programming for type safety and performance
- **RAII Principles**: Resource Acquisition Is Initialization for memory safety
- **Concurrent Programming**: Thread-safe operations and synchronization primitives
- **Network Programming**: TCP-based client-server communication patterns
- **Design Patterns**: Implementation of fundamental software design patterns

```
Core Design Principles:
┌─────────────────────────────────────────────────────────────┐
│  RAII + Smart Pointers → Memory Safety                     │
│  Template Metaprogramming → Type Safety + Performance      │
│  Mutex + Lock Guards → Thread Safety                       │
│  Exception Safety → Error Handling                         │
│  Move Semantics → Performance Optimization                 │
│  Design Patterns → Code Reusability                        │
└─────────────────────────────────────────────────────────────┘
```

## Build & Execution Instructions

### Prerequisites

- **Compiler**: g++ or clang++ with C++17 support
- **Build System**: GNU Make utility
- **Platform**: Linux/macOS/Windows with WSL
- **Memory Tools**: AddressSanitizer support for debugging

### Compilation

The library uses a comprehensive Makefile with multiple build targets:

```bash
# Build the complete library
make all

# Build specific module tests
make test_unit_1    # Basic functionality tests
make test_unit_2    # Thread-safe iostream tests
make test_unit_3    # Threading system tests
make test_unit_4    # Network module tests
make test_unit_5    # Mathematics module tests
make test_bonus     # Bonus features tests

# Clean build artifacts
make clean          # Remove object files
make fclean         # Remove all generated files
make re             # Rebuild from scratch
```

### Example Usage

```cpp
#include "libftpp.hpp"
#include "bonus/bonus.hpp"  // For bonus features

int main() {
    // Data Structures - Object Pool
    Pool<std::string> stringPool;
    stringPool.resize(10);
    auto str = stringPool.acquire("Hello, World!");
    
    // Threading - Worker Pool
    WorkerPool workers(4);
    workers.enqueue([](){ return compute_heavy_task(); });
    
    // Network - TCP Client
    Client client;
    client.connect("127.0.0.1", 8080);
    client.send_message("Hello Server");
    
    // Mathematics - Vector Operations
    IVector3<float> v1(1.0f, 2.0f, 3.0f);
    IVector3<float> v2(4.0f, 5.0f, 6.0f);
    auto result = v1.cross(v2);
    
    // Bonus - Application Framework
    SampleApplication app;
    app.setTargetFPS(60);
    app.run();
    
    return 0;
}
```

## Project Structure

```
libftpp/
├── libftpp.hpp                     # Main library header
├── Makefile                        # Build system configuration
├── colors.h                        # Terminal color definitions
├── data_structures/                # Advanced container implementations
│   ├── data_structures.hpp         # Main data structures header
│   ├── data_buffer.hpp             # Type-safe data serialization
│   └── pool.hpp                    # Object pool for memory management
├── design_patterns/                # Software design pattern implementations
│   ├── design_patterns.hpp         # Main patterns header
│   ├── memento.hpp                 # State preservation pattern
│   ├── observer.hpp                # Event notification pattern
│   ├── singleton.hpp               # Single instance pattern
│   └── state_machine.hpp           # Finite state automaton
├── IOStream/                       # Thread-safe input/output operations
│   ├── thread_safe_iostream.hpp    # Thread-safe I/O interface
│   ├── thread_safe_iostream.cpp    # Implementation
│   └── thread_safe_iostream_tests.cpp
├── threading/                      # Concurrent programming utilities
│   ├── threading.hpp               # Main threading header
│   ├── thread.hpp                  # Enhanced thread wrapper
│   ├── thread.cpp                  # Thread implementation
│   ├── worker_pool.hpp             # Thread pool management
│   ├── persistent_worker.hpp       # Long-running worker threads
│   ├── thread_safe_queue.hpp       # Concurrent queue implementation
│   └── threading.cpp               # Threading tests
├── network/                        # TCP networking implementation
│   ├── network.hpp                 # Main network header
│   ├── message.hpp/.cpp            # Message serialization
│   ├── client.hpp/.cpp             # TCP client implementation
│   ├── server.hpp/.cpp             # TCP server implementation
│   └── network_tests.cpp           # Network module tests
├── mathematics/                    # Mathematical computation library
│   ├── mathematics.hpp             # Main mathematics header
│   ├── ivector2.hpp                # 2D vector template class
│   ├── ivector3.hpp/.cpp           # 3D vector template class
│   ├── random_2D_coordinate_generator.hpp/.cpp
│   ├── perlin_noise_2D.hpp/.cpp    # Procedural noise generation
│   └── mathematics_tests.cpp       # Mathematics tests
├── bonus/                          # Extended functionality modules
│   ├── bonus.hpp                   # Main bonus features header
│   ├── timer_bonus.hpp/.cpp        # Duration and timeout management
│   ├── chronometer_bonus.hpp/.cpp  # Precise time measurement
│   ├── widget_bonus.hpp/.cpp       # UI widget framework
│   ├── application_bonus.hpp/.cpp  # Application lifecycle management
│   ├── observable_value_bonus.hpp/.cpp  # Value change notifications
│   ├── logger_bonus.hpp/.cpp       # Comprehensive logging system
│   └── bonus_tests.cpp             # Bonus features tests
└── README.md                       # This documentation
```

## Module Breakdown

### Core Modules

#### Data Structures Module
**Purpose**: Advanced container implementations and memory management utilities

**Key Components**:
- **Pool**: Object pool for efficient memory reuse and RAII compliance
- **DataBuffer**: Type-safe binary serialization with template support
- **Advanced Containers**: Specialized data structures for performance-critical applications

**Usage Patterns**:
```cpp
// Object Pool - Efficient resource management
Pool<ExpensiveObject> objectPool;
objectPool.resize(100);
{
    auto obj = objectPool.acquire(constructor_args...);
    // Automatic return to pool on scope exit
}

// Data Buffer - Type-safe serialization
DataBuffer buffer;
buffer << complex_object << primitive_data;
buffer >> restored_object >> restored_data;
```

#### Design Patterns Module
**Purpose**: Implementation of fundamental software design patterns

**Key Components**:
- **Memento**: State preservation and restoration for undo/redo functionality
- **Observer**: Event notification system for loose coupling
- **Singleton**: Thread-safe single instance management
- **State Machine**: Finite state automaton for complex state management

**Design Pattern Applications**:
```cpp
// Observer Pattern - Event-driven programming
class GameEventManager : public Observer<GameEvent> {
    void update(const GameEvent& event) override {
        // Handle game events
    }
};

// Singleton Pattern - Global resource management
auto& config = Singleton<GameConfig>::getInstance("Game", 4, true);

// Memento Pattern - State preservation
GameCharacter player("Hero", 100, 5);
auto snapshot = player.saveState();
player.takeDamage(50);
player.restoreState(snapshot);  // Back to full health
```

#### Threading Module
**Purpose**: Concurrent programming utilities and thread-safe operations

**Key Components**:
- **Enhanced Thread**: Wrapper with additional functionality and safety
- **WorkerPool**: Thread pool for task distribution and load balancing
- **PersistentWorker**: Long-running background processing threads
- **ThreadSafeQueue**: Lock-free concurrent queue implementation

**Concurrency Patterns**:
```cpp
// Worker Pool - Parallel task execution
WorkerPool pool(std::thread::hardware_concurrency());
auto future = pool.enqueue([](int x) { return fibonacci(x); }, 40);
auto result = future.get();

// Thread-Safe Queue - Producer-consumer pattern
ThreadSafeQueue<Task> taskQueue;
// Producer thread
taskQueue.push(task);
// Consumer thread
auto task = taskQueue.pop();
```

#### Network Module
**Purpose**: TCP-based networking with message serialization

**Key Components**:
- **Client**: TCP client with connection management and error handling
- **Server**: Multi-client TCP server with connection pooling
- **Message**: Binary message protocol with type safety
- **Network Utilities**: Connection management and error recovery

**Network Programming Patterns**:
```cpp
// TCP Server - Multi-client handling
Server server;
server.bind(8080);
server.start();
server.set_message_handler([](const Message& msg, int client_fd) {
    // Process client messages
});

// TCP Client - Reliable communication
Client client;
client.connect("127.0.0.1", 8080);
Message request("REQUEST", data);
client.send_message(request);
auto response = client.receive_message();
```

#### Mathematics Module
**Purpose**: Mathematical computation library with vector operations and procedural generation

**Key Components**:
- **IVector2/IVector3**: Template-based vector mathematics with operator overloading
- **Random2DCoordinateGenerator**: Deterministic pseudo-random generation
- **PerlinNoise2D**: Ken Perlin's noise algorithm for procedural content
- **Mathematical Utilities**: Common mathematical functions and constants

**Mathematical Operations**:
```cpp
// Vector Mathematics - 3D computations
IVector3<float> a(1.0f, 2.0f, 3.0f);
IVector3<float> b(4.0f, 5.0f, 6.0f);
auto dot_product = a.dot(b);
auto cross_product = a.cross(b);
auto normalized = a.normalize();

// Procedural Generation - Perlin noise
PerlinNoise2D noise(12345);  // Seeded for reproducibility
float height = noise.sample(x * 0.1f, y * 0.1f);
```

### Bonus Features

#### Timer System
**Purpose**: Duration tracking and timeout management for real-time applications

**Features**:
- High-precision timing with millisecond accuracy
- Start, stop, pause, and restart functionality
- Progress tracking and remaining time queries
- Background waiting capabilities

#### Chronometer System
**Purpose**: Precise time measurement with lap support for performance analysis

**Features**:
- Lap time recording and management
- Pause/resume functionality with accumulated timing
- Multiple time format outputs (HH:MM:SS.mmm)
- Performance profiling capabilities

#### Application Framework
**Purpose**: Complete application lifecycle management with widget system

**Features**:
- Hierarchical widget system (Button, Label, Panel)
- Event handling and focus management
- FPS control and timing systems
- Rendering pipeline and update cycles

#### Observable Value System
**Purpose**: Value change notification system implementing the Observer pattern

**Features**:
- Template-based value wrapper with change notifications
- Multiple observer registration with unique IDs
- Validation callbacks and conditional updates
- Thread-safe operations with specialized numeric types

#### Logger System
**Purpose**: Comprehensive logging with multiple output destinations and filtering

**Features**:
- Multiple log levels (TRACE, DEBUG, INFO, WARN, ERROR, FATAL)
- Category-based filtering and history management
- Color output support and location information
- Thread-safe operations with flexible formatting

## Class-by-Class Implementation Guide

### Data Structures Module

#### `Pool<T>` - Object Pool Template Class
**Purpose**: Provides efficient memory management through object reuse, eliminating allocation/deallocation overhead for frequently created/destroyed objects.

**Development Approach**:
- **RAII Design**: Uses `PoolHandle<T>` wrapper that automatically returns objects to pool on destruction
- **Template Flexibility**: Supports any constructible type with variadic template constructors
- **Move Semantics**: Implements move-only semantics to prevent accidental copying of expensive resources
- **Exception Safety**: Strong exception safety guarantees with automatic cleanup

```cpp
template<typename T>
class Pool {
private:
    std::vector<std::unique_ptr<T>> available_;
    std::mutex mutex_;
    
public:
    template<typename... Args>
    PoolHandle<T> acquire(Args&&... args);  // Variadic constructor forwarding
    void release(std::unique_ptr<T> obj);   // Return object to pool
};
```

**Key Design Decisions**:
- Stack-based available object storage for O(1) acquire/release
- Mutex protection for thread safety
- Smart pointer usage for automatic memory management
- Template parameter pack for flexible object construction

#### `DataBuffer` - Type-Safe Serialization Buffer
**Purpose**: Provides type-safe binary serialization with automatic type checking and memory management.

**Development Approach**:
- **Type Safety**: Uses `std::type_info` to verify deserialization type matches serialization type
- **Binary Efficiency**: Direct memory copying for POD types, avoiding parsing overhead
- **RAII Memory Management**: Automatic buffer management with proper cleanup
- **Stream-like Interface**: Operator overloading for intuitive << and >> operations

```cpp
class DataBuffer {
private:
    std::vector<uint8_t> buffer_;
    const std::type_info* stored_type_;
    
public:
    template<typename T>
    DataBuffer& operator<<(const T& data);  // Serialize data
    
    template<typename T>
    DataBuffer& operator>>(T& data);        // Deserialize data
};
```

**Key Design Decisions**:
- Vector storage for dynamic sizing and automatic memory management
- Type information storage for runtime type checking
- Template operators for compile-time type deduction
- POD type optimization using `std::is_trivially_copyable`

### Design Patterns Module

#### `Memento<T>` - State Preservation Pattern
**Purpose**: Implements the Memento pattern for state preservation and restoration, enabling undo/redo functionality.

**Development Approach**:
- **CRTP (Curiously Recurring Template Pattern)**: Enables static polymorphism for type-safe implementations
- **Snapshot Abstraction**: Uses `std::map<std::string, std::string>` for flexible state storage
- **Virtual Interface**: Pure virtual methods force concrete implementations
- **Exception Safety**: Rollback mechanisms for failed state restoration

```cpp
template<typename T>
class Memento {
public:
    using Snapshot = std::map<std::string, std::string>;
    
    Snapshot saveState() const;
    void restoreState(const Snapshot& snapshot);
    
protected:
    virtual void _saveToSnapshot(Snapshot& snapshot) const = 0;
    virtual void _loadFromSnapshot(Snapshot& snapshot) = 0;
};
```

**Key Design Decisions**:
- String-based snapshot storage for serialization flexibility
- Template design for type safety and code reuse
- Protected virtual methods for controlled inheritance
- Map-based storage for named state variables

#### `Observer<T>` - Event Notification Pattern
**Purpose**: Implements the Observer pattern for decoupled event notification and reactive programming.

**Development Approach**:
- **Template-based Events**: Type-safe event system with compile-time type checking
- **Weak Reference Management**: Prevents circular dependencies and memory leaks
- **Thread Safety**: Mutex protection for observer list modifications
- **Automatic Cleanup**: Removes invalid observers automatically

```cpp
template<typename EventType>
class Observer {
public:
    virtual void update(const EventType& event) = 0;
    virtual ~Observer() = default;
};

template<typename EventType>
class Subject {
private:
    std::vector<std::weak_ptr<Observer<EventType>>> observers_;
    mutable std::mutex observers_mutex_;
    
public:
    void attach(std::shared_ptr<Observer<EventType>> observer);
    void notify(const EventType& event);
};
```

**Key Design Decisions**:
- Weak pointer usage to prevent circular references
- Template events for type safety
- Mutex protection for thread-safe observer management
- Virtual destructor for proper inheritance cleanup

#### `Singleton<T>` - Thread-Safe Single Instance Pattern
**Purpose**: Provides thread-safe singleton implementation with lazy initialization and configurable construction.

**Development Approach**:
- **Thread-Safe Lazy Initialization**: Uses `std::call_once` for thread-safe initialization
- **Perfect Forwarding**: Variadic templates with universal references for constructor arguments
- **Friend Class Pattern**: Allows private constructor access while maintaining encapsulation
- **Static Storage**: Thread-safe static local variable for instance storage

```cpp
template<typename T>
class Singleton {
private:
    static std::unique_ptr<T> instance_;
    static std::once_flag initialized_;
    
public:
    template<typename... Args>
    static T& getInstance(Args&&... args);
    
    static bool isInstantiated();
    static void reset();
};
```

**Key Design Decisions**:
- `std::call_once` for thread-safe initialization
- Perfect forwarding for flexible constructor arguments
- Static members for global access
- Reset functionality for testing and cleanup

#### `StateMachine<StateType>` - Finite State Automaton
**Purpose**: Implements a flexible finite state machine with transition validation and callback support.

**Development Approach**:
- **Type-Safe States**: Template-based state system preventing invalid state assignments
- **Transition Validation**: Predicate-based transition guards with lambda support
- **Callback System**: Enter/exit callbacks for state change reactions
- **History Tracking**: Optional state history for debugging and analysis

```cpp
template<typename StateType>
class StateMachine {
private:
    StateType current_state_;
    std::map<std::pair<StateType, StateType>, std::function<bool()>> transitions_;
    std::map<StateType, std::function<void()>> enter_callbacks_;
    std::map<StateType, std::function<void()>> exit_callbacks_;
    
public:
    bool transition(const StateType& new_state);
    void addTransition(const StateType& from, const StateType& to, 
                      std::function<bool()> guard = nullptr);
    void setEnterCallback(const StateType& state, std::function<void()> callback);
};
```

**Key Design Decisions**:
- Map-based transition storage for flexible state relationships
- Lambda function guards for complex transition logic
- Separate enter/exit callbacks for state change reactions
- Template state types for compile-time validation

### Threading Module

#### `WorkerPool` - Thread Pool Management
**Purpose**: Manages a pool of worker threads for parallel task execution with load balancing and future-based result retrieval.

**Development Approach**:
- **Future-Based Results**: Returns `std::future<T>` for asynchronous result retrieval
- **Task Queue**: Thread-safe queue for work distribution among threads
- **RAII Lifecycle**: Automatic thread cleanup on destruction
- **Flexible Task Types**: Accepts any callable with any return type

```cpp
class WorkerPool {
private:
    std::vector<std::thread> workers_;
    ThreadSafeQueue<std::function<void()>> tasks_;
    std::atomic<bool> stop_;
    
public:
    explicit WorkerPool(size_t thread_count);
    
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
        
    ~WorkerPool();  // Joins all threads
};
```

**Key Design Decisions**:
- `std::packaged_task` for future-based result handling
- Atomic boolean for clean shutdown signaling
- Perfect forwarding for task arguments
- Thread-safe queue for work distribution

#### `ThreadSafeQueue<T>` - Lock-Free Concurrent Queue
**Purpose**: Provides a lock-free concurrent queue implementation for high-performance producer-consumer scenarios.

**Development Approach**:
- **Lock-Free Design**: Uses atomic pointers for lock-free operations
- **ABA Problem Prevention**: Memory ordering constraints prevent race conditions
- **Node-Based Storage**: Linked list structure for unbounded capacity
- **Exception Safety**: Strong exception safety with proper cleanup

```cpp
template<typename T>
class ThreadSafeQueue {
private:
    struct Node {
        std::atomic<T*> data{nullptr};
        std::atomic<Node*> next{nullptr};
    };
    
    std::atomic<Node*> head_{new Node};
    std::atomic<Node*> tail_{head_.load()};
    
public:
    void push(T item);
    bool pop(T& result);
    bool empty() const;
};
```

**Key Design Decisions**:
- Atomic pointers for lock-free synchronization
- Dummy head node for simplified operations
- Memory ordering specifications for correctness
- Value semantics for data storage

#### `PersistentWorker` - Long-Running Background Thread
**Purpose**: Manages long-running background threads with controlled startup, shutdown, and task execution.

**Development Approach**:
- **Controlled Lifecycle**: Explicit start/stop methods with proper synchronization
- **Callback-Based Tasks**: Flexible work function assignment
- **Graceful Shutdown**: Clean thread termination with resource cleanup
- **Status Monitoring**: Thread state tracking and health monitoring

```cpp
class PersistentWorker {
private:
    std::unique_ptr<std::thread> worker_thread_;
    std::atomic<bool> running_{false};
    std::atomic<bool> should_stop_{false};
    std::function<void()> work_function_;
    std::mutex state_mutex_;
    
public:
    void start(std::function<void()> work_func);
    void stop();
    bool isRunning() const;
};
```

**Key Design Decisions**:
- Atomic flags for thread-safe state management
- Function pointer storage for flexible work assignment
- Mutex protection for state changes
- Unique pointer for optional thread management

### Network Module

#### `Message` - Binary Message Protocol
**Purpose**: Provides a structured binary message format for network communication with type safety and automatic serialization.

**Development Approach**:
- **Binary Protocol**: Efficient binary format with length prefixing
- **Type Safety**: Message type identification and validation
- **Automatic Serialization**: Transparent data conversion for network transmission
- **Endianness Handling**: Network byte order conversion for cross-platform compatibility

```cpp
class Message {
private:
    std::string type_;
    std::vector<uint8_t> data_;
    
public:
    Message(const std::string& type, const std::vector<uint8_t>& data);
    
    template<typename T>
    Message(const std::string& type, const T& data);
    
    std::vector<uint8_t> serialize() const;
    static Message deserialize(const std::vector<uint8_t>& buffer);
};
```

**Key Design Decisions**:
- Length-prefixed format for streaming protocols
- String type identifiers for human-readable debugging
- Template constructors for automatic data serialization
- Static factory method for deserialization

#### `Client` - TCP Client Implementation
**Purpose**: Provides a robust TCP client with connection management, automatic reconnection, and error handling.

**Development Approach**:
- **RAII Socket Management**: Automatic socket cleanup and resource management
- **Connection Pooling**: Reusable connections with automatic lifecycle management
- **Error Recovery**: Automatic reconnection with exponential backoff
- **Asynchronous Operations**: Non-blocking I/O with timeout support

```cpp
class Client {
private:
    int socket_fd_;
    std::string server_address_;
    int server_port_;
    bool connected_;
    std::mutex connection_mutex_;
    
public:
    bool connect(const std::string& address, int port);
    bool send_message(const Message& message);
    std::optional<Message> receive_message();
    void disconnect();
};
```

**Key Design Decisions**:
- Socket file descriptor management with RAII
- Mutex protection for connection state
- Optional return types for error handling
- Blocking I/O with timeout mechanisms

#### `Server` - TCP Server Implementation
**Purpose**: Implements a multi-client TCP server with connection pooling and concurrent client handling.

**Development Approach**:
- **Multi-Client Support**: Concurrent handling of multiple client connections
- **Thread Pool Integration**: Uses worker threads for client request processing
- **Callback-Based Handling**: Flexible message processing through callbacks
- **Resource Management**: Automatic client connection cleanup

```cpp
class Server {
private:
    int server_socket_;
    std::atomic<bool> running_{false};
    std::unique_ptr<WorkerPool> worker_pool_;
    std::function<void(const Message&, int)> message_handler_;
    std::vector<int> client_sockets_;
    std::mutex clients_mutex_;
    
public:
    bool bind(int port);
    void start();
    void stop();
    void set_message_handler(std::function<void(const Message&, int)> handler);
};
```

**Key Design Decisions**:
- Worker pool for concurrent client handling
- Callback system for flexible message processing
- Atomic flags for thread-safe server control
- Vector storage for active client tracking

### Mathematics Module

#### `IVector2<T>` / `IVector3<T>` - Template Vector Classes
**Purpose**: Provides comprehensive vector mathematics with full operator overloading and mathematical operations.

**Development Approach**:
- **Template Design**: Generic implementation supporting any arithmetic type
- **Operator Overloading**: Natural mathematical syntax with full operator support
- **SFINAE Constraints**: Compile-time type checking for arithmetic types only
- **Performance Optimization**: Inline operations and move semantics

```cpp
template<typename T>
class IVector3 {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic");
    
private:
    T x_, y_, z_;
    
public:
    // Constructors with perfect forwarding
    IVector3(T x = 0, T y = 0, T z = 0) : x_(x), y_(y), z_(z) {}
    
    // Mathematical operations
    T dot(const IVector3& other) const;
    IVector3 cross(const IVector3& other) const;
    T magnitude() const;
    IVector3 normalize() const;
    
    // Operator overloading
    IVector3 operator+(const IVector3& other) const;
    IVector3& operator+=(const IVector3& other);
    // ... all mathematical operators
};
```

**Key Design Decisions**:
- Static assertions for type safety
- Const-correctness throughout the interface
- Return value optimization for performance
- Comprehensive operator overloading for natural syntax

#### `PerlinNoise2D` - Procedural Noise Generation
**Purpose**: Implements Ken Perlin's noise algorithm for smooth pseudo-random value generation in 2D space.

**Development Approach**:
- **Deterministic Algorithm**: Seeded pseudo-random generation for reproducible results
- **Gradient-Based Interpolation**: Smooth transitions using gradient vectors
- **Optimized Implementation**: Lookup tables and optimized mathematical operations
- **Configurable Parameters**: Adjustable frequency, amplitude, and octave settings

```cpp
class PerlinNoise2D {
private:
    std::array<int, 512> permutation_;  // Permutation table
    std::array<float, 8> gradients_;    // Gradient vectors
    int seed_;
    
    float fade(float t) const;          // Smoothstep function
    float lerp(float a, float b, float t) const;
    float grad(int hash, float x, float y) const;
    
public:
    explicit PerlinNoise2D(int seed = 0);
    float sample(float x, float y) const;
    float octave_sample(float x, float y, int octaves, float persistence) const;
};
```

**Key Design Decisions**:
- Lookup table optimization for performance
- Deterministic seeding for reproducible results
- Floating-point arithmetic for smooth interpolation
- Octave sampling for fractal noise generation

### Bonus Features

#### `Timer` - Duration and Timeout Management
**Purpose**: Provides high-precision timing functionality with timeout detection and progress tracking.

**Development Approach**:
- **Chrono Library Integration**: Uses `std::chrono` for high-precision timing
- **State Machine Design**: Clear start/stop/pause state management
- **Background Operations**: Non-blocking timeout detection
- **Progress Calculation**: Real-time progress tracking and remaining time

```cpp
class Timer {
private:
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point pause_time_;
    std::chrono::milliseconds duration_;
    TimerState state_;
    
public:
    void start(std::chrono::milliseconds duration);
    void stop();
    void pause();
    void resume();
    
    bool hasExpired() const;
    float getProgress() const;  // 0.0 to 1.0
    std::chrono::milliseconds getElapsed() const;
    std::chrono::milliseconds getRemaining() const;
};
```

**Key Design Decisions**:
- Steady clock for monotonic timing
- Millisecond precision for most applications
- State enumeration for clear state management
- Floating-point progress for smooth UI updates

#### `Chronometer` - Precision Time Measurement
**Purpose**: Provides stopwatch functionality with lap support and multiple time format outputs.

**Development Approach**:
- **Lap Management**: Vector storage for multiple lap times
- **Pause/Resume Support**: Accumulated timing with pause functionality
- **Multiple Formats**: Support for different time representations
- **High Precision**: Microsecond precision for performance measurement

```cpp
class Chronometer {
private:
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point pause_start_;
    std::chrono::microseconds accumulated_time_{0};
    std::vector<std::chrono::microseconds> laps_;
    bool running_;
    bool paused_;
    
public:
    void start();
    void stop();
    void pause();
    void resume();
    void lap();
    
    std::chrono::microseconds getElapsed() const;
    std::string getFormattedTime(bool include_microseconds = true) const;
    const std::vector<std::chrono::microseconds>& getLaps() const;
};
```

**Key Design Decisions**:
- Microsecond precision for detailed timing
- Vector storage for lap history
- Accumulated timing for pause support
- String formatting for human-readable output

#### `Application` & `Widget` - UI Framework
**Purpose**: Provides a complete application framework with hierarchical widget system and event handling.

**Development Approach**:
- **Composite Pattern**: Hierarchical widget tree structure
- **Event System**: Centralized event handling with propagation
- **Lifecycle Management**: Complete application startup/shutdown cycle
- **FPS Control**: Frame rate limiting and timing management

```cpp
class Application {
private:
    std::unique_ptr<Widget> root_widget_;
    Widget* focused_widget_;
    std::map<char, std::function<void()>> key_bindings_;
    float target_fps_;
    std::chrono::steady_clock::time_point last_frame_time_;
    
public:
    bool initialize();
    void run();
    void shutdown();
    
    void setRootWidget(std::unique_ptr<Widget> root);
    Widget* findWidget(const std::string& path);
    void handleKeyPress(char key);
    void handleMouseClick(int x, int y);
};

class Widget {
protected:
    std::string name_;
    int x_, y_, width_, height_;
    bool visible_;
    Widget* parent_;
    std::vector<std::unique_ptr<Widget>> children_;
    
public:
    virtual void update(float deltaTime) {}
    virtual void render() {}
    virtual void onClick(int x, int y) {}
    virtual void onKeyPress(char key) {}
    
    Widget* findChild(const std::string& name);
    void addChild(std::unique_ptr<Widget> child);
};
```

**Key Design Decisions**:
- Composite pattern for widget hierarchy
- Virtual methods for polymorphic behavior
- Smart pointers for automatic memory management
- Event propagation through widget tree

#### `ObservableValue<T>` - Value Change Notification
**Purpose**: Implements the Observer pattern for value changes with validation and arithmetic operations.

**Development Approach**:
- **Template Design**: Generic value wrapper for any type
- **Observer Management**: Multiple observer registration with unique IDs
- **Validation System**: Pre-change validation with rollback capability
- **Arithmetic Operations**: Operator overloading for natural value manipulation

```cpp
template<typename T>
class ObservableValue {
private:
    T value_;
    std::map<size_t, std::function<void(const T&, const T&)>> observers_;
    std::vector<std::function<bool(const T&, const T&)>> validators_;
    size_t next_observer_id_;
    mutable std::mutex mutex_;
    
public:
    explicit ObservableValue(const T& initial_value = T{});
    
    void set(const T& new_value);
    const T& get() const;
    
    size_t addObserver(std::function<void(const T&, const T&)> observer);
    void removeObserver(size_t id);
    void addValidator(std::function<bool(const T&, const T&)> validator);
    
    // Operator overloading for natural syntax
    ObservableValue& operator=(const T& value);
    operator const T&() const;
};
```

**Key Design Decisions**:
- Template design for type flexibility
- Map-based observer storage with unique IDs
- Validation pipeline for value constraints
- Mutex protection for thread safety

#### `Logger` - Comprehensive Logging System
**Purpose**: Provides a full-featured logging system with multiple output destinations, filtering, and formatting.

**Development Approach**:
- **Multiple Log Levels**: TRACE through FATAL with hierarchical filtering
- **Category System**: Named categories for organized logging
- **Multiple Outputs**: Console, file, and custom output destinations
- **Thread Safety**: Mutex-protected operations for concurrent logging

```cpp
class Logger {
public:
    enum class Level { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };
    
private:
    std::map<std::string, std::vector<LogEntry>> category_logs_;
    Level min_level_;
    bool color_output_;
    std::mutex log_mutex_;
    std::vector<std::unique_ptr<LogOutput>> outputs_;
    
public:
    template<typename... Args>
    void log(Level level, const std::string& category, 
             const std::string& format, Args&&... args);
    
    void setMinLevel(Level level);
    void addOutput(std::unique_ptr<LogOutput> output);
    void enableColors(bool enable);
    std::vector<LogEntry> getHistory(const std::string& category) const;
};
```

**Key Design Decisions**:
- Enumeration for type-safe log levels
- Category-based organization for filtering
- Variadic templates for printf-style formatting
- Strategy pattern for multiple output destinations

## Implementation Details

### Template Programming

libftpp extensively uses C++ templates for type safety and performance:

```cpp
// Template specialization for different numeric types
template<typename T>
class IVector3 {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
    // Implementation with full operator overloading
};

// Variadic templates for flexible object construction
template<typename T, typename... Args>
auto acquire(Args&&... args) -> PoolHandle<T> {
    return pool.construct(std::forward<Args>(args)...);
}
```

### Memory Management

RAII principles and smart pointers ensure memory safety:

```cpp
// Automatic resource management
class PoolHandle {
    std::unique_ptr<T> ptr;
    Pool<T>* pool;
public:
    ~PoolHandle() { if (pool) pool->release(std::move(ptr)); }
    // Move-only semantics for performance
};
```

### Thread Safety

Comprehensive thread safety through modern C++ synchronization:

```cpp
// Lock-free programming where possible
class ThreadSafeQueue {
    std::atomic<Node*> head{nullptr};
    std::atomic<Node*> tail{nullptr};
    // Lock-free enqueue/dequeue operations
};

// Scoped locking for critical sections
std::lock_guard<std::mutex> lock(mutex_);
```

## Performance Considerations

### Memory Efficiency
- Object pooling reduces allocation overhead
- Move semantics minimize unnecessary copies
- Template instantiation for type-specific optimizations

### Concurrency Performance
- Lock-free data structures where feasible
- Thread pool prevents thread creation overhead
- Atomic operations for high-performance synchronization

### Network Optimization
- Connection pooling and reuse
- Binary message protocol for efficiency
- Asynchronous I/O operations

## Applications

libftpp is designed for a wide range of applications:

### Game Development
- Object pooling for game entities
- Vector mathematics for physics
- State machines for game logic
- Networking for multiplayer functionality

### System Programming
- Thread pools for parallel processing
- Memory management utilities
- Inter-process communication
- Resource management with RAII

### Scientific Computing
- Mathematical vector operations
- Procedural content generation
- Data serialization and processing
- Performance measurement tools

### Network Applications
- Client-server architectures
- Message-based communication protocols
- Connection management and error handling
- Real-time data streaming

## Design Patterns

libftpp implements several fundamental design patterns:

### Creational Patterns
- **Singleton**: Thread-safe global resource management
- **Object Pool**: Efficient object reuse and memory management

### Structural Patterns
- **Composite**: Hierarchical widget system
- **Adapter**: Interface adaptation for different systems

### Behavioral Patterns
- **Observer**: Event notification and decoupling
- **State**: Complex state management with transitions
- **Memento**: State preservation and restoration
- **Strategy**: Algorithm selection and policy-based design

## Testing Framework

Comprehensive testing ensures reliability and correctness:

### Unit Tests
- Module-specific test suites
- Edge case and error condition testing
- Performance benchmarking
- Memory leak detection with AddressSanitizer

### Integration Tests
- Cross-module interaction verification
- Network communication testing
- Threading synchronization validation
- End-to-end application scenarios

### Test Execution
```bash
# Run all tests
make test_unit_1 && ./libftpp_test_1  # Basic functionality
make test_unit_2 && ./libftpp_test_2  # Thread-safe I/O
make test_unit_3 && ./libftpp_test_3  # Threading systems
make test_unit_4 && ./libftpp_test_4  # Network operations
make test_unit_5 && ./libftpp_test_5  # Mathematical functions
make test_bonus && ./libftpp_test_bonus  # Bonus features
```

## Advanced Features

### Template Metaprogramming
- SFINAE for conditional compilation
- Type traits for template specialization
- Variadic templates for flexible interfaces

### Modern C++17 Features
- `std::optional` for nullable return values
- Structured bindings for tuple unpacking
- `constexpr` for compile-time computation
- Fold expressions for parameter pack operations

### Exception Safety
- Strong exception safety guarantees
- RAII for automatic resource cleanup
- Exception-safe data structures

### Performance Optimization
- Move semantics throughout the library
- Template instantiation control
- Cache-friendly data structures
- Minimal overhead abstractions

## API Documentation

### Core Headers
```cpp
#include "libftpp.hpp"           // Complete library
#include "data_structures/data_structures.hpp"
#include "design_patterns/design_patterns.hpp"
#include "threading/threading.hpp"
#include "network/network.hpp"
#include "mathematics/mathematics.hpp"
#include "bonus/bonus.hpp"       // Extended features
```

### Key Classes
- `Pool<T>`: Object pool for resource management
- `WorkerPool`: Thread pool for parallel execution
- `Client/Server`: TCP networking components
- `IVector2<T>/IVector3<T>`: Mathematical vector operations
- `Timer/Chronometer`: Time measurement utilities
- `Application/Widget`: UI framework components

## Troubleshooting

### Common Issues

**Compilation Errors**:
- Ensure C++17 compiler support
- Verify all dependencies are installed
- Check template instantiation errors

**Runtime Issues**:
- Use AddressSanitizer for memory debugging
- Enable debug symbols with `-g` flag
- Check thread synchronization issues

**Performance Problems**:
- Profile with appropriate tools
- Verify object pool sizing
- Check thread pool configuration
- Monitor memory allocation patterns

### Debugging Tips
```bash
# Compile with debugging symbols
make CPPFLAGS="-g -DDEBUG"

# Run with AddressSanitizer
make CPPFLAGS="-fsanitize=address"

# Memory profiling
valgrind --tool=memcheck ./your_program
```

## Contributing

Contributions to libftpp are welcome! Please follow these guidelines:

1. **Code Style**: Follow the existing code style and formatting
2. **Testing**: Add comprehensive tests for new features
3. **Documentation**: Update documentation for API changes
4. **Performance**: Consider performance implications of changes
5. **Thread Safety**: Ensure thread safety where applicable

### Development Setup
```bash
git clone https://github.com/hugomgris/libftpp.git
cd libftpp
make all
make test_unit_1  # Verify basic functionality
```

## License

This project is developed as part of the 42 School curriculum. Please refer to the school's guidelines regarding code sharing and academic integrity.

---

<p align="center">
<i>libftpp - A comprehensive C++ library showcasing modern programming practices and software engineering principles.</i>
</p>
