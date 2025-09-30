/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   application_bonus.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:20:39 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLICATION_BONUS_HPP
# define APPLICATION_BONUS_HPP

#include "widget_bonus.hpp"
#include <chrono>
#include <functional>
#include <map>

/*
Application class for creating custom applications with interesting behaviors

Manages the main application loop, widget hierarchy, event handling,
and provides a framework for building interactive applications.
 */
class Application {
	private:
		std::string _title;
		int _width, _height;
		bool _running;
		bool _initialized;
		std::unique_ptr<Widget> _rootWidget;
		Widget* _focusedWidget;
		
		// Timing
		std::chrono::steady_clock::time_point _lastFrameTime;
		float _deltaTime;
		float _targetFPS;
		
		// Event handling
		std::map<char, std::function<void()>> _keyBindings;
		std::function<void()> _updateCallback;
		std::function<void()> _renderCallback;
		
		// Statistics
		long long _frameCount;
		float _totalRunTime;
		float _averageFPS;

	public:
		// Constructors
		Application(const std::string& title = "Application", int width = 800, int height = 600);
		Application(const Application& other) = delete;
		Application& operator=(const Application& other) = delete;
		~Application();

		// Application lifecycle
		bool initialize();
		void run();
		void shutdown();
		void quit();
		
		// Properties
		const std::string& getTitle() const;
		void setTitle(const std::string& title);
		int getWidth() const;
		int getHeight() const;
		void setSize(int width, int height);
		
		// FPS and timing
		void setTargetFPS(float fps);
		float getTargetFPS() const;
		float getCurrentFPS() const;
		float getDeltaTime() const;
		float getTotalRunTime() const;
		long long getFrameCount() const;
		
		// Widget management
		void setRootWidget(std::unique_ptr<Widget> root);
		Widget* getRootWidget() const;
		Widget* findWidget(const std::string& path);
		void setFocusedWidget(Widget* widget);
		Widget* getFocusedWidget() const;
		
		// Event handling
		void bindKey(char key, std::function<void()> callback);
		void unbindKey(char key);
		void handleKeyPress(char key);
		void handleMouseClick(int x, int y);
		
		// Callbacks
		void setUpdateCallback(std::function<void()> callback);
		void setRenderCallback(std::function<void()> callback);
		
		// Virtual methods for derived applications
		virtual void onInitialize();
		virtual void onUpdate(float deltaTime);
		virtual void onRender();
		virtual void onShutdown();
		virtual void onKeyPress(char key);
		virtual void onMouseClick(int x, int y);
		
		// Utility methods
		bool isRunning() const;
		bool isInitialized() const;
		void sleep(int milliseconds);
		void printStatistics() const;
		
		// Static utilities
		static Application* getCurrentApplication();
		static void setCurrentApplication(Application* app);

	private:
		void updateTiming();
		void limitFrameRate();
		Widget* findWidgetAt(int x, int y, Widget* root = nullptr);
		
		static Application* _currentApplication;
};

class SampleApplication : public Application {
	public:
		SampleApplication();
		virtual void onInitialize() override;
		virtual void onUpdate(float deltaTime) override;
		virtual void onKeyPress(char key) override;

	private:
		void setupUI();
		void onButtonClick();
		void onQuitClick();
		
		int _counter;
		Label* _counterLabel;
		Button* _incrementButton;
};

#endif
