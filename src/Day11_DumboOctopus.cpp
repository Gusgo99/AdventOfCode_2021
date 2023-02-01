#include "headers.hpp"

using inputs = std::string;

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
	std::fstream inputFile("./resources/Day11_DumboOctopus.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			input += line;
		}
	}

	return input;
}

std::array<size_t, 8> get_neighbours(const size_t pos, const size_t width, const size_t height) {
	std::array<size_t, 8> neighbours{
		SIZE_MAX,
		SIZE_MAX,
		SIZE_MAX,
		SIZE_MAX,
		SIZE_MAX,
		SIZE_MAX,
		SIZE_MAX,
		SIZE_MAX
	};

	if(pos >= width) neighbours[0] = pos - width;

	if(pos < (width * (height - 1))) neighbours[1] = pos + width;

	if((pos % width) > 0) {
		if(pos > width) neighbours[2] = pos - width - 1;
		neighbours[3] = pos - 1;
		if(pos < (width * (height - 1))) neighbours[4] = pos + width - 1;

	}

	if((pos % width) < (width - 1)) {
		if(pos > width) neighbours[5] = pos - width + 1;
		neighbours[6] = pos + 1;
		if(pos < (width * (height - 1))) neighbours[7] = pos + width + 1;

	}

	return neighbours;
}

int64_t solve_part_one(const inputs &input) {
	int64_t flashCount{};
	constexpr char flashIndicator = '9' + 1;

	auto currentState = input;

	for(int count = 0; count < 100; count++) {
		std::queue<size_t> flashLocations;
		std::array<bool, 100> flashed{};
		for(size_t i = 0; i < currentState.size(); i++) {
			currentState[i]++;
			if(currentState[i] > '9') {
				flashLocations.push(i);
				flashed[i] = true;
				flashCount++;

			}
		}

		while(!flashLocations.empty()) {
			auto neighbours = get_neighbours(flashLocations.front(), 10, 10);
			flashLocations.pop();
			for(auto neighbour: neighbours) {
				if(neighbour < currentState.size()) {
					currentState[neighbour]++;
					if((currentState[neighbour] > '9') && !flashed[neighbour]) {
						flashed[neighbour] = true;
						flashCount++;
						flashLocations.push(neighbour);
					}
				}
			}
		}
		for(auto &i: currentState) {
			if(i > '9') i = '0';
		}
	}

	return flashCount;
}

int64_t solve_part_two(const inputs &input) {
	int64_t currentStep{};
	int64_t flashCount{};

	auto currentState = input;

	while(flashCount != 100)  {
		currentStep++;
		flashCount = 0;

		std::queue<size_t> flashLocations;
		std::array<bool, 100> flashed{};
		for(size_t i = 0; i < currentState.size(); i++) {
			currentState[i]++;
			if(currentState[i] > '9') {
				flashLocations.push(i);
				flashed[i] = true;
				flashCount++;

			}
		}

		while(!flashLocations.empty()) {
			auto neighbours = get_neighbours(flashLocations.front(), 10, 10);
			flashLocations.pop();
			for(auto neighbour: neighbours) {
				if(neighbour < currentState.size()) {
					currentState[neighbour]++;
					if((currentState[neighbour] > '9') && !flashed[neighbour]) {
						flashed[neighbour] = true;
						flashCount++;
						flashLocations.push(neighbour);
					}
				}
			}
		}
		for(auto &i: currentState) {
			if(i > '9') i = '0';
		}
	}

	return currentStep;
}