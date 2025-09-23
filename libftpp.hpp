/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftpp.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:54:33 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 17:36:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPP_HPP
# define LIBFTPP_HPP

/**
 * @file libftpp.hpp
 * @brief Main header for the libftpp C++ library (42 School project)
 * 
 * libftpp is a comprehensive C++ library providing:
 * 
 * Core Modules:
 * - Data Structures: Advanced containers and data management
 * - Design Patterns: Implementation of common design patterns
 * - IOStream: Thread-safe input/output operations
 * - Threading: Thread management and synchronization utilities
 * - Network: TCP client/server networking with message handling
 * - Mathematics: Vector operations, random generation, and Perlin noise
 * 
 * Bonus Features (see bonus/bonus.hpp):
 * - Timer: Duration tracking and timeout functionality
 * - Chronometer: Precise time measurement with lap support
 * - Application & Widget: Framework for interactive applications
 * - ObservableValue: Value change notification system
 * - Logger: Comprehensive logging with multiple outputs and levels
 * 
 * Build Instructions:
 * - make all: Build the complete library
 * - make test_unit_X: Build and run specific module tests
 * - make test_bonus: Build and run bonus feature tests
 * 
 * @author hmunoz-g
 * @version 1.0
 * @date 2025-09-23
 */

# include <iostream>

// Core module includes
# include "./data_structures/data_structures.hpp"
# include "./design_patterns/design_patterns.hpp"
# include "./IOStream/thread_safe_iostream.hpp"
# include "./threading/threading.hpp"
# include "./network/network.hpp"
# include "./mathematics/mathematics.hpp"

// Note: Bonus features are available separately via:
// #include "./bonus/bonus.hpp"

#endif