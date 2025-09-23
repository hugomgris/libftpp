/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:10:37 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 11:40:23 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "network.hpp"
#include "../colors.h"

void testMessage() {
	std::cout << YEL << "\n=== Testing message ===" << RESET << std::endl;

	Message msg(42);

	msg << int(123) << double(3.14159) << bool(true) << std::string("Hello Network!");

	std::cout << "Message type: " << msg.type() << std::endl;
	std::cout << "Message size: " << msg.getDataSize() << " bytes" << std::endl;

	int i; double d; bool b; std::string s;
	msg >> i >> d >> b >> s;

	std::cout << "Read: " << i << ", " << d << ", " << b << ", " << s << std::endl;
	
	auto serialized = msg.serialize();
	std::cout << "Serialized size: " << serialized.size() << " bytes" << std::endl;
	
	try {
		Message restored = Message::deserialize(serialized);
		std::cout << "Deserialized message type: " << restored.type() << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Deserialization error: " << e.what() << std::endl;
	}
}

int main(void) {
	std::cout << CYN << "====== NETWORK tests ======" << RESET << std::endl;

	testMessage();

	std::cout << GRN << "\nAll tests completed successfully!" << std::endl;
}