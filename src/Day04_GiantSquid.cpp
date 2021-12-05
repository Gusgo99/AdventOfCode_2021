#include "headers.hpp"

struct inputs {
	std::vector<int64_t> numbers;
	std::vector<std::array<std::array<int64_t, 5>, 5>> boards;
};

inputs read_inputs_from_file();
int64_t solve_part_one(inputs _entries);
int64_t solve_part_two(inputs _entries);

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
	std::fstream inputFile("./resources/Day04_GiantSquid.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, line);
		for(auto i : split(line, ",")) {
			input.numbers.push_back(std::stoll(i.data()));
		}
		size_t row = 0;
		size_t column = 0;
		while(std::getline(inputFile, line)) {
			if(line.empty()) {
				input.boards.emplace_back();
				row = 0;

				continue;
			}
			column = 0;
			for(auto i : split(line, " ")) {
				input.boards.back()[row][column] = std::stoll(i.data());
				column++;
			}
			row++;
		}
	}

	return input;
}

bool has_won(const std::array<std::array<int64_t, 5>, 5> &board, const size_t row, const size_t column) {
	bool rowHasValue{};
	bool columnHasValue{};
	for(size_t i = 0; i < board.size(); i++) {
		if(board[row][i] != -1) rowHasValue = true;
		if(board[i][column] != -1) columnHasValue = true;
	}

	return !(rowHasValue && columnHasValue);
}

int64_t get_score(const std::array<std::array<int64_t, 5>, 5> &board, const int64_t lastNumber) {
	int64_t score{};

	for(auto &row: board) {
		for(auto cell: row) {
			if(cell != -1) {
				score += cell;
			}
		}
	}

	score *= lastNumber;

	return score;
}

int64_t solve_part_one(inputs input) {
	for(auto number: input.numbers) {
		for(auto &board: input.boards) {
			for(size_t i = 0; i < board.size(); i++) {
				for(size_t j = 0; j < board[i].size(); j++) {
					if(board[i][j] == number) {
						board[i][j] = -1;
						if(has_won(board, i, j)) {
							return get_score(board, number);
						}
					}
				}
			}
		}
	}

	return 0;
}

int64_t solve_part_two(inputs input) {
	size_t remainingBoards = input.boards.size();
	for(auto number: input.numbers) {
		auto newEnd = std::remove_if(
			input.boards.begin(),
			input.boards.end(),
			[&](auto &board) {
				if(remainingBoards > 0) {
					for(size_t i = 0; i < board.size(); i++) {
						for(size_t j = 0; j < board[i].size(); j++) {
							if(board[i][j] == number) {
								board[i][j] = -1;

								if(has_won(board, i, j)) {
									remainingBoards--;
									if(remainingBoards > 0) return true;
								}

								return false;
							}
						}
					}
				}
				return false;
			}
		);

		input.boards.resize(newEnd - input.boards.begin());
		if(remainingBoards == 0) {
			return get_score(input.boards[0], number);
		}
	}

	return 0;
}