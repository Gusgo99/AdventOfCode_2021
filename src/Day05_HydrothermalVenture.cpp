#include "headers.hpp"

using coordinates = std::array<int64_t, 4>;
using inputs = std::vector<coordinates>;

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
	std::fstream inputFile("./resources/Day05_HydrothermalVenture.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line{};
		while(std::getline(inputFile, line)) {
			coordinates coords{};

			coords[0] = std::stoll(line);
			line = line.substr(line.find(",") + 1);

			coords[1] = std::stoll(line);
			line = line.substr(line.find("->") + 2);

			coords[2] = std::stoll(line);
			line = line.substr(line.find(",") + 1);

			coords[3] = std::stoll(line);

			input.push_back(coords);
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	std::unordered_map<std::pair<int64_t, int64_t>, size_t, hash_pair> grid;

	for(auto [x1, y1, x2, y2]: input) {
		if(x1 == x2) {
			for(int64_t i = std::min(y1, y2); i <= std::max(y1, y2); i++) {
				grid[{x1, i}]++;
			}
		} else if(y1 == y2) {
			for(int64_t i = std::min(x1, x2); i <= std::max(x1, x2); i++) {
				grid[{i, y1}]++;
			}
		}
	}

	int64_t intersections{};

	for(auto [point, repeats]: grid) {
		if(repeats > 1) intersections++;
	}

	return intersections;
}

int64_t solve_part_two(const inputs &input) {
	std::unordered_map<std::pair<int64_t, int64_t>, size_t, hash_pair> grid;

	for(auto [x1, y1, x2, y2]: input) {
		if(x1 == x2) {
			for(int64_t i = std::min(y1, y2); i <= std::max(y1, y2); i++) {
				grid[{x1, i}]++;
			}
		} else if(y1 == y2) {
			for(int64_t i = std::min(x1, x2); i <= std::max(x1, x2); i++) {
				grid[{i, y1}]++;
			}
		} else if((std::max(x1, x2) - std::min(x1, x2)) == (std::max(y1, y2) - std::min(y1, y2))) {
			if(x1 > x2) {
				std::swap(x1, x2);
				std::swap(y1, y2);
			}
			for(int64_t i = x1; i <= x2; i++) {
				grid[{i, y1}]++;
				if(y1 < y2) y1++;
				else y1--;
			}
		}
	}

	int64_t intersections{};

	for(auto [point, repeats]: grid) {
		if(repeats > 1) intersections++;
	}

	return intersections;
}