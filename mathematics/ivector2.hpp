/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector2.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:15:19 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 16:43:13 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IVECTOR2_HPP
# define IVECTOR2_HPP

# include <iostream>
# include <stdexcept>
# include <cmath>

template<typename TType>
struct IVector2{
	TType x;
	TType y;

	IVector2(): x(TType{}), y (TType{}) {}
	IVector2(TType x, TType y): x(x), y(y) {}

	IVector2(const IVector2 &other) {
		*this = other;
	}

	IVector2 &operator=(const IVector2 &other) {
		if (this != &other) {
			this->x = other.x;
			this->y = other.y;
		}
		return *this;
	}

	~IVector2() = default;

	IVector2 operator+(const IVector2 &other) const{
		IVector2 sum;

		sum.x = this->x + other.x;
		sum.y = this->y + other.y;

		return sum;
	}

	IVector2 operator-(const IVector2 &other) const{
		IVector2 sub;

		sub.x = this->x - other.x;
		sub.y = this->y - other.y;

		return sub;
	}

	IVector2 operator*(const IVector2 &other) const{
		IVector2 mul;

		mul.x = this->x * other.x;
		mul.y = this->y * other.y;

		return mul;
	}

	IVector2 operator/(const IVector2 &other) const{
		if (other.x == TType{} || other.y == TType{}) {
			throw std::invalid_argument("Division by 0");
		}
		IVector2 div;

		div.x = this->x / other.x;
		div.y = this->y / other.y;

		return div;
	}

	bool operator==(const IVector2 &other) const{
		if (this->x == other.x && this->y == other.y) { return true; } else { return false; }
	}

	bool operator!=(const IVector2 &other) const{
		if (this->x == other.x && this->y == other.y) { return false; } else { return true; }
	}

	float length() const{
		return (sqrt(pow(x, 2) + pow(y, 2)));
	}

	float dot(const IVector2 &other) const{
		float dot = 0.0f;

		dot = std::fma(this->x, other.x, dot);
		dot = std::fma(this->y, other.y, dot);

		return (dot);
	}

	IVector2 cross() const{
		return (IVector2(0.0, 0.0));
	}

	IVector2<float> normalize() const{
		IVector2 normalized;
		float length = this->length();
		
		std::cout << "Length at normalize: " << length << std::endl;
		normalized.x = x / length;
		normalized.y = y / length;

		return normalized;
	}

	void print() const{
		std::cout << "(" << x << ", " << y << ")" << std::endl;
	}
};

#endif