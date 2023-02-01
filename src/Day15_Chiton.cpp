#include "headers.hpp"

struct inputs {
	std::vector<int64_t> riskLevel;
	size_t width;
	size_t height;
};

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
	std::fstream inputFile("./resources/Day15_Chiton.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			input.width = line.size();
			input.height++;
			for(auto i: line) {
				auto number = i - '0';
				input.riskLevel.push_back(number);
			}
		}
	}

	return input;
}

std::array<size_t, 4> get_neighbours(const size_t pos, const size_t width, const size_t height) {
	std::array<size_t, 4> neighbours{SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX};

	if(pos >= width) neighbours[0] = pos - width;

	if(pos < (width * (height - 1))) neighbours[1] = pos + width;

	if((pos % width) > 0) neighbours[2] = pos - 1;

	if((pos % width) < (width - 1)) neighbours[3] = pos + 1;

	return neighbours;
}

int64_t solve_part_one(const inputs &input) {
	std::vector<int64_t> accRisk(input.riskLevel.size(), INT64_MAX);
	std::queue<size_t> toVisit;

	accRisk[0] = 0;
	toVisit.push(1);
	toVisit.push(input.width);

	while(!toVisit.empty()) {
		auto currentPos = toVisit.front();
		toVisit.pop();
		const auto neighbours = get_neighbours(currentPos, input.width, input.height);
		int64_t minRisk = INT64_MAX;
		for(auto i: neighbours) {
			if(i < accRisk.size()) {
				if(accRisk[i] < minRisk) minRisk = accRisk[i];
			}
		}
		accRisk[currentPos] = minRisk + input.riskLevel[currentPos];
		for(auto i: neighbours) {
			if(i < accRisk.size()) {
				if((accRisk[currentPos] + input.riskLevel[i]) < accRisk[i]) {
					accRisk[i] = accRisk[currentPos] + input.riskLevel[i];
					toVisit.push(i);

				}
			}
		}
	}

	return accRisk.back();
}

int64_t solve_part_two(const inputs &input) {
	std::vector<int64_t> actualRiskLevel(5 * 5 * input.riskLevel.size());

	size_t index{};
	for(size_t iRepeat = 0; iRepeat < 5; iRepeat++) {
		for(size_t i = 0; i < input.height; i++) {
			for(size_t jRepeat = 0; jRepeat < 5; jRepeat++) {
				for(size_t j = 0; j < input.width; j++) {
					auto newRisk = input.riskLevel[i * input.width + j];
					newRisk += iRepeat + jRepeat;
					if(newRisk > 9) newRisk -= 9;
					actualRiskLevel[index] = newRisk;
					index++;
				}
			}
		}
	}

	std::vector<int64_t> accRisk(actualRiskLevel.size(), INT64_MAX);
	std::queue<size_t> toVisit;

	accRisk[0] = 0;
	toVisit.push(1);
	toVisit.push(5 * input.width);

	while(!toVisit.empty()) {
		auto currentPos = toVisit.front();
		toVisit.pop();
		const auto neighbours = get_neighbours(currentPos, 5 * input.width, 5 * input.height);
		int64_t minRisk = INT64_MAX;
		for(auto i: neighbours) {
			if(i < accRisk.size()) {
				if(accRisk[i] < minRisk) minRisk = accRisk[i];
			}
		}
		accRisk[currentPos] = minRisk + actualRiskLevel[currentPos];
		for(auto i: neighbours) {
			if(i < accRisk.size()) {
				if((accRisk[currentPos] + actualRiskLevel[i]) < accRisk[i]) {
					accRisk[i] = accRisk[currentPos] + actualRiskLevel[i];
					toVisit.push(i);

				}
			}
		}
	}

	return accRisk.back();
}