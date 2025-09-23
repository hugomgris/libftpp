/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_2D.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:10:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:09:07 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_noise_2D.hpp"
#include <cmath>
#include <algorithm>
#include <random>

PerlinNoise2D::PerlinNoise2D(unsigned int seed) {
    _permutation.resize(512);
    
    for (int i = 0; i < 256; ++i) {
        _permutation[i] = i;
    }
    
    // Shuffle the permutation table using the seed
    std::mt19937 rng(seed);
    std::shuffle(_permutation.begin(), _permutation.begin() + 256, rng);
    
    for (int i = 0; i < 256; ++i) {
        _permutation[256 + i] = _permutation[i];
    }
}

int PerlinNoise2D::_hash(int x, int y) const {
    return _permutation[(_permutation[x & 255] + y) & 255];
}

float PerlinNoise2D::_fade(float t) const {
    // Smooth interpolation function: 6t^5 - 15t^4 + 10t^3
    // This provides smooth derivatives at t=0 and t=1
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise2D::_lerp(float a, float b, float t) const {
    return a + t * (b - a);
}

float PerlinNoise2D::_grad(int hash, float x, float y) const {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise2D::sample(float x, float y) const {
    // Find the integer coordinates of the four corners of the unit square
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    
    // Find the fractional parts of x and y
    x -= std::floor(x);
    y -= std::floor(y);
    
    // Compute fade curves for x and y
    float u = _fade(x);
    float v = _fade(y);
    
    // Hash coordinates of the 4 corners of the unit square
    int aa = _hash(X,     Y    );
    int ab = _hash(X,     Y + 1);
    int ba = _hash(X + 1, Y    );
    int bb = _hash(X + 1, Y + 1);
    
    // Calculate gradients at the 4 corners
    float grad_aa = _grad(aa, x,     y    );
    float grad_ba = _grad(ba, x - 1, y    );
    float grad_ab = _grad(ab, x,     y - 1);
    float grad_bb = _grad(bb, x - 1, y - 1);
    
    // Interpolate between the gradients
    float lerp1 = _lerp(grad_aa, grad_ba, u);
    float lerp2 = _lerp(grad_ab, grad_bb, u);
    float result = _lerp(lerp1, lerp2, v);
    
    // Return result in range [-1, 1]
    return result;
}

float PerlinNoise2D::operator()(float x, float y) const {
    return sample(x, y);
}
