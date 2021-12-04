#include "headers.hpp"

enum class directions {forward, up, down};

using inputs = std::vector<std::pair<directions, int64_t>>;

inputs read_inputs_from_file();
int64_t solve_part_one(const inputs &_entries);
int64_t solve_part_two(const inputs &_entries);

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
	std::fstream inputFile("./resources/Day02_Dive.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string directionText;
		int64_t distance;
		while((inputFile >> directionText >> distance)) {
			directions direction{};
			if(directionText == "forward") direction = directions::forward;
			else if(directionText == "down") direction = directions::down;
			else if(directionText == "up") direction = directions::up;
			input.emplace_back(direction, distance);

		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	int64_t horizontal{};
	int64_t vertical{};

	for(auto &[direction, distance]: input) {
		if(direction == directions::forward) horizontal += distance;
		else if(direction == directions::down) vertical += distance;
		else vertical -= distance;
	}

	return horizontal * vertical;
}

int64_t solve_part_two(const inputs &input) {
	int64_t aim{};
	int64_t horizontal{};
	int64_t vertical{};

	for(auto &[direction, distance]: input) {
		if(direction == directions::forward) {
			horizontal += distance;
			vertical += distance * aim;

		} else if(direction == directions::down) aim += distance;
		else aim -= distance;
	}

	return horizontal * vertical;
}