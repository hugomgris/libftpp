/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_2D_coordinate_generator.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:57:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:05:12 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "random_2D_coordinate_generator.hpp"

Random2DCoordinateGenerator::Random2DCoordinateGenerator(long long seed) : _seed(seed) {}

long long Random2DCoordinateGenerator::seed() const {
    return _seed;
}

/*
Hash-based pseudo-random generation using 2D coordinates and seed.
This implementation uses a combination of prime numbers, bit operations,
and mathematical mixing to create pseudo-random values
*/
long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y) const {
    
    
    // Large prime numbers for good distribution
    const long long PRIME1 = 1000000007LL;
    const long long PRIME2 = 1000000009LL;
    const long long PRIME3 = 1000000021LL;
    
    // Mix the coordinates with the seed using prime multiplication
    long long hash = _seed;
    hash ^= (x * PRIME1) + (y * PRIME2);
    hash *= PRIME3;
    
    // Apply bit mixing for better avalanche effect
    hash ^= hash >> 21;
    hash *= PRIME1;
    hash ^= hash >> 17;
    hash *= PRIME2;
    hash ^= hash >> 13;
    
    // Additional mixing to eliminate patterns
    hash += (x << 16) ^ (y << 8) ^ _seed;
    hash *= PRIME3;
    hash ^= hash >> 16;
    
    // Final hash to ensure good distribution across the range
    hash = hash ^ (hash >> 32);
    
    return hash;
}
