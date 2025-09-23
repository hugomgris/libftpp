/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mathematics_tests.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:41:52 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:17:14 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ivector2.hpp"
#include "ivector3.hpp"
#include "random_2D_coordinate_generator.hpp"
#include "perlin_noise_2D.hpp"
#include "../colors.h"

void testBasicIVector2Operations() {
	try {
		std::cout << YEL << "\n=== Testing IVector2 ===" << RESET << std::endl;

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
		std::cout << YEL << "\n=== Testing IVector3 ===" << RESET << std::endl;

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

void testRandom2DCoordinateGenerator() {
	std::cout << YEL << "\n=== Testing Random2DCoordinateGenerator ===" << RESET << std::endl;

	std::cout << MAG << "\n=== Test basic functionality ===" << RESET << std::endl;
	Random2DCoordinateGenerator gen1(12345);
	std::cout << "Generator 1 seed: " << gen1.seed() << std::endl;
	
	std::cout << MAG << "\n=== Test deterministic behavior - same inputs should give same outputs ===" << RESET << std::endl;
	long long result1 = gen1(100, 200);
	long long result2 = gen1(100, 200);
	std::cout << "Same coordinates (100, 200) with same seed:" << std::endl;
	std::cout << "First call: " << result1 << std::endl;
	std::cout << "Second call: " << result2 << std::endl;
	std::cout << "Are they equal? " << (result1 == result2 ? "YES" : "NO") << std::endl;
	
	std::cout << MAG << "\n=== Test different coordinates give different results ===" << RESET << std::endl;
	long long result3 = gen1(101, 200);
	long long result4 = gen1(100, 201);
	std::cout << "\nDifferent coordinates:" << std::endl;
	std::cout << "Coordinates (100, 200): " << result1 << std::endl;
	std::cout << "Coordinates (101, 200): " << result3 << std::endl;
	std::cout << "Coordinates (100, 201): " << result4 << std::endl;
	
	std::cout << MAG << "\n=== Test different seeds give different results ===" << RESET << std::endl;
	Random2DCoordinateGenerator gen2(54321);
	long long result5 = gen2(100, 200);
	std::cout << "\nSame coordinates (100, 200) with different seed:" << std::endl;
	std::cout << "Seed 12345: " << result1 << std::endl;
	std::cout << "Seed 54321: " << result5 << std::endl;
	std::cout << "Are they different? " << (result1 != result5 ? "YES" : "NO") << std::endl;
	
	std::cout << MAG << "\n=== Test with negative coordinates ===" << RESET << std::endl;
	long long result6 = gen1(-50, -75);
	long long result7 = gen1(-50, -75);
	std::cout << "\nNegative coordinates (-50, -75):" << std::endl;
	std::cout << "First call: " << result6 << std::endl;
	std::cout << "Second call: " << result7 << std::endl;
	std::cout << "Consistent? " << (result6 == result7 ? "YES" : "NO") << std::endl;
	
	std::cout << MAG << "\n=== Test zero coordinates ===" << RESET << std::endl;
	long long result8 = gen1(0, 0);
	std::cout << "\nZero coordinates (0, 0): " << result8 << std::endl;
	
	std::cout << MAG << "\n=== Test large coordinates ===" << RESET << std::endl;
	long long result9 = gen1(1000000, 2000000);
	std::cout << "Large coordinates (1000000, 2000000): " << result9 << std::endl;
	
	std::cout << MAG << "\n=== Test distribution sample (show that nearby coordinates give different values) ===" << RESET << std::endl;
	std::cout << "\nDistribution sample (nearby coordinates):" << std::endl;
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			long long val = gen1(i, j);
			// Show last 4 digits for easier reading
			std::cout << (val % 10000) << "\t";
		}
		std::cout << std::endl;
	}
	
	std::cout << GRN << "Random2DCoordinateGenerator tests completed!" << RESET << std::endl;
}

void testPerlinNoise2D() {
	std::cout << YEL << "\n=== Testing PerlinNoise2D ===" << RESET << std::endl;

	std::cout << MAG << "\n=== Test basic functionality with default seed ===" << RESET << std::endl;
	PerlinNoise2D noise1;
	std::cout << "Testing basic Perlin noise functionality:" << std::endl;
	
	std::cout << MAG << "\n=== Test deterministic behavior - same coordinates should give same results ===" << RESET << std::endl;
	float result1 = noise1.sample(1.5f, 2.3f);
	float result2 = noise1.sample(1.5f, 2.3f);
	std::cout << "Same coordinates (1.5, 2.3):" << std::endl;
	std::cout << "First call: " << result1 << std::endl;
	std::cout << "Second call: " << result2 << std::endl;
	std::cout << "Are they equal? " << (result1 == result2 ? "YES" : "NO") << std::endl;
	
	std::cout << MAG << "\n=== Test operator() overload ===" << RESET << std::endl;
	float result3 = noise1(1.5f, 2.3f);
	std::cout << "Using operator(): " << result3 << std::endl;
	std::cout << "operator() matches sample()? " << (result1 == result3 ? "YES" : "NO") << std::endl;
	
	std::cout << MAG << "\n=== Test different coordinates give different results ===" << RESET << std::endl;
	float result4 = noise1.sample(1.6f, 2.3f);
	float result5 = noise1.sample(1.5f, 2.4f);
	std::cout << "\nDifferent coordinates:" << std::endl;
	std::cout << "Coordinates (1.5, 2.3): " << result1 << std::endl;
	std::cout << "Coordinates (1.6, 2.3): " << result4 << std::endl;
	std::cout << "Coordinates (1.5, 2.4): " << result5 << std::endl;
	
	std::cout << MAG << "\n=== Test different seeds give different results ===" << RESET << std::endl;
	PerlinNoise2D noise2(12345);
	float result6 = noise2.sample(1.5f, 2.3f);
	std::cout << "\nSame coordinates (1.5, 2.3) with different seed:" << std::endl;
	std::cout << "Default seed: " << result1 << std::endl;
	std::cout << "Seed 12345: " << result6 << std::endl;
	std::cout << "Are they different? " << (result1 != result6 ? "YES" : "NO") << std::endl;
	
	std::cout << MAG << "\n=== Test value range (Perlin noise should be roughly in [-1, 1]) ===" << RESET << std::endl;
	std::cout << "\nValue range test (should be roughly in [-1, 1]):" << std::endl;
	float min_val = 1.0f, max_val = -1.0f;
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 100; ++j) {
			float val = noise1.sample(i * 0.1f, j * 0.1f);
			min_val = std::min(min_val, val);
			max_val = std::max(max_val, val);
		}
	}
	std::cout << "Min value: " << min_val << std::endl;
	std::cout << "Max value: " << max_val << std::endl;
	std::cout << "Range check: " << (min_val >= -1.5f && max_val <= 1.5f ? "PASS" : "FAIL") << std::endl;
	
	std::cout << MAG << "\n=== Test smoothness (nearby values should be similar) ===" << RESET << std::endl;
	std::cout << "\nSmoothness test (nearby values should be similar):" << std::endl;
	float val1 = noise1.sample(5.0f, 5.0f);
	float val2 = noise1.sample(5.01f, 5.0f);
	float val3 = noise1.sample(5.0f, 5.01f);
	std::cout << "Value at (5.0, 5.0): " << val1 << std::endl;
	std::cout << "Value at (5.01, 5.0): " << val2 << std::endl;
	std::cout << "Value at (5.0, 5.01): " << val3 << std::endl;
	std::cout << "Difference x: " << std::abs(val1 - val2) << std::endl;
	std::cout << "Difference y: " << std::abs(val1 - val3) << std::endl;
	
	std::cout << MAG << "\n=== Test continuity at integer boundaries ===" << RESET << std::endl;
	std::cout << "\nContinuity test at integer boundaries:" << std::endl;
	float boundary1 = noise1.sample(2.999f, 3.0f);
	float boundary2 = noise1.sample(3.001f, 3.0f);
	std::cout << "Value at (2.999, 3.0): " << boundary1 << std::endl;
	std::cout << "Value at (3.001, 3.0): " << boundary2 << std::endl;
	std::cout << "Boundary difference: " << std::abs(boundary1 - boundary2) << std::endl;
	
	std::cout << MAG << "\n=== Show a small visualization of the noise pattern ===" << RESET << std::endl;
	std::cout << "\nSmall visualization (values scaled to 0-9):" << std::endl;
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 16; ++x) {
			float val = noise1.sample(x * 0.2f, y * 0.2f);
			int scaled = static_cast<int>((val + 1.0f) * 4.5f); // Scale from [-1,1] to [0,9]
			scaled = std::max(0, std::min(9, scaled));
			std::cout << scaled;
		}
		std::cout << std::endl;
	}
	
	std::cout << GRN << "PerlinNoise2D tests completed!" << RESET << std::endl;
}

int main(void) {
	std::cout << CYN << "====== MATHEMATICS tests ======" << RESET << std::endl;
	testBasicIVector2Operations();
	testBasicIVector3Operations();
	testRandom2DCoordinateGenerator();
	testPerlinNoise2D();

	std::cout << GRN << "\nAll mathematics tests completed!" << RESET << std::endl;
}