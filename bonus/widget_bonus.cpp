/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   widget_bonus.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:36:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "widget_bonus.hpp"
#include "application_bonus.hpp"
#include <iostream>
#include <sstream>

// Widget implementation
Widget::Widget(const std::string& name)
    : _name(name), _x(0), _y(0), _width(100), _height(30)
    , _visible(true), _enabled(true), _parent(nullptr), _application(nullptr) {}

Widget::Widget(const std::string& name, int x, int y, int width, int height)
    : _name(name), _x(x), _y(y), _width(width), _height(height)
    , _visible(true), _enabled(true), _parent(nullptr), _application(nullptr) {}

Widget::~Widget() {}

// Basic properties
const std::string& Widget::getName() const {
    return _name;
}

void Widget::setName(const std::string& name) {
    _name = name;
}

// Position and size
void Widget::setPosition(int x, int y) {
    _x = x;
    _y = y;
}

void Widget::setSize(int width, int height) {
    _width = width;
    _height = height;
}

void Widget::setBounds(int x, int y, int width, int height) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

int Widget::getX() const { return _x; }
int Widget::getY() const { return _y; }
int Widget::getWidth() const { return _width; }
int Widget::getHeight() const { return _height; }

// Visibility and state
void Widget::setVisible(bool visible) {
    if (_visible != visible) {
        _visible = visible;
        if (visible) {
            onShow();
        } else {
            onHide();
        }
    }
}

bool Widget::isVisible() const {
    return _visible;
}

void Widget::setEnabled(bool enabled) {
    if (_enabled != enabled) {
        _enabled = enabled;
        if (enabled) {
            onEnable();
        } else {
            onDisable();
        }
    }
}

bool Widget::isEnabled() const {
    return _enabled;
}

// Hierarchy management
void Widget::addChild(std::unique_ptr<Widget> child) {
    if (child) {
        child->_parent = this;
        child->setApplication(_application);
        _children.push_back(std::move(child));
    }
}

Widget* Widget::removeChild(const std::string& name) {
    for (auto it = _children.begin(); it != _children.end(); ++it) {
        if ((*it)->getName() == name) {
            Widget* widget = it->release();
            widget->_parent = nullptr;
            widget->setApplication(nullptr);
            _children.erase(it);
            return widget;
        }
    }
    return nullptr;
}

Widget* Widget::findChild(const std::string& name) {
    for (const auto& child : _children) {
        if (child->getName() == name) {
            return child.get();
        }
    }
    return nullptr;
}

const Widget* Widget::findChild(const std::string& name) const {
    for (const auto& child : _children) {
        if (child->getName() == name) {
            return child.get();
        }
    }
    return nullptr;
}

Widget* Widget::getParent() const {
    return _parent;
}

const std::vector<std::unique_ptr<Widget>>& Widget::getChildren() const {
    return _children;
}

// Application context
void Widget::setApplication(Application* app) {
    _application = app;
    for (const auto& child : _children) {
        child->setApplication(app);
    }
}

Application* Widget::getApplication() const {
    return _application;
}

// Virtual methods
void Widget::update(float deltaTime) {
    if (!_visible || !_enabled) {
        return;
    }
    
    for (const auto& child : _children) {
        child->update(deltaTime);
    }
}

void Widget::render() {
    if (!_visible) {
        return;
    }
    
    for (const auto& child : _children) {
        child->render();
    }
}

void Widget::onShow() {}
void Widget::onHide() {}
void Widget::onEnable() {}
void Widget::onDisable() {}
void Widget::onClick(int, int) {} // Unused parameters
void Widget::onKeyPress(char) {} // Unused parameter
void Widget::onFocus() {}
void Widget::onBlur() {}

// Utility methods
bool Widget::contains(int x, int y) const {
    return x >= _x && x < _x + _width && y >= _y && y < _y + _height;
}

std::string Widget::getFullPath() const {
    if (_parent) {
        return _parent->getFullPath() + "/" + _name;
    }
    return _name;
}

void Widget::show() { setVisible(true); }
void Widget::hide() { setVisible(false); }
void Widget::enable() { setEnabled(true); }
void Widget::disable() { setEnabled(false); }

// Button implementation
Button::Button(const std::string& name, const std::string& text)
    : Widget(name), _text(text) {}

Button::Button(const std::string& name, const std::string& text, int x, int y, int width, int height)
    : Widget(name, x, y, width, height), _text(text) {}

void Button::setText(const std::string& text) {
    _text = text;
}

const std::string& Button::getText() const {
    return _text;
}

void Button::setClickCallback(std::function<void()> callback) {
    _clickCallback = callback;
}

void Button::render() {
    if (!_visible) return;
    
    std::cout << "[Button: " << _name << " '" << _text << "' at (" 
              << _x << "," << _y << ") size " << _width << "x" << _height << "]";
    Widget::render();
}

void Button::onClick(int x, int y) {
    if (_enabled && contains(x, y) && _clickCallback) {
        _clickCallback();
    }
}

// Label implementation
Label::Label(const std::string& name, const std::string& text)
    : Widget(name), _text(text), _alignment("left") {}

Label::Label(const std::string& name, const std::string& text, int x, int y, int width, int height)
    : Widget(name, x, y, width, height), _text(text), _alignment("left") {}

void Label::setText(const std::string& text) {
    _text = text;
}

const std::string& Label::getText() const {
    return _text;
}

void Label::setAlignment(const std::string& alignment) {
    if (alignment == "left" || alignment == "center" || alignment == "right") {
        _alignment = alignment;
    }
}

const std::string& Label::getAlignment() const {
    return _alignment;
}

void Label::render() {
    if (!_visible) return;
    
    std::cout << "[Label: " << _name << " '" << _text << "' (" << _alignment 
              << ") at (" << _x << "," << _y << ") size " << _width << "x" << _height << "]";
    Widget::render();
}

// Panel implementation
Panel::Panel(const std::string& name)
    : Widget(name), _backgroundColor("default"), _hasBorder(false) {}

Panel::Panel(const std::string& name, int x, int y, int width, int height)
    : Widget(name, x, y, width, height), _backgroundColor("default"), _hasBorder(false) {}

void Panel::setBackgroundColor(const std::string& color) {
    _backgroundColor = color;
}

const std::string& Panel::getBackgroundColor() const {
    return _backgroundColor;
}

void Panel::setBorder(bool hasBorder) {
    _hasBorder = hasBorder;
}

bool Panel::hasBorder() const {
    return _hasBorder;
}

void Panel::render() {
    if (!_visible) return;
    
    std::cout << "[Panel: " << _name << " bg:" << _backgroundColor 
              << (_hasBorder ? " bordered" : "") << " at (" << _x << "," << _y 
              << ") size " << _width << "x" << _height << "]";
    Widget::render();
}

void Panel::update(float deltaTime) {
    Widget::update(deltaTime);
}
