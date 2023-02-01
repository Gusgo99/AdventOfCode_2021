#include "headers.hpp"

enum class Operation {inp, add, mul, div, mod, eql};

using instruction = std::tuple<Operation, size_t, std::variant<size_t, int64_t>>;
using inputs = std::vector<instruction>;
using State = std::tuple<int64_t, std::vector<int64_t>>;
struct Registers {
	int64_t w{};
	int64_t x{};
	int64_t y{};
	int64_t z{};
};

inputs read_entries_from_file();
int64_t solve_part_one(const inputs &input);
int64_t solve_part_two(const inputs &input);

int main() {
	const inputs input = read_entries_from_file();

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

inputs read_entries_from_file() {
	std::fstream inputFile("./resources/Day24_ArithmeticLogicUnit.txt", std::fstream::in);
	inputs input{};

	const std::map<std::string_view, Operation> textToOperation{
		{"inp", Operation::inp},
		{"add", Operation::add},
		{"mul", Operation::mul},
		{"div", Operation::div},
		{"mod", Operation::mod},
		{"eql", Operation::eql}
	};
	const std::map<std::string_view, size_t> textToRegister{
		{"w", 0},
		{"x", 1},
		{"y", 2},
		{"z", 3},
	};

	if(inputFile.is_open()) {
		std::string line;
		while(std::getline(inputFile, line)) {
			auto contents = split(line, " ");

			const Operation op = textToOperation.at(contents[0]);
			const size_t left = textToRegister.at(contents[1]);
			std::variant<size_t, int64_t> right{};

			if(contents.size() > 2) {
				if(std::isalpha(contents[2][0])) {
					right = textToRegister.at(contents[2]);
				} else {
					right = std::stoll(contents[2].data());
				}
			}

			input.emplace_back(op, left, right);
		}
	}

	return input;
}

void run_section(
	const inputs &instructions,
	Registers &regs,
	const size_t begin
) {
	for(size_t i = begin; i < instructions.size(); i++) {
		const auto &[op, leftIndex, rightVar] = instructions[i];

		int64_t Registers:: *const registersLUT[4] = {&Registers::w, &Registers::x, &Registers::y, &Registers::z};

		int64_t &left = regs.*(registersLUT[leftIndex]);
		const int64_t right = (rightVar.index() == 0) ? regs.*(registersLUT[std::get<0>(rightVar)]) : std::get<1>(rightVar);

		switch(op) {
			case Operation::add: left += right; break;
			case Operation::mul: left *= right; break;
			case Operation::div: left /= right; break;
			case Operation::mod: left %= right; break;
			case Operation::eql: left = (left == right) ? 1 : 0; break;
			case Operation::inp: return;
		}
	}
}

bool operator<(const State &lhs, const State &rhs) {
	const auto &[lhsZ, lhsDigits] = lhs;
	const auto &[rhsZ, rhsDigits] = rhs;

	const size_t minSize = std::min(lhsDigits.size(), rhsDigits.size());
	for(size_t i = 0; i < minSize; i++) {
		if(lhsDigits[i] != rhsDigits[i]) return lhsDigits[i] < rhsDigits[i];
	}

	return lhsDigits.size() < rhsDigits.size();
}

int64_t solve_part_one(const inputs &input) {
	std::vector<size_t> inpInstructions{};

	for(size_t i = 0; i < input.size(); i++) {
		if(std::get<0>(input[i]) == Operation::inp) inpInstructions.emplace_back(i);
	}

	std::stack<std::set<int64_t>> possibleZs{};
	possibleZs.emplace(std::set<int64_t>{0});
	for(auto i : inpInstructions) {
		std::set<int64_t> foundZs{};
		for(auto j : possibleZs.top()) {
			for(int64_t k = 1; k < 10; k++) {
				Registers regs{k, 0, 0, j};
				run_section(input, regs, i + 1);
				if(regs.z < 5000000) foundZs.emplace(regs.z);
			}
		}
		possibleZs.emplace(std::move(foundZs));
	}

	std::reverse(inpInstructions.begin(), inpInstructions.end());

	std::vector<std::set<int64_t>> goodZs{{0}};
	for(auto i : inpInstructions) {
		std::set<int64_t> foundZs{};
		possibleZs.pop();
		for(auto &j : possibleZs.top()) {
			for(int64_t k = 1; k < 10; k++) {
				Registers regs{k, 0, 0, j};
				run_section(input, regs, i + 1);
				if(goodZs.back().find(regs.z) != goodZs.back().end()) {
					foundZs.emplace(j);
				}
			}
		}
		goodZs.emplace_back(std::move(foundZs));
	}
	std::reverse(goodZs.begin(), goodZs.end());

	std::priority_queue<State> toCheck{};
	for(int64_t i = 1; i < 10; i++) {
		toCheck.emplace(0, std::vector<int64_t>{i});

	}

	std::reverse(inpInstructions.begin(), inpInstructions.end());

	while(!toCheck.empty()) {
		const auto [currentZ, currentDigits] = toCheck.top();
		toCheck.pop();

		if(goodZs[currentDigits.size() - 1].find(currentZ) == goodZs[currentDigits.size() - 1].end()) continue;

		Registers regs{currentDigits.back(), 0, 0, currentZ};
		run_section(input, regs, inpInstructions[currentDigits.size() - 1] + 1);

		if(currentDigits.size() < 14) {
			for(int64_t i = 9; i > 0; i--) {
				auto newDigits = currentDigits;
				newDigits.emplace_back(i);
				toCheck.emplace(regs.z, std::move(newDigits));
			}
		} else {
			if(regs.z == 0) {
				int64_t answer{};
				for(auto i : currentDigits) {
					answer *= 10;
					answer += i;
				}
				return answer;
			}
		}
	}

	return 0;
}

int64_t solve_part_two(const inputs &input) {
	std::vector<size_t> inpInstructions{};

	for(size_t i = 0; i < input.size(); i++) {
		if(std::get<0>(input[i]) == Operation::inp) inpInstructions.emplace_back(i);
	}

	std::stack<std::set<int64_t>> possibleZs{};
	possibleZs.emplace(std::set<int64_t>{0});
	for(auto i : inpInstructions) {
		std::set<int64_t> foundZs{};
		for(auto j : possibleZs.top()) {
			for(int64_t k = 1; k < 10; k++) {
				Registers regs{k, 0, 0, j};
				run_section(input, regs, i + 1);
				if(regs.z < 5000000) foundZs.emplace(regs.z);
			}
		}
		possibleZs.emplace(std::move(foundZs));
	}

	std::reverse(inpInstructions.begin(), inpInstructions.end());

	std::vector<std::set<int64_t>> goodZs{{0}};
	for(auto i: inpInstructions) {
		std::set<int64_t> foundZs{};
		possibleZs.pop();
		for(auto &j : possibleZs.top()) {
			for(int64_t k = 1; k < 10; k++) {
				Registers regs{k, 0, 0, j};
				run_section(input, regs, i + 1);
				if(goodZs.back().find(regs.z) != goodZs.back().end()) {
					foundZs.emplace(j);
				}
			}
		}
		goodZs.emplace_back(std::move(foundZs));
	}
	std::reverse(goodZs.begin(), goodZs.end());

	std::priority_queue<State, std::vector<State>, std::greater<State>> toCheck{};
	for(int64_t i = 1; i < 10; i++) {
		toCheck.emplace(0, std::vector<int64_t>{i});

	}

	std::reverse(inpInstructions.begin(), inpInstructions.end());

	while(!toCheck.empty()) {
		const auto [currentZ, currentDigits] = toCheck.top();
		toCheck.pop();

		if(goodZs[currentDigits.size() - 1].find(currentZ) == goodZs[currentDigits.size() - 1].end()) continue;

		Registers regs{currentDigits.back(), 0, 0, currentZ};
		run_section(input, regs, inpInstructions[currentDigits.size() - 1] + 1);

		if(currentDigits.size() < 14) {
			for(int64_t i = 9; i > 0; i--) {
				auto newDigits = currentDigits;
				newDigits.emplace_back(i);
				toCheck.emplace(regs.z, std::move(newDigits));
			}
		} else {
			if(regs.z == 0) {
				int64_t answer{};
				for(auto i : currentDigits) {
					answer *= 10;
					answer += i;
				}
				return answer;
			}
		}
	}

	return 0;
}