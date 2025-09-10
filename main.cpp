/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:54:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/10 15:13:42 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftpp.hpp"

int main (void) {
	IntPool pool;
	pool.resize(3);

	try {
		auto obj1 = pool.acquire(42);
		auto obj2 = pool.acquire(100);
		std::cout << "obj1: " << *obj1 << ", obj2: " << *obj2 << std::endl;
		auto obj3 = pool.acquire(200);
		std::cout << "obj3: " << *obj3 << std::endl;
		auto obj4 = pool.acquire(1);
		std::cout << "obj4: " << *obj4 << std::endl;
	} catch (const std::runtime_error &e) {
		std::cout << "Caught exception: " << e.what() << std::endl;
	}

	auto obj3 = pool.acquire(200);
	std::cout << "obj3: " << *obj3 << std::endl;

	return (0);
}