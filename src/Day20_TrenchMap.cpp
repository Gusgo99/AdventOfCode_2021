#include "headers.hpp"

struct inputs {
	std::string imageEnhancementString;
	std::string image;
	int64_t imageWidth;
	int64_t imageHeight;
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
	//std::fstream inputFile("./resources/__.txt", std::fstream::in);
	std::fstream inputFile("./resources/Day20_TrenchMap.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, input.imageEnhancementString);
		std::getline(inputFile, line);
		while(std::getline(inputFile, line)) {
			input.image += line;
			input.imageHeight++;
			input.imageWidth = line.size();
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	std::unordered_set<std::pair<int64_t, int64_t>, hash_pair> litPixels;
	std::pair<int64_t, int64_t> start{-1, -1};
	std::pair<int64_t, int64_t> end{input.imageWidth, input.imageHeight};
	bool outsidePixels = true;

	for(size_t i = 0; i < input.image.size(); i++) {
		if(input.image[i] == '#') {
			litPixels.emplace(i % input.imageWidth, i / input.imageWidth);
		}
	}

	for(int i = 0; i < 2; i++) {
		if(
			(input.imageEnhancementString[0] == '#') &&
			(input.imageEnhancementString.back() == '.')
		) {
			outsidePixels = !outsidePixels;

			if((i % 2) != 0) {
				for(auto i = start.first; i <= end.first; i++) {
					litPixels.emplace(i, start.second);
					litPixels.emplace(i, end.second);
				}
				for(auto i = start.second; i <= end.second; i++) {
					litPixels.emplace(start.first, i);
					litPixels.emplace(end.first, i);
				}
			}
		}
		std::unordered_set<std::pair<int64_t, int64_t>, hash_pair> nextLitPixels;
		for(int64_t y = start.second; y <= end.second; y++) {
			for(int64_t x = start.first; x <= end.first; x++) {
				size_t index{};
				for(auto dy: {y - 1, y, y + 1}) {
					for(auto dx: {x - 1, x, x + 1}) {
						index <<= 1;

						if((dx < start.first) || (dx > end.first) || (dy < start.second) || (dy > end.second)) {
							index |= (outsidePixels ? 1 : 0);

						} else if(litPixels.find({dx, dy}) != litPixels.end()) {
							index |= 1;
						}
					}
				}
				if(input.imageEnhancementString[index] == '#') {
					nextLitPixels.emplace(x, y);
				}
			}
		}
		litPixels = std::move(nextLitPixels);
		start.first--;
		start.second--;
		end.first++;
		end.second++;
	}
	
	return litPixels.size();
}

int64_t solve_part_two(const inputs &input) {
	std::unordered_set<std::pair<int64_t, int64_t>, hash_pair> litPixels;
	std::pair<int64_t, int64_t> start{-1, -1};
	std::pair<int64_t, int64_t> end{input.imageWidth, input.imageHeight};
	bool outsidePixels = true;

	for(size_t i = 0; i < input.image.size(); i++) {
		if(input.image[i] == '#') {
			litPixels.emplace(i % input.imageWidth, i / input.imageWidth);
		}
	}

	for(int i = 0; i < 50; i++) {
		if(
			(input.imageEnhancementString[0] == '#') &&
			(input.imageEnhancementString.back() == '.')
			) {
			outsidePixels = !outsidePixels;

			if((i % 2) != 0) {
				for(auto i = start.first; i <= end.first; i++) {
					litPixels.emplace(i, start.second);
					litPixels.emplace(i, end.second);
				}
				for(auto i = start.second; i <= end.second; i++) {
					litPixels.emplace(start.first, i);
					litPixels.emplace(end.first, i);
				}
			}
		}
		std::unordered_set<std::pair<int64_t, int64_t>, hash_pair> nextLitPixels;
		for(int64_t y = start.second; y <= end.second; y++) {
			for(int64_t x = start.first; x <= end.first; x++) {
				size_t index{};
				for(auto dy: {y - 1, y, y + 1}) {
					for(auto dx: {x - 1, x, x + 1}) {
						index <<= 1;

						if((dx < start.first) || (dx > end.first) || (dy < start.second) || (dy > end.second)) {
							index |= (outsidePixels ? 1 : 0);

						} else if(litPixels.find({dx, dy}) != litPixels.end()) {
							index |= 1;
						}
					}
				}
				if(input.imageEnhancementString[index] == '#') {
					nextLitPixels.emplace(x, y);
				}
			}
		}
		litPixels = std::move(nextLitPixels);
		start.first--;
		start.second--;
		end.first++;
		end.second++;
	}

	return litPixels.size();
}