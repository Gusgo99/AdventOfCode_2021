#include "headers.hpp"

using inputs = std::array<int64_t, 4>;

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
	std::fstream inputFile("./resources/Day17_TrickShot.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, line);

		line = line.substr(line.find("x=") + 2);
		input[0] = std::stoll(line.substr(0, line.find("..")));
		line = line.substr(line.find("..") + 2);
		input[1] = std::stoll(line.substr(0, line.find(",")));

		line = line.substr(line.find("y=") + 2);
		input[2] = std::stoll(line.substr(0, line.find("..")));
		line = line.substr(line.find("..") + 2);
		input[3] = std::stoll(line);
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	int64_t maxDistancePerStep = std::abs(input[2]);

	return maxDistancePerStep * (maxDistancePerStep - 1) / 2;
}

int64_t solve_part_two(const inputs &input) {
	std::set<std::pair<int64_t, int64_t>> velocities;

	for(int64_t x = input[0]; x <= input[1]; x++) {
		for(int64_t y = input[2]; y <= input[3]; y++) {
			for(int64_t step = 1; step <= (std::abs(2 * y) + 1); step++) {
				bool yHitsTarget = ((2 * y + step * (step - 1)) % (2 * step)) == 0;
				if(yHitsTarget) {
					bool xHitsTargetBeforeStopping = 2 * x - step > step * step;
					xHitsTargetBeforeStopping &= ((2 * x + step * (step - 1)) % (2 * step)) == 0;
					if(xHitsTargetBeforeStopping) {
						std::pair<int64_t, int64_t> velocity{};
						velocity.first = (2 * x + step * (step - 1)) / (2 * step);
						velocity.second = (2 * y + step * (step - 1)) / (2 * step);
						velocities.insert(velocity);
					}

					int64_t deltaSqrt = std::round(std::sqrt(1 + 8 * x));
					bool xHitsTargetAfterStopping = deltaSqrt * deltaSqrt == 1 + 8 * x;
					xHitsTargetAfterStopping &= ((deltaSqrt - 1) / 2) <= step;
					if(xHitsTargetAfterStopping) {
						std::pair<int64_t, int64_t> velocity{};
						velocity.first = ((deltaSqrt - 1) / 2);
						velocity.second = (2 * y + step * (step - 1)) / (2 * step);
						velocities.insert(velocity);
					}
				}
			}
		}
	}

	return velocities.size();
}