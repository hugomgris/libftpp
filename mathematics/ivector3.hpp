/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ivector3.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:45:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 16:51:33 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IVECTOR3_HPP
# define IVECTOR3_HPP

# include <iostream>
# include <stdexcept>
# include <cmath>

template<typename TType>
struct IVector3{
	TType x;
	TType y;
	TType z;

	IVector3(): x(TType{}), y(TType{}), z(TType{}) {}
	IVector3(TType x, TType y, TType z): x(x), y(y), z(z) {}

	IVector3(const IVector3 &other) {
		*this = other;
	}

	IVector3 &operator=(const IVector3 &other) {
		if (this != &other) {
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
		}
		return *this;
	}

	~IVector3() = default;

	IVector3 operator+(const IVector3 &other) const{
		IVector3 sum;

		sum.x = this->x + other.x;
		sum.y = this->y + other.y;
		sum.z = this->z + other.z;

		return sum;
	}

	IVector3 operator-(const IVector3 &other) const{
		IVector3 sub;

		sub.x = this->x - other.x;
		sub.y = this->y - other.y;
		sub.z = this->z - other.z;

		return sub;
	}

	IVector3 operator*(const IVector3 &other) const{
		IVector3 mul;

		mul.x = this->x * other.x;
		mul.y = this->y * other.y;
		mul.z = this->z * other.z;

		return mul;
	}

	IVector3 operator/(const IVector3 &other) const{
		if (other.x == TType{} || other.y == TType{} || other.z == TType{}) {
			throw std::invalid_argument("Division by 0");
		}
		IVector3 div;

		div.x = this->x / other.x;
		div.y = this->y / other.y;
		div.z = this->z / other.z;

		return div;
	}

	bool operator==(const IVector3 &other) const{
		return (this->x == other.x && this->y == other.y && this->z == other.z);
	}

	bool operator!=(const IVector3 &other) const{
		return !(this->x == other.x && this->y == other.y && this->z == other.z);
	}

	float length() const{
		return (sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
	}

	float dot(const IVector3 &other) const{
		float dot = 0.0f;

		dot = std::fma(this->x, other.x, dot);
		dot = std::fma(this->y, other.y, dot);
		dot = std::fma(this->z, other.z, dot);

		return (dot);
	}

	IVector3 cross(const IVector3 &other) const{
		IVector3 result;
		
		result.x = this->y * other.z - this->z * other.y;
		result.y = this->z * other.x - this->x * other.z;
		result.z = this->x * other.y - this->y * other.x;

		return result;
	}

	IVector3<float> normalize() const{
		float length = this->length();
		
		if (length == 0.0f) {
			throw std::invalid_argument("Cannot normalize zero vector");
		}
		
		IVector3<float> normalized;
		normalized.x = static_cast<float>(x) / length;
		normalized.y = static_cast<float>(y) / length;
		normalized.z = static_cast<float>(z) / length;

		return normalized;
	}

	void print() const{
		std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
	}
};

#endif
