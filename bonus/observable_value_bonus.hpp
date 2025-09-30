/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observable_value_bonus.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:20:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/30 09:22:32 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBSERVABLE_VALUE_BONUS_HPP
# define OBSERVABLE_VALUE_BONUS_HPP

#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <mutex>

/**
ObservableValue notifies subscribers when the value is modified

Template class that wraps a value and provides notification mechanisms
when the value changes. Supports multiple observers and thread-safe operations.
*/
template<typename T>
class ObservableValue {
	public:
		using ValueType = T;
		using ChangeCallback = std::function<void(const T& oldValue, const T& newValue)>;
		using ValidationCallback = std::function<bool(const T& newValue)>;
		using ObserverID = size_t;

	private:
		T _value;
		std::vector<std::pair<ObserverID, ChangeCallback>> _observers;
		std::vector<ValidationCallback> _validators;
		mutable std::mutex _mutex;
		ObserverID _nextObserverID;
		std::string _name;
		bool _notificationsEnabled;

	public:
		// Constructors
		ObservableValue();
		explicit ObservableValue(const T& initialValue);
		ObservableValue(const T& initialValue, const std::string& name);
		ObservableValue(const ObservableValue& other);
		ObservableValue& operator=(const ObservableValue& other);
		~ObservableValue();

		// Value access
		const T& get() const;
		T& getRef();
		void set(const T& newValue);
		bool trySet(const T& newValue);
		
		// Operators
		ObservableValue& operator=(const T& newValue);
		operator const T&() const;
		T* operator->();
		const T* operator->() const;
		T& operator*();
		const T& operator*() const;
		
		// Comparison operators
		bool operator==(const T& other) const;
		bool operator!=(const T& other) const;
		bool operator<(const T& other) const;
		bool operator<=(const T& other) const;
		bool operator>(const T& other) const;
		bool operator>=(const T& other) const;

		// Observer management
		ObserverID addObserver(ChangeCallback callback);
		bool removeObserver(ObserverID observerID);
		void removeAllObservers();
		size_t getObserverCount() const;
		
		// Validation
		void addValidator(ValidationCallback validator);
		void removeAllValidators();
		bool isValid(const T& value) const;
		
		// Notification control
		void enableNotifications();
		void disableNotifications();
		bool areNotificationsEnabled() const;
		void notifyObservers(const T& oldValue, const T& newValue);
		
		// Utility
		const std::string& getName() const;
		void setName(const std::string& name);
		std::string toString() const;
		
		// Conditional updates
		template<typename Predicate>
		bool setIf(const T& newValue, Predicate predicate) {
			std::lock_guard<std::mutex> lock(_mutex);
			if (predicate(_value, newValue)) {
				if (validateValue(newValue) && _value != newValue) {
					T oldValue = _value;
					_value = newValue;
					
					if (_notificationsEnabled) {
						notifyObserversInternal(oldValue, _value);
					}
					return true;
				}
			}
			return false;
		}
		
		void transform(std::function<T(const T&)> transformer);
		
		// Batch operations
		class BatchUpdate {
			ObservableValue<T>& _observable;
			T _oldValue;
			bool _committed;
			
	public:
		BatchUpdate(ObservableValue<T>& observable);
		~BatchUpdate();
		void commit();
		void cancel();
	};
	
	BatchUpdate beginBatch();

	private:
		void notifyObserversInternal(const T& oldValue, const T& newValue);
		bool validateValue(const T& value) const;
};


template<typename T>
class ObservableNumber : public ObservableValue<T> {
	static_assert(std::is_arithmetic_v<T>, "ObservableNumber requires arithmetic type");

	public:
		using ObservableValue<T>::ObservableValue;
		using ObservableValue<T>::operator=;
		
		// Arithmetic operations
		ObservableNumber& operator+=(const T& value);
		ObservableNumber& operator-=(const T& value);
		ObservableNumber& operator*=(const T& value);
		ObservableNumber& operator/=(const T& value);
		ObservableNumber& operator%=(const T& value);
		
		ObservableNumber& operator++();
		ObservableNumber operator++(int);
		ObservableNumber& operator--();
		ObservableNumber operator--(int);
		
		// Range constraints
		void setRange(const T& min, const T& max);
		void setMin(const T& min);
		void setMax(const T& max);
		T getMin() const;
		T getMax() const;
		bool hasRange() const;
		
		// Utility methods
		void increment(const T& step = T(1));
		void decrement(const T& step = T(1));
		void clamp();
		bool isInRange() const;

	private:
		T _minValue;
		T _maxValue;
		bool _hasRange;
		
		void setupRangeValidator();
};

template<typename Container>
class ObservableContainer : public ObservableValue<Container> {
	public:
		using ObservableValue<Container>::ObservableValue;
		using ValueType = typename Container::value_type;
		using SizeType = typename Container::size_type;
		
		// Container-specific notifications
		using AddCallback = std::function<void(const ValueType& item, SizeType index)>;
		using RemoveCallback = std::function<void(const ValueType& item, SizeType index)>;
		using ClearCallback = std::function<void()>;
		
		// Container operations with notifications
		void push_back(const ValueType& item);
		void pop_back();
		void clear();
		SizeType size() const;
		bool empty() const;
		
		// Observer management for container events
		typename ObservableValue<Container>::ObserverID addAddObserver(AddCallback callback);
		typename ObservableValue<Container>::ObserverID addRemoveObserver(RemoveCallback callback);
		typename ObservableValue<Container>::ObserverID addClearObserver(ClearCallback callback);

	private:
		std::vector<std::pair<typename ObservableValue<Container>::ObserverID, AddCallback>> _addObservers;
		std::vector<std::pair<typename ObservableValue<Container>::ObserverID, RemoveCallback>> _removeObservers;
		std::vector<std::pair<typename ObservableValue<Container>::ObserverID, ClearCallback>> _clearObservers;
};

// Type aliases for common usage
using ObservableInt = ObservableNumber<int>;
using ObservableFloat = ObservableNumber<float>;
using ObservableDouble = ObservableNumber<double>;
using ObservableString = ObservableValue<std::string>;
using ObservableBool = ObservableValue<bool>;

#endif
