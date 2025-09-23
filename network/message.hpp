#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <vector>
# include <string>
# include <cstdint>
# include <stdexcept>
# include <cstddef>
# include <cstring>
# include <arpa/inet.h>

class Message {
private:
	int _messageType;
	std::vector<uint8_t> _data;
	mutable size_t _readPos;

	// Endianness conversion helpers
	template<typename T>
	T hostToNetwork(const T &value) const {
		return value;
	}

	template<typename T>
	T networkToHost(const T &value) const { 
		return value;
	}

public:
	explicit Message(int type): _messageType(type), _readPos(0) {}
	
	int type() const { return _messageType; }

	template<typename T>
	Message &operator<<(const T &value) {
		T networkValue = hostToNetwork(value);
		const uint8_t *bytes = reinterpret_cast<const uint8_t*>(&networkValue);
		_data.insert(_data.end(), bytes, bytes + sizeof(T));
		return *this;
	}

	template<typename T>
	Message &operator>>(T &value) {
		if (_readPos + sizeof(T) > _data.size()) {
			throw std::runtime_error("Message read past end");
		}

		T networkValue;
		std::memcpy(&networkValue, &_data[_readPos], sizeof(T));
		value = networkToHost(networkValue);
		_readPos += sizeof(T);
		return *this;
	}

	// String operations
	Message &operator<<(const std::string &str);
	Message &operator>>(std::string &str);

	// Raw data access for networking
	const uint8_t *getData() const { return _data.data(); }
	size_t getDataSize() const { return _data.size(); }

	// Utility methods
	void clear() { _data.clear(); _readPos = 0; }
	void resetReadPos() { _readPos = 0; }

	// Serialization for network transmission
	std::vector<uint8_t> serialize() const;
	static Message deserialize(const std::vector<uint8_t> &networkData);
};

// Template specializations for endianness
template<>
inline uint32_t Message::hostToNetwork<uint32_t>(const uint32_t &value) const { 
	return htonl(value); 
}

template<>
inline uint32_t Message::networkToHost<uint32_t>(const uint32_t &value) const { 
	return ntohl(value); 
}

template<>
inline uint16_t Message::hostToNetwork<uint16_t>(const uint16_t &value) const { 
	return htons(value); 
}

template<>
inline uint16_t Message::networkToHost<uint16_t>(const uint16_t &value) const { 
	return ntohs(value); 
}

template<>
inline int32_t Message::hostToNetwork<int32_t>(const int32_t &value) const { 
	return static_cast<int32_t>(htonl(static_cast<uint32_t>(value))); 
}

template<>
inline int32_t Message::networkToHost<int32_t>(const int32_t &value) const { 
	return static_cast<int32_t>(ntohl(static_cast<uint32_t>(value))); 
}

#endif