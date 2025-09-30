/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   widget_bonus.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:25:04 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WIDGET_BONUS_HPP
# define WIDGET_BONUS_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>

// Forward declaration
class Application;

/*
Base Widget class for creating UI components

Provides a foundation for creating interactive widgets with positioning,
sizing, event handling, and hierarchical structure.
*/
class Widget {
protected:
	std::string _name;
	int _x, _y;
	int _width, _height;
	bool _visible;
	bool _enabled;
	Widget* _parent;
	std::vector<std::unique_ptr<Widget>> _children;
	Application* _application;

public:
	// Constructors
	Widget(const std::string& name = "Widget");
	Widget(const std::string& name, int x, int y, int width, int height);
	Widget(const Widget& other) = delete;
	Widget& operator=(const Widget& other) = delete;
	virtual ~Widget();

	// Basic properties
	const std::string& getName() const;
	void setName(const std::string& name);
	
	// Position and size
	void setPosition(int x, int y);
	void setSize(int width, int height);
	void setBounds(int x, int y, int width, int height);
	int getX() const;
	int getY() const;
	int getWidth() const;
	int getHeight() const;
	
	// Visibility and state
	void setVisible(bool visible);
	bool isVisible() const;
	void setEnabled(bool enabled);
	bool isEnabled() const;
	
	// Hierarchy management
	void addChild(std::unique_ptr<Widget> child);
	Widget* removeChild(const std::string& name);
	Widget* findChild(const std::string& name);
	const Widget* findChild(const std::string& name) const;
	Widget* getParent() const;
	const std::vector<std::unique_ptr<Widget>>& getChildren() const;
	
	// Application context
	void setApplication(Application* app);
	Application* getApplication() const;
	
	// Virtual methods for derived classes
	virtual void update(float deltaTime);
	virtual void render();
	virtual void onShow();
	virtual void onHide();
	virtual void onEnable();
	virtual void onDisable();
	
	// Event handling (simplified)
	virtual void onClick(int x, int y);
	virtual void onKeyPress(char key);
	virtual void onFocus();
	virtual void onBlur();
	
	// Utility methods
	bool contains(int x, int y) const;
	std::string getFullPath() const;
	void show();
	void hide();
	void enable();
	void disable();
};

class Button : public Widget {
private:
	std::string _text;
	std::function<void()> _clickCallback;

public:
	Button(const std::string& name, const std::string& text = "Button");
	Button(const std::string& name, const std::string& text, int x, int y, int width, int height);
	
	void setText(const std::string& text);
	const std::string& getText() const;
	
	void setClickCallback(std::function<void()> callback);
	
	virtual void render() override;
	virtual void onClick(int x, int y) override;
};

class Label : public Widget {
private:
	std::string _text;
	std::string _alignment;

public:
	Label(const std::string& name, const std::string& text = "Label");
	Label(const std::string& name, const std::string& text, int x, int y, int width, int height);
	
	void setText(const std::string& text);
	const std::string& getText() const;
	
	void setAlignment(const std::string& alignment);
	const std::string& getAlignment() const;
	
	virtual void render() override;
};

class Panel : public Widget {
private:
	std::string _backgroundColor;
	bool _hasBorder;

public:
	Panel(const std::string& name);
	Panel(const std::string& name, int x, int y, int width, int height);
	
	void setBackgroundColor(const std::string& color);
	const std::string& getBackgroundColor() const;
	
	void setBorder(bool hasBorder);
	bool hasBorder() const;
	
	virtual void render() override;
	virtual void update(float deltaTime) override;
};

#endif
