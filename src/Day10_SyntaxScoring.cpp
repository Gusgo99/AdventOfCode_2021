#include "headers.hpp"

using inputs = std::vector<std::string>;

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
	std::fstream inputFile("./resources/Day10_SyntaxScoring.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			input.emplace_back(line);
		}
	}

	return input;
}

bool is_character_opening(const char c) {
	const std::array<char, 4> openings{'(', '[', '{', '<'};

	for(auto i: openings) if(c == i) return true;

	return false;
}

bool is_character_closing(const char c) {
	const std::array<char, 4> closings{')', ']', '}', '>'};

	for(auto i: closings) if(c == i) return true;

	return false;
}

bool do_characters_match(const char first, const char second) {
	const std::array<std::pair<char, char>, 4> matches{{
		{'(', ')'},
		{'[', ']'},
		{'{', '}'},
		{'<', '>'}
	}};

	for(auto i: matches) {
		if((first == i.first) && (second == i.second)) return true;
	}

	return false;
}

int64_t get_error_score(const char c) {
	switch(c) {
		case ')': return 3;
		case ']': return 57;
		case '}': return 1197;
		case '>': return 25137;
	}
	return 0;
}

int64_t solve_part_one(const inputs &input) {
	int64_t score{};

	for(auto &line: input) {
		std::stack<char> openings;
		for(auto c: line) {
			if(is_character_opening(c)) {
				openings.push(c);

			} else if(is_character_closing(c)) {
				auto first = openings.top();
				openings.pop();

				if(!do_characters_match(first, c)) {
					score += get_error_score(c);
				}
			}
		}
	}
	
	return score;
}

int64_t get_missing_score(const char c) {
	switch(c) {
		case '(': return 1;
		case '[': return 2;
		case '{': return 3;
		case '<': return 4;
	}
	return 0;
}

int64_t solve_part_two(const inputs &input) {
	std::vector<int64_t> scores;

	for(auto &line: input) {
		std::stack<char> openings;
		for(auto c: line) {
			if(is_character_opening(c)) {
				openings.push(c);

			} else if(is_character_closing(c)) {
				auto first = openings.top();
				openings.pop();
				if(!do_characters_match(first, c)) {
					openings = std::stack<char>{};
					break;
				}
			}
		}
		if(!openings.empty()) scores.push_back(0);
		while(!openings.empty()) {
			scores.back() = 5 * scores.back() + get_missing_score(openings.top());
			openings.pop();
		}
	}

	std::sort(scores.begin(), scores.end());

	return scores[scores.size() / 2];
}