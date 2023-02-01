#include "headers.hpp"

using inputs = std::vector<uint8_t>;

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
	std::fstream inputFile("./resources/Day16_PacketDecoder.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		std::getline(inputFile, line);
		for(size_t i = 0; i < line.size() / 2; i++) {
			std::stringstream ss;
			ss << line.substr(2 * i, 2) << std::hex;
			unsigned value{};
			ss >> value;
			input.push_back(value);
		}
	}

	return input;
}

uint64_t take_bits(const inputs &input, size_t &takenBits, const size_t bitsToTake) {
	uint64_t bits{};

	const auto byteToStartTaking = takenBits / 8;
	const auto byteToEndTaking = (takenBits + bitsToTake) / 8;
	for(size_t i = byteToStartTaking; i <= byteToEndTaking; i++) {
		bits <<= 8;
		if(i >= input.size()) break;
		bits |= input[i];
	}

	takenBits += bitsToTake;

	bits >>= 8 - (takenBits % 8);

	bits &= (1ULL << bitsToTake) - 1;

	return bits;
}

int64_t solve_part_one(const inputs &input) {
	size_t takenBits{};
	std::stack<std::pair<bool, int>> subPackets;

	subPackets.emplace(true, 1);

	int64_t versionSum{};
	while(!subPackets.empty()) {
		auto [bits, end] = subPackets.top();
		
		if(bits) {
			if(end <= takenBits) {
				subPackets.pop();
				continue;
			}
		} else {
			if(end > 1) subPackets.top().second -= 1;
			else subPackets.pop();
		}

		auto V = take_bits(input, takenBits, 3);
		versionSum += V;

		auto T = take_bits(input, takenBits, 3);
		if(T == 4) {
			while(take_bits(input, takenBits, 5) & 0x10);

		} else {
			auto I = take_bits(input, takenBits, 1);
			if(I == 0) {
				auto L = take_bits(input, takenBits, 15);
				subPackets.emplace(true, takenBits + L);

			} else {
				auto L = take_bits(input, takenBits, 11);
				subPackets.emplace(false, L);
			}
		}
	}

	return versionSum;
}

enum class operation {sum, multiply, minimum, maximum, value, greaterThan, lessThan, equalTo};

int64_t execute_operation(int64_t a, int64_t b, operation op) {
	switch(op) {
		case operation::sum: return a + b;
		case operation::multiply: return a * b;
		case operation::minimum: return std::min(a, b);
		case operation::maximum: return std::max(a, b);
		case operation::greaterThan: return int64_t(a > b);
		case operation::lessThan: return int64_t(a < b);
		case operation::equalTo: return int64_t(a == b);
	}
	return 0;
}

int64_t solve_part_two(const inputs &input) {
	size_t takenBits{};
	std::stack<std::tuple<bool, int, operation>> subPackets;
	std::stack<std::optional<int64_t>> values;

	subPackets.emplace(false, 1, operation::maximum);
	values.emplace();

	while(!subPackets.empty()) {
		auto &[bits, end, op] = subPackets.top();

		if(bits) {
			if(takenBits >= end) {
				auto newValue = *values.top();
				values.pop();
				while(values.top()) {
					newValue = execute_operation(*values.top(), newValue, op);
					values.pop();

				}
				values.top() = newValue;
				subPackets.pop();
				continue;

			}
		} else if(end > 0) {
			end--;

		} else {
			auto newValue = *values.top();
			values.pop();
			while(values.top()) {
				newValue = execute_operation(*values.top(), newValue, op);
				values.pop();

			}
			values.top() = newValue;
			subPackets.pop();
			continue;
		}

		take_bits(input, takenBits, 3);

		auto T = static_cast<operation>(take_bits(input, takenBits, 3));
		if(T == operation::value) {
			int64_t value{};
			int64_t newBits{};
			do {
				newBits = take_bits(input, takenBits, 5);
				value <<= 4;
				value |= newBits & 0xF;

			} while(newBits & 0x10);
			values.emplace(value);

		} else {
			auto I = take_bits(input, takenBits, 1);
			values.emplace();
			if(I == 0) {
				auto L = take_bits(input, takenBits, 15);
				subPackets.emplace(true, takenBits + L, T);

			} else {
				auto L = take_bits(input, takenBits, 11);
				subPackets.emplace(false, L, T);
			}
		}
	}

	return *values.top();
}