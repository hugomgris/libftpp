/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:38:48 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 11:39:59 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "message.hpp"

Message &Message::operator<<(const std::string &str) {
	uint32_t length = static_cast<uint32_t>(str.length());
	*this << length;
	
	const uint8_t* strData = reinterpret_cast<const uint8_t*>(str.c_str());
	_data.insert(_data.end(), strData, strData + length);
	
	return *this;
}

Message &Message::operator>>(std::string &str) {
	uint32_t length;
	*this >> length;
	
	if (_readPos + length > _data.size()) {
		throw std::runtime_error("Not enough data to read string");
	}
	
	str.assign(reinterpret_cast<const char*>(&_data[_readPos]), length);
	_readPos += length;
	
	return *this;
}

std::vector<uint8_t> Message::serialize() const {
	std::vector<uint8_t> result;
	
	uint32_t networkType = htonl(static_cast<uint32_t>(_messageType));
	const uint8_t* typeBytes = reinterpret_cast<const uint8_t*>(&networkType);
	result.insert(result.end(), typeBytes, typeBytes + sizeof(uint32_t));
	
	uint32_t networkSize = htonl(static_cast<uint32_t>(_data.size()));
	const uint8_t* sizeBytes = reinterpret_cast<const uint8_t*>(&networkSize);
	result.insert(result.end(), sizeBytes, sizeBytes + sizeof(uint32_t));
	
	result.insert(result.end(), _data.begin(), _data.end());
	
	return result;
}

Message Message::deserialize(const std::vector<uint8_t> &networkData) {
	if (networkData.size() < 8) { 
		throw std::runtime_error("Invalid network data: too small");
	}
	
	size_t pos = 0;
	
	uint32_t networkType;
	std::memcpy(&networkType, &networkData[pos], sizeof(uint32_t));
	int messageType = static_cast<int>(ntohl(networkType));
	pos += sizeof(uint32_t);
	
	uint32_t networkSize;
	std::memcpy(&networkSize, &networkData[pos], sizeof(uint32_t));
	uint32_t dataSize = ntohl(networkSize);
	pos += sizeof(uint32_t);
	
	if (pos + dataSize > networkData.size()) {
		throw std::runtime_error("Invalid network data: size mismatch");
	}
	
	Message result(messageType);
	result._data.assign(networkData.begin() + pos, networkData.begin() + pos + dataSize);
	
	return result;
}