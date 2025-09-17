/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memento.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:06:50 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/17 11:10:15 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMENTO_HPP
# define MEMENTO_HPP

# include "../data_structures/data_structures.hpp"

class Memento {
	public:
		using Snapshot = DataBuffer;

		// Public interfaces
		Snapshot save() {
			Snapshot snapshot;
			_saveToSnapshot(snapshot);
			return snapshot;
		}

		void load(const Snapshot &state) {
			Snapshot mutable_state = state;
			_loadFromSnapshot(mutable_state);
		}
	
	private:
		// Pure virtualization, as Memento needs to be inherited by other classes
		virtual void _saveToSnapshot(Snapshot& snapshot) const = 0;
		virtual void _loadFromSnapshot(Snapshot &snapshot) = 0;
};

#endif