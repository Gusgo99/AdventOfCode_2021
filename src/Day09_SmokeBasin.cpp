#include "headers.hpp"

struct inputs {
	size_t width;
	size_t height;
	std::string heightmap;
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
	std::fstream inputFile("./resources/Day09_SmokeBasin.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			input.width = line.size();
			input.height++;
			input.heightmap += line;
		}
	}

	return input;
}

std::array<size_t, 4> get_neighbours(const size_t pos, const size_t width, const size_t height) {
	std::array<size_t, 4> neighbours{SIZE_MAX, SIZE_MAX, SIZE_MAX, SIZE_MAX};

	if(pos >= width) neighbours[0] = pos - width;

	if(pos < (width * (height - 1))) neighbours[1] = pos + width;

	if((pos % width) > 0) neighbours[2] = pos - 1;

	if((pos % width) < (width - 1)) neighbours[3] = pos + 1;

	return neighbours;
}

int64_t solve_part_one(const inputs &input) {
	int64_t count{};

	for(size_t i = 0; i < input.heightmap.size(); i++) {
		const auto neighbours = get_neighbours(i, input.width, input.height);
		bool isLowPoint = true;
		for(auto neighbour: neighbours) {
			if(neighbour >= input.heightmap.size()) continue;
			if(input.heightmap[i] >= input.heightmap[neighbour]) {
				isLowPoint = false;
				break;
			}
		}
		if(isLowPoint) count += int64_t(input.heightmap[i]) - int64_t('0') + 1;
	}
	
	return count;
}

int64_t solve_part_two(const inputs &input) {
	std::vector<std::pair<size_t, size_t>> basins;

	for(size_t i = 0; i < input.heightmap.size(); i++) {
		const auto neighbours = get_neighbours(i, input.width, input.height);
		bool isLowest = true;
		for(auto neighbour: neighbours) {
			if(neighbour >= input.heightmap.size()) continue;
			if(input.heightmap[i] >= input.heightmap[neighbour]) {
				isLowest = false;
				break;
			}
		}
		if(isLowest) basins.emplace_back(i, 0);
	}

	std::vector<bool> visited(input.heightmap.size());
	for(auto &[lowPoint, size]: basins) {
		std::stack<size_t> toVisit;
		toVisit.push(lowPoint);
		while(!toVisit.empty()) {
			const auto currentPoint = toVisit.top();
			toVisit.pop();
			if(visited[currentPoint]) continue;
			visited[currentPoint] = true;
			if(input.heightmap[currentPoint] == '9') continue;
			size++;
			auto neighbours = get_neighbours(currentPoint, input.width, input.height);
			for(auto i: neighbours) {
				if(i < input.heightmap.size()) {
					if(!visited[i]) toVisit.push(i);
				}
			}
		}
	}

	std::sort(basins.begin(), basins.end(), [](auto left, auto right) {
		return left.second > right.second;
	});

	return basins[0].second * basins[1].second * basins[2].second;
}