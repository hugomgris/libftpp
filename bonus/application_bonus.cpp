/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application_bonus.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:21:27 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "application_bonus.hpp"
#include <iostream>
#include <thread>
#include <sstream>

// Static member definition
Application* Application::_currentApplication = nullptr;

// Application implementation
Application::Application(const std::string& title, int width, int height)
	: _title(title), _width(width), _height(height), _running(false), _initialized(false)
	, _focusedWidget(nullptr), _deltaTime(0.0f), _targetFPS(60.0f)
	, _frameCount(0), _totalRunTime(0.0f), _averageFPS(0.0f) {}

Application::~Application() {
	if (_currentApplication == this) {
		_currentApplication = nullptr;
	}
}

// Application lifecycle
bool Application::initialize() {
	if (_initialized) {
		return true;
	}
	
	_lastFrameTime = std::chrono::steady_clock::now();
	_frameCount = 0;
	_totalRunTime = 0.0f;
	_initialized = true;
	
	setCurrentApplication(this);
	onInitialize();
	
	return true;
}

void Application::run() {
	if (!initialize()) {
		std::cerr << "Failed to initialize application" << std::endl;
		return;
	}
	
	_running = true;
	std::cout << "Starting application: " << _title << " (" << _width << "x" << _height << ")" << std::endl;
	
	while (_running) {
		updateTiming();
		
		// Update phase
		if (_rootWidget) {
			_rootWidget->update(_deltaTime);
		}
		onUpdate(_deltaTime);
		if (_updateCallback) {
			_updateCallback();
		}
		
		// Render phase
		if (_rootWidget) {
			_rootWidget->render();
		}
		onRender();
		if (_renderCallback) {
			_renderCallback();
		}
		
		// Simulate some input (for demonstration)
		if (_frameCount % 180 == 0) { // Every 3 seconds at 60 FPS
			handleKeyPress('u'); // Simulate 'u' key press
		}
		
		limitFrameRate();
		_frameCount++;
		
		// Auto-quit after 5 seconds for demonstration
		if (_totalRunTime > 5.0f) {
			quit();
		}
	}
	
	shutdown();
}

void Application::shutdown() {
	_running = false;
	onShutdown();
	std::cout << "Application shutdown complete." << std::endl;
	printStatistics();
}

void Application::quit() {
	_running = false;
}

// Properties
const std::string& Application::getTitle() const {
	return _title;
}

void Application::setTitle(const std::string& title) {
	_title = title;
}

int Application::getWidth() const {
	return _width;
}

int Application::getHeight() const {
	return _height;
}

void Application::setSize(int width, int height) {
	_width = width;
	_height = height;
}

// FPS and timing
void Application::setTargetFPS(float fps) {
	_targetFPS = fps > 0 ? fps : 60.0f;
}

float Application::getTargetFPS() const {
	return _targetFPS;
}

float Application::getCurrentFPS() const {
	return _averageFPS;
}

float Application::getDeltaTime() const {
	return _deltaTime;
}

float Application::getTotalRunTime() const {
	return _totalRunTime;
}

long long Application::getFrameCount() const {
	return _frameCount;
}

// Widget management
void Application::setRootWidget(std::unique_ptr<Widget> root) {
	_rootWidget = std::move(root);
	if (_rootWidget) {
		_rootWidget->setApplication(this);
	}
}

Widget* Application::getRootWidget() const {
	return _rootWidget.get();
}

Widget* Application::findWidget(const std::string& path) {
	if (!_rootWidget || path.empty()) {
		return nullptr;
	}
	
	if (path == _rootWidget->getName()) {
		return _rootWidget.get();
	}
	
	return _rootWidget->findChild(path);
}

void Application::setFocusedWidget(Widget* widget) {
	if (_focusedWidget != widget) {
		if (_focusedWidget) {
			_focusedWidget->onBlur();
		}
		_focusedWidget = widget;
		if (_focusedWidget) {
			_focusedWidget->onFocus();
		}
	}
}

Widget* Application::getFocusedWidget() const {
	return _focusedWidget;
}

// Event handling
void Application::bindKey(char key, std::function<void()> callback) {
	_keyBindings[key] = callback;
}

void Application::unbindKey(char key) {
	_keyBindings.erase(key);
}

void Application::handleKeyPress(char key) {
	// Check for specific key bindings first
	auto it = _keyBindings.find(key);
	if (it != _keyBindings.end()) {
		it->second();
		return;
	}
	
	// Pass to focused widget
	if (_focusedWidget) {
		_focusedWidget->onKeyPress(key);
	}
	
	// Pass to application
	onKeyPress(key);
}

void Application::handleMouseClick(int x, int y) {
	Widget* clickedWidget = findWidgetAt(x, y);
	if (clickedWidget) {
		setFocusedWidget(clickedWidget);
		clickedWidget->onClick(x, y);
	}
	onMouseClick(x, y);
}

// Callbacks
void Application::setUpdateCallback(std::function<void()> callback) {
	_updateCallback = callback;
}

void Application::setRenderCallback(std::function<void()> callback) {
	_renderCallback = callback;
}

// Virtual methods
void Application::onInitialize() {}
void Application::onUpdate(float) {} // Unused parameter
void Application::onRender() {}
void Application::onShutdown() {}
void Application::onKeyPress(char) {} // Unused parameter
void Application::onMouseClick(int, int) {} // Unused parameters

// Utility methods
bool Application::isRunning() const {
	return _running;
}

bool Application::isInitialized() const {
	return _initialized;
}

void Application::sleep(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void Application::printStatistics() const {
	std::cout << "\n=== Application Statistics ===" << std::endl;
	std::cout << "Total runtime: " << _totalRunTime << " seconds" << std::endl;
	std::cout << "Total frames: " << _frameCount << std::endl;
	std::cout << "Average FPS: " << _averageFPS << std::endl;
	std::cout << "Target FPS: " << _targetFPS << std::endl;
}

// Static utilities
Application* Application::getCurrentApplication() {
	return _currentApplication;
}

void Application::setCurrentApplication(Application* app) {
	_currentApplication = app;
}

// Private methods
void Application::updateTiming() {
	auto now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastFrameTime);
	_deltaTime = duration.count() / 1000000.0f; // Convert to seconds
	_totalRunTime += _deltaTime;
	_lastFrameTime = now;
	
	// Calculate running average FPS
	if (_totalRunTime > 0) {
		_averageFPS = _frameCount / _totalRunTime;
	}
}

void Application::limitFrameRate() {
	if (_targetFPS <= 0) {
		return;
	}
	
	float targetFrameTime = 1.0f / _targetFPS;
	if (_deltaTime < targetFrameTime) {
		float sleepTime = targetFrameTime - _deltaTime;
		int sleepMs = static_cast<int>(sleepTime * 1000);
		if (sleepMs > 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
		}
	}
}

Widget* Application::findWidgetAt(int x, int y, Widget* root) {
	if (!root) {
		root = _rootWidget.get();
	}
	
	if (!root || !root->isVisible() || !root->contains(x, y)) {
		return nullptr;
	}
	
	// Check children first (front to back)
	const auto& children = root->getChildren();
	for (auto it = children.rbegin(); it != children.rend(); ++it) {
		Widget* found = findWidgetAt(x, y, it->get());
		if (found) {
			return found;
		}
	}
	
	return root;
}

// SampleApplication implementation
SampleApplication::SampleApplication() 
	: Application("Sample Application", 400, 300), _counter(0), _counterLabel(nullptr), _incrementButton(nullptr) {}

void SampleApplication::onInitialize() {
	setupUI();
	
	// Bind keys
	bindKey('q', [this]() { quit(); });
	bindKey(' ', [this]() { onButtonClick(); });
	
	std::cout << "Sample Application initialized!" << std::endl;
	std::cout << "Press 'q' to quit, SPACE to increment counter, 'u' for updates" << std::endl;
}

void SampleApplication::onUpdate(float) {
	// Update counter label if it exists
	if (_counterLabel) {
		std::ostringstream oss;
		oss << "Counter: " << _counter;
		_counterLabel->setText(oss.str());
	}
}

void SampleApplication::onKeyPress(char key) {
	if (key == 'u') {
		std::cout << "\nFrame: " << getFrameCount() 
				<< ", FPS: " << getCurrentFPS() 
				<< ", Runtime: " << getTotalRunTime() << "s" << std::endl;
	}
}

void SampleApplication::setupUI() {
	auto mainPanel = std::make_unique<Panel>("MainPanel", 0, 0, getWidth(), getHeight());
	mainPanel->setBackgroundColor("lightgray");
	mainPanel->setBorder(true);
	
	auto titleLabel = std::make_unique<Label>("TitleLabel", "Sample Application", 50, 20, 300, 30);
	titleLabel->setAlignment("center");
	
	auto counterLabel = std::make_unique<Label>("CounterLabel", "Counter: 0", 50, 80, 200, 30);
	_counterLabel = counterLabel.get();
	
	auto incrementButton = std::make_unique<Button>("IncrementButton", "Increment", 50, 120, 100, 40);
	_incrementButton = incrementButton.get();
	_incrementButton->setClickCallback([this]() { onButtonClick(); });
	
	auto quitButton = std::make_unique<Button>("QuitButton", "Quit", 170, 120, 80, 40);
	quitButton->setClickCallback([this]() { onQuitClick(); });
	
	mainPanel->addChild(std::move(titleLabel));
	mainPanel->addChild(std::move(counterLabel));
	mainPanel->addChild(std::move(incrementButton));
	mainPanel->addChild(std::move(quitButton));
	
	setRootWidget(std::move(mainPanel));
}

void SampleApplication::onButtonClick() {
	_counter++;
	std::cout << "Button clicked! Counter: " << _counter << std::endl;
}

void SampleApplication::onQuitClick() {
	std::cout << "Quit button clicked!" << std::endl;
	quit();
}
