/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observable_value_bonus.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:22:19 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "observable_value_bonus.hpp"
#include <sstream>
#include <algorithm>
#include <limits>

// Template explicit instantiations for common types
template class ObservableValue<int>;
template class ObservableValue<float>;
template class ObservableValue<double>;
template class ObservableValue<std::string>;
template class ObservableValue<bool>;

template class ObservableNumber<int>;
template class ObservableNumber<float>;
template class ObservableNumber<double>;

// ObservableValue implementation
template<typename T>
ObservableValue<T>::ObservableValue() 
	: _value{}, _nextObserverID(1), _name("ObservableValue"), _notificationsEnabled(true) {}

template<typename T>
ObservableValue<T>::ObservableValue(const T& initialValue)
	: _value(initialValue), _nextObserverID(1), _name("ObservableValue"), _notificationsEnabled(true) {}

template<typename T>
ObservableValue<T>::ObservableValue(const T& initialValue, const std::string& name)
	: _value(initialValue), _nextObserverID(1), _name(name), _notificationsEnabled(true) {}

template<typename T>
ObservableValue<T>::ObservableValue(const ObservableValue& other) {
	std::lock_guard<std::mutex> lock(other._mutex);
	_value = other._value;
	_observers = other._observers;
	_validators = other._validators;
	_nextObserverID = other._nextObserverID;
	_name = other._name;
	_notificationsEnabled = other._notificationsEnabled;
}

template<typename T>
ObservableValue<T>& ObservableValue<T>::operator=(const ObservableValue& other) {
	if (this != &other) {
		std::lock_guard<std::mutex> lock1(_mutex);
		std::lock_guard<std::mutex> lock2(other._mutex);
		T oldValue = _value;
		_value = other._value;
		_observers = other._observers;
		_validators = other._validators;
		_nextObserverID = other._nextObserverID;
		_name = other._name;
		_notificationsEnabled = other._notificationsEnabled;
		
		if (_notificationsEnabled) {
			notifyObserversInternal(oldValue, _value);
		}
	}
	return *this;
}

template<typename T>
ObservableValue<T>::~ObservableValue() {}

// Value access
template<typename T>
const T& ObservableValue<T>::get() const {
	std::lock_guard<std::mutex> lock(_mutex);
	return _value;
}

template<typename T>
T& ObservableValue<T>::getRef() {
	std::lock_guard<std::mutex> lock(_mutex);
	return _value;
}

template<typename T>
void ObservableValue<T>::set(const T& newValue) {
	std::lock_guard<std::mutex> lock(_mutex);
	
	if (!validateValue(newValue)) {
		return; // Validation failed
	}
	
	if (_value != newValue) {
		T oldValue = _value;
		_value = newValue;
		
		if (_notificationsEnabled) {
			notifyObserversInternal(oldValue, _value);
		}
	}
}

template<typename T>
bool ObservableValue<T>::trySet(const T& newValue) {
	std::lock_guard<std::mutex> lock(_mutex);
	
	if (!validateValue(newValue)) {
		return false;
	}
	
	if (_value != newValue) {
		T oldValue = _value;
		_value = newValue;
		
		if (_notificationsEnabled) {
			notifyObserversInternal(oldValue, _value);
		}
	}
	return true;
}

// Operators
template<typename T>
ObservableValue<T>& ObservableValue<T>::operator=(const T& newValue) {
	set(newValue);
	return *this;
}

template<typename T>
ObservableValue<T>::operator const T&() const {
	return get();
}

template<typename T>
T* ObservableValue<T>::operator->() {
	std::lock_guard<std::mutex> lock(_mutex);
	return &_value;
}

template<typename T>
const T* ObservableValue<T>::operator->() const {
	std::lock_guard<std::mutex> lock(_mutex);
	return &_value;
}

template<typename T>
T& ObservableValue<T>::operator*() {
	std::lock_guard<std::mutex> lock(_mutex);
	return _value;
}

template<typename T>
const T& ObservableValue<T>::operator*() const {
	std::lock_guard<std::mutex> lock(_mutex);
	return _value;
}

// Comparison operators
template<typename T>
bool ObservableValue<T>::operator==(const T& other) const {
	return get() == other;
}

template<typename T>
bool ObservableValue<T>::operator!=(const T& other) const {
	return get() != other;
}

template<typename T>
bool ObservableValue<T>::operator<(const T& other) const {
	return get() < other;
}

template<typename T>
bool ObservableValue<T>::operator<=(const T& other) const {
	return get() <= other;
}

template<typename T>
bool ObservableValue<T>::operator>(const T& other) const {
	return get() > other;
}

template<typename T>
bool ObservableValue<T>::operator>=(const T& other) const {
	return get() >= other;
}

// Observer management
template<typename T>
typename ObservableValue<T>::ObserverID ObservableValue<T>::addObserver(ChangeCallback callback) {
	std::lock_guard<std::mutex> lock(_mutex);
	ObserverID id = _nextObserverID++;
	_observers.emplace_back(id, callback);
	return id;
}

template<typename T>
bool ObservableValue<T>::removeObserver(ObserverID observerID) {
	std::lock_guard<std::mutex> lock(_mutex);
	auto it = std::find_if(_observers.begin(), _observers.end(),
		[observerID](const auto& pair) { return pair.first == observerID; });
	
	if (it != _observers.end()) {
		_observers.erase(it);
		return true;
	}
	return false;
}

template<typename T>
void ObservableValue<T>::removeAllObservers() {
	std::lock_guard<std::mutex> lock(_mutex);
	_observers.clear();
}

template<typename T>
size_t ObservableValue<T>::getObserverCount() const {
	std::lock_guard<std::mutex> lock(_mutex);
	return _observers.size();
}

// Validation
template<typename T>
void ObservableValue<T>::addValidator(ValidationCallback validator) {
	std::lock_guard<std::mutex> lock(_mutex);
	_validators.push_back(validator);
}

template<typename T>
void ObservableValue<T>::removeAllValidators() {
	std::lock_guard<std::mutex> lock(_mutex);
	_validators.clear();
}

template<typename T>
bool ObservableValue<T>::isValid(const T& value) const {
	std::lock_guard<std::mutex> lock(_mutex);
	return validateValue(value);
}

// Notification control
template<typename T>
void ObservableValue<T>::enableNotifications() {
	std::lock_guard<std::mutex> lock(_mutex);
	_notificationsEnabled = true;
}

template<typename T>
void ObservableValue<T>::disableNotifications() {
	std::lock_guard<std::mutex> lock(_mutex);
	_notificationsEnabled = false;
}

template<typename T>
bool ObservableValue<T>::areNotificationsEnabled() const {
	std::lock_guard<std::mutex> lock(_mutex);
	return _notificationsEnabled;
}

template<typename T>
void ObservableValue<T>::notifyObservers(const T& oldValue, const T& newValue) {
	std::lock_guard<std::mutex> lock(_mutex);
	if (_notificationsEnabled) {
		notifyObserversInternal(oldValue, newValue);
	}
}

// Utility
template<typename T>
const std::string& ObservableValue<T>::getName() const {
	std::lock_guard<std::mutex> lock(_mutex);
	return _name;
}

template<typename T>
void ObservableValue<T>::setName(const std::string& name) {
	std::lock_guard<std::mutex> lock(_mutex);
	_name = name;
}

template<typename T>
std::string ObservableValue<T>::toString() const {
	std::ostringstream oss;
	oss << _name << ": " << get();
	return oss.str();
}

// Conditional updates
template<typename T>
void ObservableValue<T>::transform(std::function<T(const T&)> transformer) {
	std::lock_guard<std::mutex> lock(_mutex);
	T newValue = transformer(_value);
	if (validateValue(newValue) && _value != newValue) {
		T oldValue = _value;
		_value = newValue;
		
		if (_notificationsEnabled) {
			notifyObserversInternal(oldValue, _value);
		}
	}
}

// Private methods
template<typename T>
void ObservableValue<T>::notifyObserversInternal(const T& oldValue, const T& newValue) {
	for (const auto& observer : _observers) {
		observer.second(oldValue, newValue);
	}
}

template<typename T>
bool ObservableValue<T>::validateValue(const T& value) const {
	for (const auto& validator : _validators) {
		if (!validator(value)) {
			return false;
		}
	}
	return true;
}

// BatchUpdate implementation
template<typename T>
ObservableValue<T>::BatchUpdate::BatchUpdate(ObservableValue<T>& observable)
	: _observable(observable), _oldValue(observable.get()), _committed(false) {
	_observable.disableNotifications();
}

template<typename T>
ObservableValue<T>::BatchUpdate::~BatchUpdate() {
	if (!_committed) {
		cancel();
	}
}

template<typename T>
void ObservableValue<T>::BatchUpdate::commit() {
	if (!_committed) {
		_committed = true;
		_observable.enableNotifications();
		_observable.notifyObservers(_oldValue, _observable.get());
	}
}

template<typename T>
void ObservableValue<T>::BatchUpdate::cancel() {
	if (!_committed) {
		_committed = true;
		_observable.set(_oldValue);
		_observable.enableNotifications();
	}
}

template<typename T>
typename ObservableValue<T>::BatchUpdate ObservableValue<T>::beginBatch() {
	return BatchUpdate(*this);
}

// ObservableNumber implementation
template<typename T>
ObservableNumber<T>& ObservableNumber<T>::operator+=(const T& value) {
	this->transform([value](const T& current) { return current + value; });
	return *this;
}

template<typename T>
ObservableNumber<T>& ObservableNumber<T>::operator-=(const T& value) {
	this->transform([value](const T& current) { return current - value; });
	return *this;
}

template<typename T>
ObservableNumber<T>& ObservableNumber<T>::operator*=(const T& value) {
	this->transform([value](const T& current) { return current * value; });
	return *this;
}

template<typename T>
ObservableNumber<T>& ObservableNumber<T>::operator/=(const T& value) {
	this->transform([value](const T& current) { return current / value; });
	return *this;
}

template<typename T>
ObservableNumber<T>& ObservableNumber<T>::operator%=(const T& value) {
	if constexpr (std::is_integral_v<T>) {
		this->transform([value](const T& current) { return current % value; });
	}
	return *this;
}

template<typename T>
ObservableNumber<T>& ObservableNumber<T>::operator++() {
	*this += T(1);
	return *this;
}

template<typename T>
ObservableNumber<T> ObservableNumber<T>::operator++(int) {
	ObservableNumber<T> temp(*this);
	++(*this);
	return temp;
}

template<typename T>
ObservableNumber<T>& ObservableNumber<T>::operator--() {
	*this -= T(1);
	return *this;
}

template<typename T>
ObservableNumber<T> ObservableNumber<T>::operator--(int) {
	ObservableNumber<T> temp(*this);
	--(*this);
	return temp;
}
