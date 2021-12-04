#include "headers.hpp"

using inputs = std::vector<std::bitset<12>>;

inputs read_inputs_from_file();
int64_t solve_part_one(const inputs &_entries);
int64_t solve_part_two(inputs _entries);

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
	std::fstream inputFile("./resources/Day03_BinaryDiagnostic.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::bitset<12> value{};
		while(inputFile >> value) {
			input.emplace_back(value);

		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	std::array<size_t, 12> onesPresent{};
	
	for(const auto i: input) {
		for(size_t j = 0; j < i.size(); j++) {
			onesPresent[j] += size_t{i[j]};
		}
	}

	std::bitset<12> gammaRate;

	for(size_t i = 0; i < gammaRate.size(); i++) {
		gammaRate[i] = onesPresent[i] > 500;
	}

	std::bitset<12> epsilonRate = ~gammaRate;

	return gammaRate.to_ulong() * epsilonRate.to_ulong();
}

int64_t solve_part_two(inputs input) {
	auto copy = input;

	for(auto invertCondition: {false, true}) {
		size_t bit{11};
		while(input.size() > 1) {
			size_t onesCount{};
			for(const auto i: input) {
				onesCount += size_t{i[bit]};
			}

			std::vector<std::bitset<12>>::iterator newEnd;
			if(invertCondition ^ (2 * onesCount >= input.size())) {
				newEnd = std::remove_if(
					input.begin(),
					input.end(),
					[=](auto val) { return !val[bit]; }
				);

			} else {
				newEnd = std::remove_if(
					input.begin(),
					input.end(),
					[=](auto val) { return val[bit]; }
				);

			}
			input.resize(newEnd - input.begin());

			bit--;
		}

		std::swap(input, copy);

	}

	return input[0].to_ulong() * copy[0].to_ulong();
}