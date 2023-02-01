#include "headers.hpp"

enum class foldDirection { xFold, yFold };

struct inputs {
	std::vector<std::pair<int64_t, int64_t>> points;
	std::vector<std::pair<foldDirection, int64_t>> folds;
};

inputs read_inputs_from_file();
int64_t solve_part_one(const inputs &input);
std::string solve_part_two(const inputs &input);

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
	std::fstream inputFile("./resources/Day13_TransparentOrigami.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			if(line.empty()) continue;
			if(line[0] == 'f') {
				foldDirection dir{};
				if(line.find("x") != std::string::npos) dir = foldDirection::xFold;
				else dir = foldDirection::yFold;
				input.folds.emplace_back(dir, std::stoll(line.substr(line.find('=') + 1)));

			} else {
				auto x = line.substr(0, line.find(','));
				auto y = line.substr(line.find(',') + 1);
				input.points.emplace_back(std::stoll(x), std::stoll(y));
			}
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	std::unordered_set<std::pair<int64_t, int64_t>, hash_pair> folded;

	auto [foldDir, coord] = input.folds[0];
	for(auto [x, y]: input.points) {
		auto &toLook = (foldDir == foldDirection::xFold) ? x : y;
		if(toLook > coord) {
			toLook = 2 * coord - toLook;
		}
		if(toLook != coord) {
			folded.insert({x, y});
		}
	}

	return folded.size();
}

std::string solve_part_two(const inputs &input) {
	std::unordered_set<std::pair<int64_t, int64_t>, hash_pair> current{input.points.begin(), input.points.end()};
	std::unordered_set<std::pair<int64_t, int64_t>, hash_pair> next;

	int64_t maxX{};
	int64_t maxY{};

	for(auto [foldDir, coord]: input.folds) {
		if(foldDir == foldDirection::xFold) maxX = coord;
		else maxY = coord;
		for(auto [x, y]: current) {
			auto &toLook = (foldDir == foldDirection::xFold) ? x : y;
			if(toLook > coord) {
				toLook = 2 * coord - toLook;
			}
			if(toLook != coord) {
				next.insert({x, y});
			}
		}
		std::swap(current, next);
	}

	std::string folded = "\n";
	for(size_t y = 0; y < maxY; y++) {
		for(size_t x = 0; x < maxX; x++) {
			if(current.find({x, y}) != current.end()) folded += '#';
			else folded += ' ';
		}
		folded += '\n';
	}

	return folded;
}