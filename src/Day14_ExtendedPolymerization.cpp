#include "headers.hpp"

struct inputs {
	std::unordered_map<std::pair<char, char>, size_t, hash_pair> polymer;
	std::unordered_map<std::pair<char, char>, char, hash_pair> rules;
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
	std::fstream inputFile("./resources/Day14_ExtendedPolymerization.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, line);
		for(size_t i = 1; i < line.size(); i++) {
			input.polymer[{line[i - 1], line[i]}]++;
		}
		std::getline(inputFile, line);
		while(std::getline(inputFile, line)) {
			input.rules[{line[0], line[1]}] = line[6];
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	auto currentPolymer = input.polymer;
	std::unordered_map<std::pair<char, char>, size_t, hash_pair> nextPolymer;

	for(size_t i = 0; i < 10; i++) {
		for(auto &[pair, count]: currentPolymer) {
			if(count > 0) {
				nextPolymer[{pair.first, input.rules.at(pair)}] += count;
				nextPolymer[{input.rules.at(pair), pair.second}] += count;
				count = 0;
			}
		}
		std::swap(currentPolymer, nextPolymer);
	}

	std::unordered_map<char, size_t> elementCount;
	for(auto [pair, count]: currentPolymer) {
		elementCount[pair.first] += count;
		elementCount[pair.second] += count;
	}

	auto [leastCommon, mostCommon] = std::minmax_element(
		elementCount.begin(),
		elementCount.end(),
		[](auto a, auto b) {
			return a.second < b.second;
		}
	);
	
	return (mostCommon->second - leastCommon->second) / 2;
}

int64_t solve_part_two(const inputs &input) {
	auto currentPolymer = input.polymer;
	std::unordered_map<std::pair<char, char>, size_t, hash_pair> nextPolymer;

	for(size_t i = 0; i < 40; i++) {
		for(auto &[pair, count]: currentPolymer) {
			if(count > 0) {
				nextPolymer[{pair.first, input.rules.at(pair)}] += count;
				nextPolymer[{input.rules.at(pair), pair.second}] += count;
				count = 0;
			}
		}
		std::swap(currentPolymer, nextPolymer);
	}

	std::unordered_map<char, size_t> elementCount;
	for(auto [pair, count]: currentPolymer) {
		elementCount[pair.first] += count;
		elementCount[pair.second] += count;
	}

	auto [leastCommon, mostCommon] = std::minmax_element(
		elementCount.begin(),
		elementCount.end(),
		[](auto a, auto b) {
			return a.second < b.second;
		}
	);

	return (mostCommon->second - leastCommon->second) / 2;
}