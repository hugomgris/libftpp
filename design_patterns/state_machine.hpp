/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 10:16:28 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/17 10:53:04 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_MACHINE_HPP
# define STATE_MACHINE_HPP

# include <set>
# include <map>
# include <functional>
# include <stdexcept>

class StateMachineException : public std::exception {
	private:
		std::string message;
	public:
		StateMachineException(const std::string &msg) : message(msg) {}
		const char *what () const noexcept override {
			return message.c_str();
		}
};

template<typename TState>
class StateMachine {
	private:
		std::set<TState> validStates;
		std::map<std::pair<TState, TState>, std::function<void()>> transitions;
		std::map<TState, std::function<void()>> actions;

		TState currentState;
		bool hasCurrentState;

	public:
		StateMachine(): hasCurrentState(false) {}

		const TState &getCurrentState() const {
			if (!hasCurrentState) {
				throw StateMachineException("No current state set");
			}
			return (currentState);
		}
		
		void addState(const TState &state){
			validStates.insert(state);

			if (!hasCurrentState) {
				currentState = state;
				hasCurrentState = true;
			}
		}

		void addTransition(const TState &startState, const TState &finalState,
						const std::function<void()> &lambda) {
			if (validStates.find(startState) == validStates.end()) {
				throw StateMachineException("Start state not registered");
			}
			if (validStates.find(finalState) == validStates.end()) {
				throw StateMachineException("Final state not registered");
			}

			transitions[std::make_pair(startState, finalState)] = lambda;
		}

		void addAction(const TState &state, const std::function<void()> &lambda) {
			if (validStates.find(state) == validStates.end()) {
				throw StateMachineException("State not registered");
			}

			actions[state] = lambda;
		}

		void transitionTo(const TState &state) {
			if (!hasCurrentState) {
				throw StateMachineException("No current state set");
			}

			if (validStates.find(state) == validStates.end()) {
				throw StateMachineException("Target state not registered");
			}

			auto transitionKey = std::make_pair(currentState, state);
			auto it = transitions.find(transitionKey);

			if (it == transitions.end()) {
				throw StateMachineException("No transition defined from current state to target state");
			}

			it->second();

			currentState = state;
		}

		void update() {
			if (!hasCurrentState) {
				throw StateMachineException("No current state set");
			}

			auto it = actions.find(currentState);
			if (it == actions.end()) {
				throw StateMachineException("No action defined for current state");
			}

			it->second();
		}
};

#endif