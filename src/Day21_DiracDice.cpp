#include "headers.hpp"

using inputs = std::pair<int64_t, int64_t>;

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
	std::fstream inputFile("./resources/Day21_DiracDice.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, line);
		line = line.substr(line.find(":") + 1);
		input.first = std::stoll(line);
		std::getline(inputFile, line);
		line = line.substr(line.find(":") + 1);
		input.second = std::stoll(line);
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	int64_t diceRolls{};
	int64_t scoreP1{};
	int64_t scoreP2{};
	int64_t p1 = input.first - 1;
	int64_t p2 = input.second - 1;
	int64_t i{};
	bool player1Turn{};
	while((scoreP1 < 1000) && (scoreP2 < 1000)) {
		if(player1Turn) {
			std::array<int64_t, 3> rolls{};
			rolls[0] = i % 100;
			rolls[1] = (i + 1) % 100;
			rolls[2] = (i + 2) % 100;
			p2 += rolls[0] + rolls[1] + rolls[2] + 3;
			p2 %= 10;
			scoreP2 += p2 + 1;
			diceRolls += 3;
			i = (i + 3) % 100;
		} else {
			std::array<int64_t, 3> rolls{};
			p1 += i % 100;
			p1 += (i + 1) % 100;
			p1 += (i + 2) % 100;
			p1 += 3;
			p1 %= 10;
			scoreP1 += p1 + 1;
			diceRolls += 3;
			i = (i + 3) % 100;
		}
		player1Turn = !player1Turn;
	}
	
	return std::min(scoreP1, scoreP2) * diceRolls;
}

struct hash_tuple {
	size_t operator()(const std::tuple<int64_t, int64_t, int64_t, int64_t, bool, int64_t> &p) const {
		auto hash0 = std::hash<int64_t>{}(std::get<0>(p));
		auto hash1 = std::hash<int64_t>{}(std::get<1>(p));
		auto hash2 = std::hash<int64_t>{}(std::get<2>(p));
		auto hash3 = std::hash<int64_t>{}(std::get<3>(p));
		auto hash4 = std::hash<bool>{}(std::get<4>(p));
		auto hash5 = std::hash<int64_t>{}(std::get<5>(p));
		hash0 ^= hash1 + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2);
		hash2 ^= hash3 + 0x9e3779b9 + (hash2 << 6) + (hash2 >> 2);
		hash4 ^= hash5 + 0x9e3779b9 + (hash4 << 6) + (hash4 >> 2);

		hash0 ^= hash2 + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2);

		hash0 ^= hash4 + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2);

		return hash0;
	}
};

int64_t solve_part_two(const inputs &input) {
	using state = std::tuple<int64_t, int64_t, int64_t, int64_t, bool, int64_t>;
	std::stack<state> states;
	std::unordered_map<state, std::pair<int64_t, int64_t>, hash_tuple> winsByState;

	states.emplace(0, 0, input.first - 1, input.second - 1, true, 1);

	int64_t winsP1{};
	int64_t winsP2{};

	while(!states.empty()) {
		auto currentState = states.top();
		auto &[scoreP1, scoreP2, posP1, posP2, p1Turn, copies] = currentState;
		states.pop();

		if((scoreP1 < 21) && (scoreP2 < 21)) {
			auto scoreForCurrentPlayer = p1Turn ? scoreP1 : scoreP2;
			auto posForCurrentPlayer = p1Turn ? posP1 : posP2;
			std::array<std::pair<int64_t, int64_t>, 7> possibleResults{{
				{3, 1},
				{4, 3},
				{5, 6},
				{6, 7},
				{7, 6},
				{8, 3},
				{9, 1}
			}};

			bool foundAllPossible = true;
			std::pair<int64_t, int64_t> acc{};
			for(auto [sum, count]: possibleResults) {
				auto newPos = (posForCurrentPlayer + sum) % 10;
				auto newScore = scoreForCurrentPlayer + newPos + 1;
				state nextState{};
				if(p1Turn) nextState = state{newScore, scoreP2, newPos, posP2, !p1Turn, copies * count};
				else nextState = state{scoreP1, newScore, posP1, newPos, !p1Turn, copies * count};
				if(winsByState.find(nextState) != winsByState.end()) {
					auto nextStateCount = winsByState[nextState];
					winsP1 += nextStateCount.first;
					winsP2 += nextStateCount.second;
					acc.first += nextStateCount.first;
					acc.second += nextStateCount.second;
				} else {
					foundAllPossible = false;
					states.push(nextState);
				}
			}
			if(foundAllPossible) winsByState[currentState] = acc;
		}
		else {
			if(scoreP1 > scoreP2) {
				winsP1 += copies;
				winsByState[currentState].first = copies;
			} else {
				winsP2 += copies;
				winsByState[currentState].second = copies;
			}
		}
	}

	return std::max(winsP1, winsP2);
}