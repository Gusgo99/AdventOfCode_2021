#include "headers.hpp"

using number = std::vector<std::pair<int, int>>;
using inputs = std::vector<number>;

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
	std::fstream inputFile("./resources/Day18_Snailfish.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			input.emplace_back();
			int depth{};
			for(auto c: line) {
				if((c >= '0') && (c <= '9')) {
					input.back().emplace_back(c - '0', depth);
				} else if(c == '[') depth++;
				else if(c == ']') depth--;
			}
		}
	}

	return input;
}

bool explode(number &num) {
	for(size_t i = 0; i < num.size(); i++) {
		if(num[i].second > 4) {
			if(i > 0) {
				num[i - 1].first += num[i].first;
			}
			num[i].first = 0;
			if((i + 2) < num.size()) {
				num[i + 2].first += num[i + 1].first;
			}

			num[i].second--;

			num.erase(num.begin() + i + 1);

			return true;
		}
	}
	return false;
}

bool split(number &num) {
	for(size_t i = 0; i < num.size(); i++) {
		if(num[i].first > 9) {
			auto current = num[i].first;

			num.emplace(num.begin() + i, num[i]);

			num[i].first = current / 2;
			num[i + 1].first = (current + 1) / 2;

			num[i].second++;
			num[i + 1].second++;

			return true;
		}
	}
	return false;
}

void reduce(number &num) {
	while(explode(num) || split(num));
}

void operator+=(number &lhs, const number &rhs) {
	lhs.insert(lhs.end(), rhs.begin(), rhs.end());

	for(auto &i: lhs) i.second++;

	reduce(lhs);
}

number operator+(const number &lhs, const number &rhs) {
	auto result = lhs;

	result += rhs;

	return result;
}

int64_t calculate_magnitude(const number &num) {
	const std::array<int, 16> returnBy{1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 4};
	std::stack<int64_t> history;
	int64_t multiplier{1};
	size_t depthRepeatCount{};

	for(int64_t i = 0; i < num[0].second; i++) {
		multiplier *= 3;
		history.push(3);
	}

	int64_t magnitude = num[0].first * multiplier;

	for(size_t i = 1; i < num.size(); i++) {
		const auto currentDepth = num[i - 1].second - returnBy[depthRepeatCount];

		for(int64_t j = num[i - 1].second; j > currentDepth; j--) {
			multiplier /= history.top();
			history.pop();
		}

		for(int j = currentDepth; j < num[i].second; j++) {
			int64_t toMultiply = 3;
			if(j == currentDepth) toMultiply = 2;
			multiplier *= toMultiply;
			history.push(toMultiply);
		}

		if(num[i].second == num[i - 1].second) depthRepeatCount++;
		else depthRepeatCount = 0;

		magnitude += num[i].first * multiplier;
	}

	return magnitude;
}

int64_t solve_part_one(const inputs &input) {
	number acc = input[0];
	for(size_t i = 1; i < input.size(); i++) {
		acc += input[i];
	}
	
	return calculate_magnitude(acc);
}

int64_t solve_part_two(const inputs &input) {
	int64_t largest{};

	for(size_t i = 0; i < input.size(); i++) {
		for(size_t j = i + 1; j < input.size(); j++) {
			const auto sum = input[i] + input[j];
			const auto magnitude = calculate_magnitude(sum);

			if(magnitude > largest) largest = magnitude;
		}
	}
	
	return largest;
}