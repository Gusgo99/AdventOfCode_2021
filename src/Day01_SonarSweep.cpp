#include "headers.hpp"

using inputs = std::vector<int64_t>;

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
	std::fstream inputFile("./resources/Day01_SonarSweep.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		int64_t temp{};
		while(inputFile >> temp) {
			input.push_back(temp);

		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	int64_t solution{};

	for(size_t i = 1; i < input.size(); i++) {
		solution += int{input[i] > input[i - 1]};
	}

	return solution;
}

int64_t solve_part_two(const inputs &input) {
	int64_t solution{};
	
	for(size_t i = 3; i < input.size(); i++) {
		solution += int{input[i] > input[i - 3]};
	}

	return solution;
}