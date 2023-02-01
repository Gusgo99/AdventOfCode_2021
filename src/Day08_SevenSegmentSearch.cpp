#include "headers.hpp"

struct displays {
	std::array<uint8_t, 10> tips;
	std::array<uint8_t, 4> numbers;
};

using inputs = std::vector<displays>;

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
	std::fstream inputFile("./resources/Day08_SevenSegmentSearch.txt", std::fstream::in);
	inputs input{};

	const std::unordered_map<char, uint8_t> LUTSegments{
		{'a', 0x01},
		{'b', 0x02},
		{'c', 0x04},
		{'d', 0x08},
		{'e', 0x10},
		{'f', 0x20},
		{'g', 0x40},
	};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			input.emplace_back();
			auto parts = split(line, "|");
			size_t indexTips{};
			for(auto digit: split(parts[0], " ")) {
				for(auto c: digit) input.back().tips[indexTips] |= LUTSegments.at(c);
				indexTips++;
			}
			size_t indexDigits{};
			for(auto digit: split(parts[1], " ")) {
				for(auto c: digit) input.back().numbers[indexDigits] |= LUTSegments.at(c);
				indexDigits++;
			}
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	std::array<int, 128> relations{};
	int64_t count{};

	for(auto &line: input) {
		for(auto tip: line.tips) {
			int segmentCount = std::bitset<7>{tip}.count();
			if(segmentCount == 2) relations[tip] = 1;
			else if(segmentCount == 3) relations[tip] = 7;
			else if(segmentCount == 4) relations[tip] = 4;
			else if(segmentCount == 7) relations[tip] = 8;

		}
		for(auto number: line.numbers) {
			if(relations[number] != 0) count++;
		}
	}
	
	return count;
}

int64_t solve_part_two(const inputs &input) {
	std::array<int, 128> relations{};
	int64_t sum{};
	uint8_t one{};
	uint8_t eight{};
	uint8_t four{};
	uint8_t seven{};

	for(auto &line: input) {
		for(auto tip: line.tips) {
			int segmentCount = std::bitset<7>{tip}.count();
			if(segmentCount == 2) {
				relations[tip] = 1;
				one = tip;

			} else if(segmentCount == 3) {
				relations[tip] = 7;
				seven = tip;

			} else if(segmentCount == 4) {
				relations[tip] = 4;
				four = tip;

			} else if(segmentCount == 7) {
				relations[tip] = 8;
				eight = tip;

			}
		}
		for(auto tip: line.tips) {
			const int segmentCount = std::bitset<7>{tip}.count();
			if(segmentCount == 5) {
				if((tip & one) == one) relations[tip] = 3;
				else if((tip | four) == eight) relations[tip] = 2;
				else relations[tip] = 5;
			}
			if(segmentCount == 6) {
				if((tip & seven) != seven) relations[tip] = 6;
				else if((tip | four) == tip) relations[tip] = 9;
				else relations[tip] = 0;
			}
		}
		int64_t value{};
		for(auto digit: line.numbers) {
			value *= 10;
			value += relations[digit];
		}
		sum += value;
	}

	return sum;
}