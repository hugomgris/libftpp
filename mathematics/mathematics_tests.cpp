/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathematics_tests.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:41:52 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 16:51:33 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ivector2.hpp"
#include "ivector3.hpp"
#include "../colors.h"

void testBasicIVector2Operations() {
	try {
		std::cout << MAG << "\n=== Testing IVector2 ===" << RESET << std::endl;

		IVector2<int> vector1(42, 42);
		std::cout << "Vector1: ";
		vector1.print();

		IVector2<int> vector2(58, 58);
		std::cout << "Vector2: ";
		vector2.print();

		IVector2<int> vectorSum_1 = vector1 + vector2;
		std::cout << "Sum of Vector1 and Vector2: ";
		vectorSum_1.print();

		IVector2<double> vector3(42.42, 42.42);
		std::cout << "\nVector3: ";
		vector3.print();

		IVector2<double> vector4(57.58, 57.58);
		std::cout << "Vector4: ";
		vector4.print();

		IVector2<double> vectorSum_2 = vector3 - vector4;
		std::cout << "Substraction of Vector3 and Vector4: ";
		vectorSum_2.print();

		IVector2<float> vector5(5.5f, 5.5f);
		std::cout << "\nVector5: ";
		vector5.print();

		IVector2<float> vector6(2.2f, 2.2f);
		std::cout << "Vector6: ";
		vector6.print();

		IVector2<float> vectorSum_3 = vector5 * vector6;
		std::cout << "Multiplication of Vector5 and Vector6: ";
		vectorSum_3.print();

		IVector2<float> vectorSum_4 = vector5 / vector6;
		std::cout << "Division of Vector5 and Vector6: ";
		vectorSum_4.print();

		IVector2<int> vector8(42, 42);
		std::cout << "\nVector8: ";
		vector8.print();

		std::cout << "Testing if vector 1 == vector8: " << (vector1 == vector8 ? "true" : "false") << std::endl;
		std::cout << "Testing if vector 2 == vector8: " << (vector2 == vector8 ? "true" : "false") << std::endl;

		IVector2<int> vector9(3, 3);
		std::cout << "\nVector9: ";
		vector9.print();

		std::cout << "Testing vector9's length: " << vector9.length() << std::endl;
		IVector2 vector2_normalized = vector5.normalize();
		std::cout << "Testing vector5's normalization: " << std::endl;
		vector2_normalized.print();

		std::cout << "Testing dot product of vectors 5 and 6: " << vector5.dot(vector6) << std::endl;

		IVector2<int> vector7(0, 0);
		std::cout << "\nVector7: ";
		vector7.print();

		IVector2<int> vector10 = vector1;
		std::cout << "Random copy constructor test by copying Vector1:";
		vector1.print();

		IVector2<int> vectorSum_5 = vector1 / vector7;
		std::cout << "\nDivision of Vector1 and Vector7: ";
		vectorSum_5.print();
	} catch (std::invalid_argument &e) {
		std::cout << RED << "Caught exception: " << e.what() << RESET << std::endl;
	}
}

void testBasicIVector3Operations() {
	try {
		std::cout << MAG << "\n=== Testing IVector3 ===" << RESET << std::endl;

		IVector3<int> vector1(42, 42, 42);
		std::cout << "Vector1: ";
		vector1.print();

		IVector3<int> vector2(58, 58, 58);
		std::cout << "Vector2: ";
		vector2.print();

		IVector3<int> vectorSum_1 = vector1 + vector2;
		std::cout << "Sum of Vector1 and Vector2: ";
		vectorSum_1.print();

		IVector3<double> vector3(42.42, 42.42, 42.42);
		std::cout << "\nVector3: ";
		vector3.print();

		IVector3<double> vector4(57.58, 57.58, 57.58);
		std::cout << "Vector4: ";
		vector4.print();

		IVector3<double> vectorSum_2 = vector3 - vector4;
		std::cout << "Substraction of Vector3 and Vector4: ";
		vectorSum_2.print();

		IVector3<float> vector5(5.5f, 5.5f, 5.5f);
		std::cout << "\nVector5: ";
		vector5.print();

		IVector3<float> vector6(2.2f, 2.2f, 2.2f);
		std::cout << "Vector6: ";
		vector6.print();

		IVector3<float> vectorSum_3 = vector5 * vector6;
		std::cout << "Multiplication of Vector5 and Vector6: ";
		vectorSum_3.print();

		IVector3<float> vectorSum_4 = vector5 / vector6;
		std::cout << "Division of Vector5 and Vector6: ";
		vectorSum_4.print();

		IVector3<int> vector8(42, 42, 42);
		std::cout << "\nVector8: ";
		vector8.print();

		std::cout << "Testing if vector 1 == vector8: " << (vector1 == vector8 ? "true" : "false") << std::endl;
		std::cout << "Testing if vector 2 == vector8: " << (vector2 == vector8 ? "true" : "false") << std::endl;

		IVector3<int> vector9(3, 4, 5);
		std::cout << "\nVector9: ";
		vector9.print();

		std::cout << "Testing vector9's length: " << vector9.length() << std::endl;
		IVector3<float> vector3_normalized = vector5.normalize();
		std::cout << "Testing vector5's normalization: " << std::endl;
		vector3_normalized.print();

		std::cout << "Testing dot product of vectors 5 and 6: " << vector5.dot(vector6) << std::endl;

		// Test proper 3D cross product
		IVector3<int> vectorA(1, 0, 0);  // Unit vector along X
		IVector3<int> vectorB(0, 1, 0);  // Unit vector along Y
		std::cout << "\nVectorA (X-axis): ";
		vectorA.print();
		std::cout << "VectorB (Y-axis): ";
		vectorB.print();
		
		IVector3<int> crossProduct = vectorA.cross(vectorB);
		std::cout << "Cross product A x B (should be Z-axis): ";
		crossProduct.print();

		IVector3<int> vector7(0, 0, 0);
		std::cout << "\nVector7 (zero vector): ";
		vector7.print();

		IVector3<int> vector10 = vector1;
		std::cout << "Random copy constructor test by copying Vector1:";
		vector1.print();

		IVector3<int> vectorSum_5 = vector1 / vector7;
		std::cout << "\nDivision of Vector1 and Vector7: ";
		vectorSum_5.print();
	} catch (std::invalid_argument &e) {
		std::cout << RED << "Caught exception: " << e.what() << RESET << std::endl;
	}
}

int main(void) {
	std::cout << CYN << "====== MATHEMATICS tests ======" << RESET << std::endl;
	testBasicIVector2Operations();
	testBasicIVector3Operations();

	std::cout << GRN << "\nAll mathematics tests completed!" << RESET << std::endl;
}