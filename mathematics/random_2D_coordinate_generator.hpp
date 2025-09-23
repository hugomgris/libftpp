/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*    random_2D_coordinate_generator.hpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:54:12 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 16:55:35 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
# define RANDOM_2D_COORDINATE_GENERATOR_HPP

class Random2DCoordinateGenerator {
	private:
		long long _seed;
		
	public:
		// Constructor with seed
		Random2DCoordinateGenerator(long long seed);
		
		// Getter for seed
		long long seed() const;
		
		// operator() for generation - generates pseudo-random number from 2D coordinates
		long long operator()(const long long& x, const long long& y) const;
};

#endif