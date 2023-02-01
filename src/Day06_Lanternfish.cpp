#include "headers.hpp"

using inputs = std::vector<int64_t>;

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
	std::fstream inputFile("./resources/Day06_Lanternfish.txt", std::fstream::in);
	inputs input;

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, line);
		for(auto i: split(line, ",")) {
			input.push_back(std::stoll(i.data()));
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	std::vector<size_t> fishesCount(7);
	for(auto i: input) fishesCount[i]++;

	std::queue<int64_t> newBorns;

	newBorns.push(0);
	newBorns.push(0);

	size_t posFishes = 1;
	for(size_t i = 0; i <= 80; i++) {
		newBorns.push(fishesCount[posFishes]);
		fishesCount[posFishes] += newBorns.front();
		newBorns.pop();
		posFishes = (posFishes + 1) % fishesCount.size();
	}

	int64_t fishCount{};
	for(auto i: fishesCount) fishCount += i;
	while(newBorns.empty()) {
		fishCount += newBorns.front();
		newBorns.pop();
	}
	
	return fishCount;
}

int64_t solve_part_two(const inputs &input) {
	std::vector<size_t> fishesCount(7);
	for(auto i: input) fishesCount[i]++;

	std::queue<int64_t> newBorns;

	newBorns.push(0);
	newBorns.push(0);

	size_t posFishes = 1;
	for(size_t i = 0; i <= 256; i++) {
		newBorns.push(fishesCount[posFishes]);
		fishesCount[posFishes] += newBorns.front();
		newBorns.pop();
		posFishes = (posFishes + 1) % fishesCount.size();
	}

	int64_t fishCount{};
	for(auto i: fishesCount) fishCount += i;
	while(newBorns.empty()) {
		fishCount += newBorns.front();
		newBorns.pop();
	}

	return fishCount;
}