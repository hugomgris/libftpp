/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_noise_2D.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:10:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:08:18 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PERLIN_NOISE_2D_HPP
# define PERLIN_NOISE_2D_HPP

#include <vector>

class PerlinNoise2D {
private:
    std::vector<int> _permutation;  // Permutation table for Perlin noise
    
    // Private helper methods for Perlin noise implementation
    int _hash(int x, int y) const;
    float _fade(float t) const;
    float _lerp(float a, float b, float t) const;
    float _grad(int hash, float x, float y) const;
    
public:
    PerlinNoise2D(unsigned int seed = 0);
    
    ~PerlinNoise2D() = default;
    
    float sample(float x, float y) const;
    
    float operator()(float x, float y) const;
};

#endif
