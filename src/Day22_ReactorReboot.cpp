#include "headers.hpp"

using cuboid = std::array<std::pair<int64_t, int64_t>, 3>;
using inputs = std::vector<std::pair<bool, cuboid>>;

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
	std::fstream inputFile("./resources/Day22_ReactorReboot.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			input.emplace_back();
			input.back().first = line[1] == 'n';

			line = line.substr(line.find("=") + 1);
			input.back().second[0].first = std::stoll(line);
			line = line.substr(line.find(".") + 2);
			input.back().second[0].second = std::stoll(line);

			line = line.substr(line.find("=") + 1);
			input.back().second[1].first = std::stoll(line);
			line = line.substr(line.find(".") + 2);
			input.back().second[1].second = std::stoll(line);

			line = line.substr(line.find("=") + 1);
			input.back().second[2].first = std::stoll(line);
			line = line.substr(line.find(".") + 2);
			input.back().second[2].second = std::stoll(line);
		}
	}

	return input;
}

std::optional<cuboid> intersect(const cuboid &a, const cuboid &b) {
	cuboid intersection{};

	for(size_t i = 0; i < a.size(); i++) {
		intersection[i].first = std::max(a[i].first, b[i].first);
		intersection[i].second = std::min(a[i].second, b[i].second);

		if(intersection[i].first > intersection[i].second) return {};
	}

	return intersection;
}

int64_t get_volume(const cuboid &c) {
	int64_t dx = c[0].second - c[0].first + 1;
	int64_t dy = c[1].second - c[1].first + 1;
	int64_t dz = c[2].second - c[2].first + 1;

	return dx * dy * dz;
}

int64_t solve_part_one(const inputs &input) {
	std::vector<std::pair<bool, cuboid>> simplifiedInstructions;
	const std::pair<int64_t, int64_t> initializationRange{-50, 50};
	const cuboid initializationCuboid{initializationRange, initializationRange, initializationRange};

	for(auto &[turnOn, currentCuboid] : input) {
		if(!intersect(currentCuboid, initializationCuboid)) continue;
		const size_t alreadyAddedCubes = simplifiedInstructions.size();
		if(turnOn) simplifiedInstructions.emplace_back(turnOn, currentCuboid);
		for(size_t i = 0; i < alreadyAddedCubes; i++) {
			auto intersection = intersect(currentCuboid, simplifiedInstructions[i].second);
			if(intersection) {
				simplifiedInstructions.emplace_back(!simplifiedInstructions[i].first, *intersection);
			}
		}
	}

	int64_t onCubes{};

	for(auto [turnOn, simplifiedCuboid] : simplifiedInstructions) {
		auto cuboidVolume = get_volume(simplifiedCuboid);

		onCubes += turnOn ? cuboidVolume : -cuboidVolume;
	}

	return onCubes;
}

int64_t solve_part_two(const inputs &input) {
	std::vector<std::pair<bool, cuboid>> simplifiedInstructions;

	for(auto &[turnOn, currentCuboid]: input) {
		const size_t alreadyAddedCubes = simplifiedInstructions.size();
		if(turnOn) simplifiedInstructions.emplace_back(turnOn, currentCuboid);
		for(size_t i = 0; i < alreadyAddedCubes; i++) {
			auto intersection = intersect(currentCuboid, simplifiedInstructions[i].second);
			if(intersection) {
				simplifiedInstructions.emplace_back(!simplifiedInstructions[i].first, *intersection);
			}
		}
	}
	
	int64_t onCubes{};

	for(auto [turnOn, simplifiedCuboid]: simplifiedInstructions) {
		auto cuboidVolume = get_volume(simplifiedCuboid);

		onCubes += turnOn ? cuboidVolume : -cuboidVolume;
	}

	return onCubes;
}