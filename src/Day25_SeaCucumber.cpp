#include "headers.hpp"

struct inputs {
	std::string map;
	int64_t width;
	int64_t height;
};

inputs read_inputs_from_file();
int64_t solve_part_one(const inputs &input);

int main() {
	const inputs input = read_inputs_from_file();

	auto start = std::chrono::high_resolution_clock::now();
	auto solutionPart1 = solve_part_one(input);
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "Part 1 solution: " << solutionPart1 << std::endl;
	std::cout << "Part 1 time: " << std::chrono::duration<double>(end - start).count() << std::endl;

	return 0;
}

inputs read_inputs_from_file() {
	std::fstream inputFile("./resources/Day25_SeaCucumber.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			input.width = line.size();
			input.height++;
			input.map += line;
		}
	}

	return input;
}

int64_t solve_part_one(const inputs &input) {
	auto currentState = input.map;
	int64_t stepCount{};
	
	while(true) {
		std::string nextState(currentState.size(), '.');
		bool hasMoved{};

		//for(int i = 0; i < input.height; i++) {
		//	std::cout << currentState.substr(i * input.width, input.width) << "\n";
		//}
		//std::cout << "----\n";

		for(auto toMove: {'>', 'v'}) {
			for(size_t i = 0; i < currentState.size(); i++) {
				if((toMove == '>') && (currentState[i] == '>')) {
					size_t rightPos = i + 1;
					if((rightPos % input.width) == 0) rightPos -= input.width;
					if((currentState[rightPos] == '.') && (nextState[rightPos] == '.')) {
						nextState[rightPos] = currentState[i];
						hasMoved = true;

					} else {
						nextState[i] = currentState[i];
					}
				}
				if((toMove == 'v') && (currentState[i] == 'v')) {
					const size_t downPos = (i + input.width) % currentState.size();
					if((currentState[downPos] != 'v') && (nextState[downPos] == '.')) {
						nextState[downPos] = currentState[i];
						hasMoved = true;

					} else {
						nextState[i] = currentState[i];
					}
				}
			}
		}

		stepCount++;

		if(!hasMoved) break;
		currentState = std::move(nextState);
	}

	return stepCount;
}