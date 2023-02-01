#include "headers.hpp"

template<size_t S>
using map = std::array<std::array<char, S>, 11>;

using inputs = map<3>;

inputs read_inputs_from_file();
int64_t solve_part_one(const inputs &input);
int64_t solve_part_two(const inputs &input);

int main() {
	const inputs input = read_inputs_from_file();

	auto start = std::chrono::high_resolution_clock::now();
	auto solutionPart1 = solve_part_one(input);
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1 solution: " << solutionPart1 << std::endl;
	std::cout << "Part 1 time: " << std::chrono::duration<double>(end - start).count() << std::endl;

	start = std::chrono::high_resolution_clock::now();
	auto solutionPart2 = solve_part_two(input);
	end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 2 solution: " << solutionPart2 << std::endl;
	std::cout << "Part 2 time: " << std::chrono::duration<double>(end - start).count() << std::endl;

	return 0;
}

inputs read_inputs_from_file() {
	std::fstream inputFile("./resources/Day23_Amphipod.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, line);
		std::getline(inputFile, line);
		std::getline(inputFile, line);
		input[2][1] = line[3];
		input[4][1] = line[5];
		input[6][1] = line[7];
		input[8][1] = line[9];
		std::getline(inputFile, line);
		input[2][2] = line[3];
		input[4][2] = line[5];
		input[6][2] = line[7];
		input[8][2] = line[9];

	}

	return input;
}

std::map<char, int64_t> energyPerStep{{'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};
std::map<char, int> letterToHouse{{'A', 2}, {'B', 4}, {'C', 6}, {'D', 8}, {'\0', INT_MAX}};

template<size_t S>
int64_t do_obligatory_moves(map<S> &state) {
	bool moved = true;
	int64_t energyCost{};

	while(moved) {
		moved = false;
		for(size_t i = 0; i < state.size(); i++) {
			if(state[i][0] != '\0') {
				bool shouldMove = true;
				const int house = letterToHouse[state[i][0]];

				for(size_t j = 1; j < S; j++) {
					shouldMove &= (state[house][j] == '\0') || (state[house][j] == state[i][0]);
				}

				for(int j = i + 1; j < house; j++) {
					shouldMove &= state[j][0] == '\0';
				}
				for(int j = int(i) - 1; j > house; j--) {
					shouldMove &= state[j][0] == '\0';
				}

				if(shouldMove) {
					int64_t dist = (house > i) ? (house - i) : (i - house);
					for(size_t j = S - 1; j > 0; j--) {
						if(state[house][j] == '\0') {
							dist += j;
							dist *= energyPerStep[state[i][0]];
							state[house][j] = state[i][0];
							state[i][0] = '\0';
							break;
						}
					}
					energyCost += dist;
					moved = true;
				}
			}
		}
	}

	return energyCost;
}

template<size_t S>
bool is_solved(const map<S> &state) {
	for(size_t i = 0; i < state.size(); i++) {
		for(size_t j = 0; j < state[i].size(); j++) {
			if(state[i][j] != '\0') {
				if(letterToHouse[state[i][j]] != i) return false;
			}
		}
	}

	return true;
}

template<size_t S>
bool is_movable(const map<S> &state, const size_t i, const size_t j) {
	if(state[i][j] == '\0') return false;
	for(size_t k = 1; k < j; k++) {
		if(state[i][k] != '\0') return false;
	}
	if(letterToHouse[state[i][j]] == i) {
		bool areInRightHouse = true;
		for(size_t k = j; k < S; k++) {
			areInRightHouse &= state[i][j] == state[i][k];
		}
		if(areInRightHouse) return false;
	}

	return true;
}

std::vector<std::array<size_t, 4>> get_possible_moves(const map<3> &state) {
	std::vector<std::array<size_t, 4>> possibleMoves;
	possibleMoves.reserve(16);

	for(auto house : {2, 4, 6, 8}) {
		for(auto room : {1, 2}) {
			if(is_movable(state, house, room)) {
				for(int i = house - 1; i >= -1; i -= 2) {
					auto position = std::max(0, i);
					if(state[position][0] != '\0') break;
					possibleMoves.push_back({});
					possibleMoves.back()[0] = house;
					possibleMoves.back()[1] = room;
					possibleMoves.back()[2] = position;
					possibleMoves.back()[3] = 0;

				}
				for(int i = house + 1; i <= 11; i += 2) {
					auto position = std::min(i, 10);
					if(state[position][0] != '\0') break;
					possibleMoves.push_back({});
					possibleMoves.back()[0] = house;
					possibleMoves.back()[1] = room;
					possibleMoves.back()[2] = position;
					possibleMoves.back()[3] = 0;
				}
			}
		}
	}

	return possibleMoves;
}

template<size_t S>
map<S> execute_move(map<S> state, std::array<size_t, 4> &move) {
	state[move[2]][move[3]] = state[move[0]][move[1]];
	state[move[0]][move[1]] = '\0';

	return state;
}

template<size_t S>
int64_t get_move_energy(const map<S> &state, const std::array<size_t, 4> &move) {
	auto rowDistance = std::max(move[0], move[2]) - std::min(move[0], move[2]);
	auto columnDistance = std::max(move[1], move[3]) - std::min(move[1], move[3]);

	return (rowDistance + columnDistance) * energyPerStep[state[move[0]][move[1]]];
}

template<size_t S>
void print_state(const map<S> &state) {
	for(size_t j = 0; j < state[0].size(); j++) {
		for(size_t i = 0; i < state.size(); i++) {
			if(j != 0 && (i % 2)) std::cout << '#';
			else std::cout << (state[i][j] != 0 ? state[i][j] : ' ');
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

int64_t solve_part_one(const inputs &input) {
	std::stack<std::tuple<map<3>, int64_t>> states;
	int64_t minEnergy = INT64_MAX;

	states.emplace(input, 0);

	while(!states.empty()) {
		auto [currentState, currentEnergy] = states.top();
		states.pop();

		if(is_solved(currentState)) {
			minEnergy = std::min(minEnergy, currentEnergy);
			continue;
		}

		auto possibleMoves = get_possible_moves(currentState);
		for(auto move : possibleMoves) {
			auto newEnergy = currentEnergy + get_move_energy(currentState, move);
			auto newState = execute_move(currentState, move);
			newEnergy += do_obligatory_moves(newState);
			states.emplace(newState, newEnergy);

		}
	}
	
	return minEnergy;
}

std::vector<std::array<size_t, 4>> get_possible_moves(const map<5> &state) {
	std::vector<std::array<size_t, 4>> possibleMoves;
	possibleMoves.reserve(16);

	for(auto house : {2, 4, 6, 8}) {
		for(auto room : {1, 2, 3, 4}) {
			if(is_movable(state, house, room)) {
				for(int i = house - 1; i >= -1; i -= 2) {
					auto position = std::max(0, i);
					if(state[position][0] != '\0') break;
					possibleMoves.push_back({});
					possibleMoves.back()[0] = house;
					possibleMoves.back()[1] = room;
					possibleMoves.back()[2] = position;
					possibleMoves.back()[3] = 0;

				}
				for(int i = house + 1; i <= 11; i += 2) {
					auto position = std::min(i, 10);
					if(state[position][0] != '\0') break;
					possibleMoves.push_back({});
					possibleMoves.back()[0] = house;
					possibleMoves.back()[1] = room;
					possibleMoves.back()[2] = position;
					possibleMoves.back()[3] = 0;
				}
			}
		}
	}

	return possibleMoves;
}

int64_t solve_part_two(const inputs &input) {
	std::stack<std::tuple<map<5>, int64_t>> states;
	int64_t minEnergy = INT64_MAX;

	map<5> expandedStartingPositions{};
	for(size_t i = 0; i < 11; i++) {
		expandedStartingPositions[i][1] = input[i][1];
		expandedStartingPositions[i][4] = input[i][2];
	}

	expandedStartingPositions[2][2] = 'D';
	expandedStartingPositions[2][3] = 'D';
	expandedStartingPositions[4][2] = 'C';
	expandedStartingPositions[4][3] = 'B';
	expandedStartingPositions[6][2] = 'B';
	expandedStartingPositions[6][3] = 'A';
	expandedStartingPositions[8][2] = 'A';
	expandedStartingPositions[8][3] = 'C';

	states.emplace(expandedStartingPositions, 0);

	while(!states.empty()) {
		auto [currentState, currentEnergy] = states.top();
		states.pop();

		if(is_solved(currentState)) {
			minEnergy = std::min(minEnergy, currentEnergy);
			continue;
		}

		auto possibleMoves = get_possible_moves(currentState);
		for(auto move : possibleMoves) {
			auto newEnergy = currentEnergy + get_move_energy(currentState, move);
			auto newState = execute_move(currentState, move);
			newEnergy += do_obligatory_moves(newState);
			states.emplace(newState, newEnergy);
		
		}
	}

	return minEnergy;
}