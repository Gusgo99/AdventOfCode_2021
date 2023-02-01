#include "headers.hpp"

using inputs = std::vector<int64_t>;

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
	std::fstream inputFile("./resources/Day07_TheTreacheryOfWhales.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, line);
		for(auto i: split(line, ",")) {
			input.push_back(std::stoll(i.data()));
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	auto orderedSubs = input;
	std::sort(orderedSubs.begin(), orderedSubs.end());
	std::vector<int64_t> accPos;

	for(auto i : orderedSubs) {
		if(accPos.empty()) accPos.emplace_back(i);
		else accPos.emplace_back(accPos.back() + i);
	}

	std::vector<int64_t> fuelAmount;
	size_t index{};
	for(int64_t depth = orderedSubs.front(); depth <= orderedSubs.back(); depth++) {
		while((index < orderedSubs.size()) && (orderedSubs[index] <= depth)) index++;
		const int64_t lowerPosFuelAmount = index * depth - accPos[index - 1];
		const int64_t higherPosFuelAmount = 
			(accPos.back() - accPos[index - 1]) -
			(input.size() - index) * depth;
		fuelAmount.emplace_back(lowerPosFuelAmount + higherPosFuelAmount);
	}
	
	return *std::min_element(fuelAmount.begin(), fuelAmount.end());
}

int64_t solve_part_two(const inputs &input) {
	auto [minPos, maxPos] = std::minmax_element(input.begin(), input.end());

	std::vector<int64_t> consumptions;
	for(auto i: input) {
		const int64_t dist = i - *minPos;
		consumptions.push_back(dist * (dist + 1) / 2);
	}

	std::vector<int64_t> fuelAmount;
	for(int64_t pos = *minPos; pos <= *maxPos; pos++) {
		fuelAmount.push_back(std::accumulate(consumptions.begin(), consumptions.end(), 0));
		for(size_t i = 0; i < consumptions.size(); i++) {
			int64_t consumptionDiff = (pos + 1) - input[i];
			if(pos < input[i]) consumptionDiff -= 1;
			consumptions[i] += consumptionDiff;
		}
	}

	return *std::min_element(fuelAmount.begin(), fuelAmount.end());
}