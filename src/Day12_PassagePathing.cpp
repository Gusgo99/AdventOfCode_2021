#include "headers.hpp"

using inputs = std::vector<std::pair<std::unordered_set<size_t>, bool>>;

constexpr size_t startNode = 0;
constexpr size_t endNode = 1;

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
	std::fstream inputFile("./resources/Day12_PassagePathing.txt", std::fstream::in);
	inputs input{};
	std::unordered_map<std::string, size_t> indexes;

	indexes["start"] = startNode;
	indexes["end"] = endNode;

	input.resize(2);

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			auto start = line.substr(0, line.find("-"));
			auto end = line.substr(start.size() + 1);
			if(indexes.find(start) == indexes.end()) {
				indexes[start] = input.size();
				input.emplace_back();
			}
			if(indexes.find(end) == indexes.end()) {
				indexes[end] = input.size();
				input.emplace_back();
			}

			input[indexes[start]].first.insert(indexes[end]);
			input[indexes[start]].second = start[0] == std::toupper(start[0]);
			input[indexes[end]].first.insert(indexes[start]);
			input[indexes[end]].second = end[0] == std::toupper(end[0]);
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	std::stack<std::pair<size_t, std::vector<size_t>>> toVisit{};
	size_t paths{};

	toVisit.push({0, {}});

	while(!toVisit.empty()) {
		const auto current = toVisit.top().first;
		auto toAvoid = std::move(toVisit.top().second);
		toVisit.pop();

		if(!input[current].second) {
			toAvoid.push_back(current);
		}

		for(auto i: input[current].first) {
			if(i == startNode) continue;
			if(i == endNode) {
				paths++;
				continue;
			}
			if(std::find(toAvoid.begin(), toAvoid.end(), i) == toAvoid.end()) toVisit.emplace(i, toAvoid);
		}
	}
	
	return paths;
}

int64_t solve_part_two(const inputs &input) {
	std::stack<std::tuple<size_t, bool, std::vector<size_t>>> toVisit{};
	size_t paths{};

	toVisit.push({0, false, {}});

	while(!toVisit.empty()) {
		const auto current = std::get<0>(toVisit.top());
		const auto visitedSmallTwice = std::get<1>(toVisit.top());
		auto toAvoid = std::move(std::get<2>(toVisit.top()));
		toVisit.pop();

		if(!input[current].second) {
			toAvoid.push_back(current);
		}

		for(auto i: input[current].first) {
			if(i == startNode) continue;
			if(i == endNode) {
				paths++;
				continue;
			}
			if(std::find(toAvoid.begin(), toAvoid.end(), i) == toAvoid.end()) toVisit.emplace(i, visitedSmallTwice, toAvoid);
			else if(!visitedSmallTwice) toVisit.emplace(i, true, toAvoid);
		}
	}

	return paths;
}