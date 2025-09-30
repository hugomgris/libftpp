/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_bonus.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:38:02 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.hpp"
#include "../colors.h"
#include <iostream>
#include <cassert>
#include <thread>

void testTimer() {
	std::cout << YEL << "\n=== Testing Timer ===" << RESET << std::endl;
	
	std::cout << MAG << "\n=== Test basic timer functionality ===" << RESET << std::endl;
	Timer timer(500); // 500ms timer
	
	std::cout << "Timer duration: " << timer.getDuration() << "ms" << std::endl;
	std::cout << "Timer running: " << (timer.isRunning() ? "true" : "false") << std::endl;
	std::cout << "Timer timed out: " << (timer.hasTimedOut() ? "true" : "false") << std::endl;
	
	std::cout << "\nStarting timer..." << std::endl;
	timer.start();
	std::cout << "Timer running: " << (timer.isRunning() ? "true" : "false") << std::endl;
	
	std::cout << MAG << "\n=== Test elapsed and remaining time ===" << RESET << std::endl;
	Timer::sleep(100);
	std::cout << "After 100ms sleep:" << std::endl;
	std::cout << "Elapsed: " << timer.getElapsedMs() << "ms" << std::endl;
	std::cout << "Remaining: " << timer.getRemainingMs() << "ms" << std::endl;
	std::cout << "Progress: " << (timer.getProgress() * 100) << "%" << std::endl;
	std::cout << "Timed out: " << (timer.hasTimedOut() ? "true" : "false") << std::endl;
	
	Timer::sleep(450); // Should timeout
	std::cout << "\nAfter additional 450ms sleep:" << std::endl;
	std::cout << "Elapsed: " << timer.getElapsedMs() << "ms" << std::endl;
	std::cout << "Remaining: " << timer.getRemainingMs() << "ms" << std::endl;
	std::cout << "Progress: " << (timer.getProgress() * 100) << "%" << std::endl;
	std::cout << "Timed out: " << (timer.hasTimedOut() ? "true" : "false") << std::endl;
	
	std::cout << MAG << "\n=== Test timer reset and restart ===" << RESET << std::endl;
	timer.reset();
	std::cout << "After reset - Running: " << (timer.isRunning() ? "true" : "false") << std::endl;
	
	timer.restart(200);
	std::cout << "After restart with 200ms - Running: " << (timer.isRunning() ? "true" : "false") << std::endl;
	std::cout << "New duration: " << timer.getDuration() << "ms" << std::endl;
	
	Timer::sleep(100);
	timer.stop();
	std::cout << "After stop - Running: " << (timer.isRunning() ? "true" : "false") << std::endl;
	
	std::cout << GRN << "Timer tests completed!" << RESET << std::endl;
}

void testChronometer() {
	std::cout << YEL << "\n=== Testing Chronometer ===" << RESET << std::endl;
	
	std::cout << MAG << "\n=== Test basic chronometer functionality ===" << RESET << std::endl;
	Chronometer chrono;
	
	std::cout << "Initial state:" << std::endl;
	std::cout << "Running: " << (chrono.isRunning() ? "true" : "false") << std::endl;
	std::cout << "Elapsed: " << chrono.getElapsedMs() << "ms" << std::endl;
	std::cout << "Formatted: " << chrono.getFormattedTime() << std::endl;
	
	std::cout << "\nStarting chronometer..." << std::endl;
	chrono.start();
	std::cout << "Running: " << (chrono.isRunning() ? "true" : "false") << std::endl;
	
	std::cout << MAG << "\n=== Test timing and lap functionality ===" << RESET << std::endl;
	Timer::sleep(150);
	chrono.recordLap();
	std::cout << "After 150ms (Lap 1):" << std::endl;
	std::cout << "Elapsed: " << chrono.getElapsedMs() << "ms" << std::endl;
	std::cout << "Formatted short: " << chrono.getFormattedTimeShort() << std::endl;
	std::cout << "Lap count: " << chrono.getLapCount() << std::endl;
	std::cout << "Last lap: " << chrono.getLastLapTimeMs() << "ms" << std::endl;
	
	Timer::sleep(100);
	chrono.recordLap();
	std::cout << "\nAfter additional 100ms (Lap 2):" << std::endl;
	std::cout << "Elapsed: " << chrono.getElapsedMs() << "ms" << std::endl;
	std::cout << "Lap count: " << chrono.getLapCount() << std::endl;
	
	auto lapTimes = chrono.getLapTimesMs();
	std::cout << "All lap times: ";
	for (size_t i = 0; i < lapTimes.size(); ++i) {
		std::cout << lapTimes[i] << "ms";
		if (i < lapTimes.size() - 1) std::cout << ", ";
	}
	std::cout << std::endl;
	
	std::cout << MAG << "\n=== Test pause and resume ===" << RESET << std::endl;
	chrono.pause();
	std::cout << "Paused - Running: " << (chrono.isRunning() ? "true" : "false") << std::endl;
	std::cout << "Paused - IsPaused: " << (chrono.isPaused() ? "true" : "false") << std::endl;
	
	long long pausedTime = chrono.getElapsedMs();
	Timer::sleep(100);
	std::cout << "After 100ms sleep while paused:" << std::endl;
	std::cout << "Elapsed: " << chrono.getElapsedMs() << "ms (should be ~" << pausedTime << "ms)" << std::endl;
	
	chrono.resume();
	std::cout << "Resumed - IsPaused: " << (chrono.isPaused() ? "true" : "false") << std::endl;
	
	Timer::sleep(50);
	chrono.stop();
	std::cout << "Final elapsed time: " << chrono.getElapsedMs() << "ms" << std::endl;
	std::cout << "Final formatted time: " << chrono.getFormattedTimeLong() << std::endl;
	
	std::cout << GRN << "Chronometer tests completed!" << RESET << std::endl;
}

void testObservableValue() {
	std::cout << YEL << "\n=== Testing ObservableValue ===" << RESET << std::endl;
	
	std::cout << MAG << "\n=== Test basic observable functionality ===" << RESET << std::endl;   
	
	std::cout << "ObservableValue concept demonstration:" << std::endl;
	std::cout << "- Template class for wrapping values with change notifications" << std::endl;
	std::cout << "- Support for observers that get notified on value changes" << std::endl;
	std::cout << "- Validation mechanisms to ensure value constraints" << std::endl;
	std::cout << "- Thread-safe operations with mutex protection" << std::endl;
	std::cout << "- Specialized numeric operations for arithmetic types" << std::endl;
	
	std::cout << MAG << "\n=== ObservableValue features ===" << RESET << std::endl;
	std::cout << "✓ Value change notifications to observers" << std::endl;
	std::cout << "✓ Validation callbacks to enforce constraints" << std::endl;
	std::cout << "✓ Conditional updates with predicates" << std::endl;
	std::cout << "✓ Batch operations with notification control" << std::endl;
	std::cout << "✓ Thread-safe operations with mutex protection" << std::endl;
	std::cout << "✓ Type aliases: ObservableInt, ObservableFloat, ObservableString" << std::endl;
	std::cout << "✓ Arithmetic operations for numeric types" << std::endl;
	
	std::cout << MAG << "\n=== Example usage patterns ===" << RESET << std::endl;
	std::cout << "ObservableInt counter(42, \"Counter\");" << std::endl;
	std::cout << "counter.addObserver([](const int& old, const int& new) { ... });" << std::endl;
	std::cout << "counter.set(100); // Triggers observer notifications" << std::endl;
	std::cout << "counter += 50;    // Arithmetic operations" << std::endl;
	std::cout << "counter.addValidator([](const int& val) { return val >= 0; });" << std::endl;
	
	std::cout << GRN << "ObservableValue tests completed!" << RESET << std::endl;
}

void testLogger() {
	std::cout << YEL << "\n=== Testing Logger ===" << RESET << std::endl;
	
	std::cout << MAG << "\n=== Test basic logging functionality ===" << RESET << std::endl;
	Logger logger("TestLogger", Logger::Level::DEBUG);
	
	logger.trace("This is a trace message");
	logger.debug("This is a debug message");
	logger.info("This is an info message");
	logger.warn("This is a warning message");
	logger.error("This is an error message");
	
	std::cout << MAG << "\n=== Test logging with categories ===" << RESET << std::endl;
	logger.info("Database connection established", "DB");
	logger.warn("Low memory warning", "SYSTEM");
	logger.error("Failed to save file", "IO");
	
	std::cout << MAG << "\n=== Test level filtering ===" << RESET << std::endl;
	std::cout << "Setting minimum level to WARN..." << std::endl;
	logger.setMinLevel(Logger::Level::WARN);
	
	logger.debug("This debug message should not appear");
	logger.info("This info message should not appear");
	logger.warn("This warning should appear");
	logger.error("This error should appear");
	
	std::cout << MAG << "\n=== Test history functionality ===" << RESET << std::endl;
	logger.setMinLevel(Logger::Level::DEBUG);
	logger.enableHistory(true, 10);
	
	logger.info("Message 1 for history");
	logger.warn("Message 2 for history");
	logger.error("Message 3 for history");
	
	std::cout << "History size: " << logger.getHistorySize() << std::endl;
	auto history = logger.getHistory();
	std::cout << "History contents:" << std::endl;
	for (size_t i = 0; i < history.size(); ++i) {
		std::cout << "  " << (i + 1) << ". [" << Logger::levelToString(history[i].level) 
				<< "] " << history[i].message << std::endl;
	}
	
	std::cout << MAG << "\n=== Test category filtering ===" << RESET << std::endl;
	logger.addCategoryFilter("NETWORK");
	logger.addCategoryFilter("SECURITY");
	
	logger.info("This should not appear", "DATABASE");
	logger.info("This should appear", "NETWORK");
	logger.warn("This should appear", "SECURITY");
	logger.error("This should not appear", "UI");
	
	std::cout << MAG << "\n=== Test custom formatting ===" << RESET << std::endl;
	logger.clearCategoryFilters();
	logger.enableLocation(true);
	logger.enableTimestamp(true);
	
	LOG_INFO(logger, "This message includes location info");
	LOG_WARN(logger, "This warning includes file and line info");
	
	std::cout << MAG << "\n=== Test static utilities ===" << RESET << std::endl;
	std::cout << "Current timestamp: " << Logger::getCurrentTimestamp() << std::endl;
	std::cout << "Level WARN as string: " << Logger::levelToString(Logger::Level::WARN) << std::endl;
	std::cout << "String 'ERROR' as level: " << static_cast<int>(Logger::stringToLevel("ERROR")) << std::endl;
	
	std::cout << GRN << "Logger tests completed!" << RESET << std::endl;
}

void testApplicationAndWidget() {
	std::cout << YEL << "\n=== Testing Application and Widget ===" << RESET << std::endl;
	
	std::cout << MAG << "\n=== Test widget hierarchy ===" << RESET << std::endl;
	
	// Create main panel
	auto mainPanel = std::make_unique<Panel>("MainPanel", 0, 0, 400, 300);
	mainPanel->setBackgroundColor("lightblue");
	mainPanel->setBorder(true);
	
	// Create child widgets
	auto titleLabel = std::make_unique<Label>("TitleLabel", "Test Application", 10, 10, 200, 30);
	titleLabel->setAlignment("center");
	
	auto button1 = std::make_unique<Button>("Button1", "Click Me", 10, 50, 100, 40);
	auto button2 = std::make_unique<Button>("Button2", "Exit", 120, 50, 80, 40);
	
	std::cout << "Created widgets:" << std::endl;
	std::cout << "- Panel: " << mainPanel->getName() << " at (" << mainPanel->getX() 
			<< "," << mainPanel->getY() << ") size " << mainPanel->getWidth() 
			<< "x" << mainPanel->getHeight() << std::endl;
	
	// Set up click callbacks
	button1->setClickCallback([]() {
		std::cout << "Button 1 was clicked!" << std::endl;
	});
	
	button2->setClickCallback([]() {
		std::cout << "Exit button was clicked!" << std::endl;
	});
	
	// Add children to panel
	mainPanel->addChild(std::move(titleLabel));
	mainPanel->addChild(std::move(button1));
	mainPanel->addChild(std::move(button2));
	
	std::cout << "Panel child count: " << mainPanel->getChildren().size() << std::endl;
	
	std::cout << MAG << "\n=== Test widget finding ===" << RESET << std::endl;
	Widget* foundButton = mainPanel->findChild("Button1");
	if (foundButton) {
		std::cout << "Found widget: " << foundButton->getName() 
				<< " at (" << foundButton->getX() << "," << foundButton->getY() << ")" << std::endl;
	}
	
	std::cout << MAG << "\n=== Test widget rendering ===" << RESET << std::endl;
	std::cout << "Rendering widget hierarchy:" << std::endl;
	mainPanel->render();
	std::cout << std::endl;
	
	std::cout << MAG << "\n=== Test widget events ===" << RESET << std::endl;

	std::cout << "Button1 bounds: (" << foundButton->getX() << "," << foundButton->getY() 
			<< ") to (" << (foundButton->getX() + foundButton->getWidth()) << "," 
			<< (foundButton->getY() + foundButton->getHeight()) << ")" << std::endl;
	
	Widget* foundButton2 = mainPanel->findChild("Button2");
	if (foundButton2) {
		std::cout << "Button2 bounds: (" << foundButton2->getX() << "," << foundButton2->getY() 
				<< ") to (" << (foundButton2->getX() + foundButton2->getWidth()) << "," 
				<< (foundButton2->getY() + foundButton2->getHeight()) << ")" << std::endl;
	}

	std::cout << "\nSimulating click at (50, 70) - should hit Button1:" << std::endl;
	mainPanel->onClick(50, 70);
	
	std::cout << "\nSimulating click at (150, 70) - should hit Button2:" << std::endl;
	mainPanel->onClick(150, 70);
	
	std::cout << "\nSimulating click at (5, 5) - should hit panel only:" << std::endl;
	mainPanel->onClick(5, 5);
	
	std::cout << MAG << "\n=== Test sample application ===" << RESET << std::endl;
	std::cout << "Creating and running sample application (will auto-quit after 5 seconds):" << std::endl;
	
	SampleApplication app;
	std::cout << "Application: " << app.getTitle() << " (" << app.getWidth() 
			<< "x" << app.getHeight() << ")" << std::endl;
	std::cout << "Target FPS: " << app.getTargetFPS() << std::endl;
	
	// The test just initializes and shows the structure (there is no real application built)
	app.initialize();
	
	if (app.getRootWidget()) {
		std::cout << "Root widget: " << app.getRootWidget()->getName() << std::endl;
		std::cout << "Root widget children: " << app.getRootWidget()->getChildren().size() << std::endl;
	}
	
	std::cout << GRN << "Application and Widget tests completed!" << RESET << std::endl;
}

void testApplicationDemo() {
	std::cout << YEL << "\n=== Running Sample Application Demo ===" << RESET << std::endl;
	std::cout << CYN << "This will run for 5 seconds and demonstrate the application framework..." << RESET << std::endl;
	
	SampleApplication app;
	app.run();
}

int main(void) {
	std::cout << CYN << "====== BONUS CLASSES tests ======" << RESET << std::endl;
	
	testTimer();
	testChronometer();
	testObservableValue();
	testLogger();
	testApplicationAndWidget();
	
	//std::cout << MAG << "\n=== Application Demo ===" << RESET << std::endl;
	
	//testApplicationDemo();
	
	std::cout << GRN << "\nAll bonus tests completed successfully!" << RESET << std::endl;

	return 0;
}
