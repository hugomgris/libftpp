/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_buffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:58:07 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/16 15:12:19 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_BUFFER_HPP
# define DATA_BUFFER_HPP

# include <vector>
# include <cstddef>
# include <memory>
# include <typeinfo>
# include <cstring>
# include <stdexcept>

class DataBuffer {
	private:
		std::vector<std::unique_ptr<std::byte[]>> data_chunks;
		std::vector<size_t> sizes;
		std::vector<const std::type_info*> types;
		size_t read_position;

	public:
		DataBuffer() : read_position(0) {}
	
		// Copy constructor and assignment operator overload (needed for some Design Patterns)
		DataBuffer(const DataBuffer &other): read_position(0) {
			*this = other;
		}

		DataBuffer &operator=(const DataBuffer &other) {
			if (this != &other) {
				data_chunks.clear();
				sizes.clear();
				types.clear();
				read_position = 0;

				if (other.data_chunks.size() > 0) {
					sizes = other.sizes;
					types = other.types;
					read_position = other.read_position;
					data_chunks.reserve(other.data_chunks.size());
					
					for (size_t i = 0; i < other.data_chunks.size(); ++i) {
						auto new_chunk = std::make_unique<std::byte[]>(sizes[i]);
						std::memcpy(new_chunk.get(), other.data_chunks[i].get(), sizes[i]);
						data_chunks.push_back(std::move(new_chunk));
					}
				}
			}

			return (*this);
		}
	
		// Serialization
		template<typename T>
		DataBuffer &operator<<(const T &obj) {
			sizes.push_back(sizeof(T));
			types.push_back(&typeid(T));
			data_chunks.push_back(std::make_unique<std::byte[]>(sizeof(T)));
			std::memcpy(data_chunks.back().get(), &obj, sizeof(T));
			
			return (*this);
		}

		// Deserialization
		template<typename T>
		DataBuffer &operator>>(T& obj) {
			if (read_position >= data_chunks.size()) {
				throw std::out_of_range("No more data to read");
			}
			if (typeid(T) != *types[read_position]) {
				throw std::invalid_argument("Type mismatch");
			}
			std::memcpy(&obj, data_chunks[read_position].get(), sizes[read_position]);
			read_position++;

			return (*this);
		}
};

#endif