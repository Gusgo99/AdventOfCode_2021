#include "headers.hpp"

struct P3 {
	int64_t x;
	int64_t y;
	int64_t z;
};

struct hashP3 {
	size_t operator()(const P3 &p) const {
		auto hash1 = std::hash<int64_t>{}(p.x);
		auto hash2 = std::hash<int64_t>{}(p.y);
		auto hash3 = std::hash<int64_t>{}(p.z);
		return hash1 ^ hash2 ^ hash3;
	}
};

bool operator<(const P3 &lhs, const P3 &rhs) {
	if(lhs.x < rhs.x) return true;
	else if(lhs.x > rhs.x) return false;
	else if(lhs.y < rhs.y) return true;
	else if(lhs.y > rhs.y) return false;
	else if(lhs.z < rhs.z) return true;
	else if(lhs.z > rhs.z) return false;
	else return false;
}

bool operator!=(const P3 &lhs, const P3 &rhs) {
	return (lhs < rhs) || (rhs < lhs);
}

bool operator==(const P3 &lhs, const P3 &rhs) {
	return !(lhs != rhs);
}

P3 operator-(const P3 &lhs, const P3 &rhs) {
	return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

P3 operator+(const P3 &lhs, const P3 &rhs) {
	return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

template<typename Item, typename Container>
bool is_in(const Item &item, const Container &container) {
	return container.find(item) != container.end();
}

template<typename Item, typename Container>
size_t find_index(const Item &item, const Container &container) {
	return std::find(container.begin(), container.end(), item) - container.begin();
}

int64_t manhattan_distance(const P3 &lhs, const P3 &rhs) {
	return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y) + std::abs(lhs.z - rhs.z);
}

using inputs = std::vector<std::vector<P3>>;

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
	std::fstream inputFile("./resources/Day19_BeaconScanner.txt", std::fstream::in);
	inputs input{};

	if(inputFile.is_open()) {
		std::string line;
		size_t currentScanner{};
		while(std::getline(inputFile, line)) {
			if(line.find("scanner") != std::string::npos) {
				auto scannerID = line.substr(12);
				scannerID = scannerID.substr(0, scannerID.find(" "));
				currentScanner = std::stoull(scannerID);
				if(currentScanner >= input.size()) {
					input.resize(currentScanner + 1);
				}
			} else if(line.empty()) {
			} else {
				auto coords = split(line, ",");
				P3 point{
					std::stoll(coords[0].data()),
					std::stoll(coords[1].data()),
					std::stoll(coords[2].data())
				};

				input[currentScanner].push_back(point);
			}
		}
	}

	return input;
}

std::array<P3, 24> get_representations(const P3 &point) {
	std::array<P3, 24> representations{};
	size_t index{};

	// i.j.k = -1
	representations[index].x = point.x;
	representations[index].y = point.y;
	representations[index].z = point.z;
	index++;

	// i.-j.-k = -1
	representations[index].x = point.x;
	representations[index].y = -point.y;
	representations[index].z = -point.z;
	index++;

	// -i.j.-k = -1
	representations[index].x = -point.x;
	representations[index].y = point.y;
	representations[index].z = -point.z;
	index++;

	// -i.-j.k = -1
	representations[index].x = -point.x;
	representations[index].y = -point.y;
	representations[index].z = point.z;
	index++;

	// -i.k.j = -1
	representations[index].x = -point.x;
	representations[index].y = point.z;
	representations[index].z = point.y;
	index++;

	// i.-k.j = -1
	representations[index].x = point.x;
	representations[index].y = -point.z;
	representations[index].z = point.y;
	index++;

	// i.k.-j = -1
	representations[index].x = point.x;
	representations[index].y = point.z;
	representations[index].z = -point.y;
	index++;

	// -i.-k.-j = -1
	representations[index].x = -point.x;
	representations[index].y = -point.z;
	representations[index].z = -point.y;
	index++;

	// -j.i.k = -1
	representations[index].x = -point.y;
	representations[index].y = point.x;
	representations[index].z = point.z;
	index++;

	// j.-i.k = -1
	representations[index].x = point.y;
	representations[index].y = -point.x;
	representations[index].z = point.z;
	index++;

	// j.i.-k = -1
	representations[index].x = point.y;
	representations[index].y = point.x;
	representations[index].z = -point.z;
	index++;

	// -j.-i.-k = -1
	representations[index].x = -point.y;
	representations[index].y = -point.x;
	representations[index].z = -point.z;
	index++;

	// j.k.i = -1
	representations[index].x = point.y;
	representations[index].y = point.z;
	representations[index].z = point.x;
	index++;

	// j.-k.-i = -1
	representations[index].x = point.y;
	representations[index].y = -point.z;
	representations[index].z = -point.x;
	index++;

	// -j.k.-i = -1
	representations[index].x = -point.y;
	representations[index].y = point.z;
	representations[index].z = -point.x;
	index++;

	// -j.-k.i = -1
	representations[index].x = -point.y;
	representations[index].y = -point.z;
	representations[index].z = point.x;
	index++;

	// k.i.j = -1
	representations[index].x = point.z;
	representations[index].y = point.x;
	representations[index].z = point.y;
	index++;

	// k.-i.-j = -1
	representations[index].x = point.z;
	representations[index].y = -point.x;
	representations[index].z = -point.y;
	index++;

	// -k.i.-j = -1
	representations[index].x = -point.z;
	representations[index].y = point.x;
	representations[index].z = -point.y;
	index++;

	// -k.-i.j = -1
	representations[index].x = -point.z;
	representations[index].y = -point.x;
	representations[index].z = point.y;
	index++;

	// -k.j.i = -1
	representations[index].x = -point.z;
	representations[index].y = point.y;
	representations[index].z = point.x;
	index++;

	// k.-j.i = -1
	representations[index].x = point.z;
	representations[index].y = -point.y;
	representations[index].z = point.x;
	index++;

	// k.j.-i = -1
	representations[index].x = point.z;
	representations[index].y = point.y;
	representations[index].z = -point.x;
	index++;

	// -k.-j.-i = -1
	representations[index].x = -point.z;
	representations[index].y = -point.y;
	representations[index].z = -point.x;
	index++;

	return representations;
}

P3 get_standard_representation(P3 point) {
	point.x = std::abs(point.x);
	point.y = std::abs(point.y);
	point.z = std::abs(point.z);

	if(point.x > point.y) std::swap(point.x, point.y);
	if(point.y > point.z) std::swap(point.y, point.z);
	if(point.x > point.y) std::swap(point.x, point.y);

	return point;
}

template<typename T>
T get_repeated_element(const std::pair<T, T> &lhs, const std::pair<T, T> &rhs) {
	if(lhs.first == rhs.first) return lhs.first;
	if(lhs.first == rhs.second) return lhs.first;

	return lhs.second;
}

template<typename T>
T get_other_element(const std::pair<T, T> &pair, const T &element) {
	if(pair.first == element) return pair.second;

	return pair.first;
}

int64_t solve_part_one(const inputs &input) {
	std::unordered_set<P3, hashP3> beacons{input.front().begin(), input.front().end()};
	std::vector<std::optional<P3>> scanners(input.size());
	scanners[0] = P3{0, 0, 0};
	size_t scannersFound = 1;

	std::unordered_map<P3, std::pair<P3, P3>, hashP3> relativePositionsToBeaconPositions{};
	std::vector<std::unordered_set<P3, hashP3>> scannersToRelativePositions(input.size());
	std::vector<std::unordered_map<P3, std::pair<size_t, size_t>, hashP3>> relativePositionsToBeaconIndexes(input.size());

	for(auto &i : beacons) {
		for(auto &j : beacons) {
			if(i == j) continue;
			const auto relativePosition = get_standard_representation(i - j);
			relativePositionsToBeaconPositions[relativePosition] = std::pair{i, j};
		}
	}

	for(size_t i = 0; i < input.size(); i++) {
		for(size_t j = 0; j < input[i].size(); j++) {
			for(size_t k = j + 1; k < input[i].size(); k++) {
				auto representation = get_standard_representation(input[i][j] - input[i][k]);
				scannersToRelativePositions[i].emplace(representation);
				relativePositionsToBeaconIndexes[i][representation] = std::pair{j, k};
			}
		}
	}

	while(scannersFound != input.size()) {
		for(size_t i = 0; i < input.size(); i++) {
			if(scanners[i]) continue;

			std::vector<size_t> beaconIndexes{};

			for(auto &relativePosition : scannersToRelativePositions[i]) {
				if(is_in(relativePosition, relativePositionsToBeaconPositions)) {
					auto &matchingIndexes = relativePositionsToBeaconIndexes[i][relativePosition];
					beaconIndexes.emplace_back(matchingIndexes.first);
					beaconIndexes.emplace_back(matchingIndexes.second);
				}
			}
			std::sort(beaconIndexes.begin(), beaconIndexes.end());
			auto newEnd = std::unique(beaconIndexes.begin(), beaconIndexes.end());
			beaconIndexes.erase(newEnd, beaconIndexes.end());
			if(beaconIndexes.size() >= 12) {
				const auto firstRel = get_standard_representation(input[i][beaconIndexes[0]] - input[i][beaconIndexes[1]]);
				const auto secondRel = get_standard_representation(input[i][beaconIndexes[0]] - input[i][beaconIndexes[2]]);

				const auto &firstAbs = relativePositionsToBeaconPositions[firstRel];
				const auto &secondAbs = relativePositionsToBeaconPositions[secondRel];

				const auto commonPosition = get_repeated_element(firstAbs, secondAbs);
				const auto otherPosition = get_other_element(firstAbs, commonPosition);

				const auto firstRepresentations = get_representations(input[i][beaconIndexes[0]] - input[i][beaconIndexes[1]]);
				const auto diff = commonPosition - otherPosition;
				const auto scannerOrientation = find_index(diff, firstRepresentations);

				const auto scannerPosition = commonPosition - get_representations(input[i][beaconIndexes[0]])[scannerOrientation];
				scanners[i] = scannerPosition;
				scannersFound++;

				for(size_t j = 0; j < input[i].size(); j++) {
					auto beaconPosition = scannerPosition + get_representations(input[i][j])[scannerOrientation];
					for(auto &beacon : beacons) {
						if(manhattan_distance(beaconPosition, beacon) <= 6001) {
							const auto relativePosition = get_standard_representation(beaconPosition - beacon);
							relativePositionsToBeaconPositions[relativePosition] = std::pair{beacon, beaconPosition};
						}
					}
					beacons.emplace(beaconPosition);
				}
			}
		}
	}

	return beacons.size();
}

int64_t solve_part_two(const inputs &input) {
	std::unordered_set<P3, hashP3> beacons{input.front().begin(), input.front().end()};
	std::vector<std::optional<P3>> scanners(input.size());
	scanners[0] = P3{0, 0, 0};
	size_t scannersFound = 1;

	std::unordered_map<P3, std::pair<P3, P3>, hashP3> relativePositionsToBeaconPositions{};
	std::vector<std::unordered_set<P3, hashP3>> scannersToRelativePositions(input.size());
	std::vector<std::unordered_map<P3, std::pair<size_t, size_t>, hashP3>> relativePositionsToBeaconIndexes(input.size());

	for(auto &i : beacons) {
		for(auto &j : beacons) {
			if(i == j) continue;
			const auto relativePosition = get_standard_representation(i - j);
			relativePositionsToBeaconPositions[relativePosition] = std::pair{i, j};
		}
	}

	for(size_t i = 0; i < input.size(); i++) {
		for(size_t j = 0; j < input[i].size(); j++) {
			for(size_t k = j + 1; k < input[i].size(); k++) {
				auto representation = get_standard_representation(input[i][j] - input[i][k]);
				scannersToRelativePositions[i].emplace(representation);
				relativePositionsToBeaconIndexes[i][representation] = std::pair{j, k};
			}
		}
	}

	while(scannersFound != input.size()) {
		for(size_t i = 0; i < input.size(); i++) {
			if(scanners[i]) continue;

			std::vector<size_t> beaconIndexes{};

			for(auto &relativePosition : scannersToRelativePositions[i]) {
				if(is_in(relativePosition, relativePositionsToBeaconPositions)) {
					auto &matchingIndexes = relativePositionsToBeaconIndexes[i][relativePosition];
					beaconIndexes.emplace_back(matchingIndexes.first);
					beaconIndexes.emplace_back(matchingIndexes.second);
				}
			}
			std::sort(beaconIndexes.begin(), beaconIndexes.end());
			auto newEnd = std::unique(beaconIndexes.begin(), beaconIndexes.end());
			beaconIndexes.erase(newEnd, beaconIndexes.end());
			if(beaconIndexes.size() >= 12) {
				const auto firstRel = get_standard_representation(input[i][beaconIndexes[0]] - input[i][beaconIndexes[1]]);
				const auto secondRel = get_standard_representation(input[i][beaconIndexes[0]] - input[i][beaconIndexes[2]]);

				const auto &firstAbs = relativePositionsToBeaconPositions[firstRel];
				const auto &secondAbs = relativePositionsToBeaconPositions[secondRel];

				const auto commonPosition = get_repeated_element(firstAbs, secondAbs);
				const auto otherPosition = get_other_element(firstAbs, commonPosition);

				const auto firstRepresentations = get_representations(input[i][beaconIndexes[0]] - input[i][beaconIndexes[1]]);
				const auto diff = commonPosition - otherPosition;
				const auto scannerOrientation = find_index(diff, firstRepresentations);

				const auto scannerPosition = commonPosition - get_representations(input[i][beaconIndexes[0]])[scannerOrientation];
				scanners[i] = scannerPosition;
				scannersFound++;

				for(size_t j = 0; j < input[i].size(); j++) {
					auto beaconPosition = scannerPosition + get_representations(input[i][j])[scannerOrientation];
					for(auto &beacon : beacons) {
						if(manhattan_distance(beaconPosition, beacon) <= 6001) {
							const auto relativePosition = get_standard_representation(beaconPosition - beacon);
							relativePositionsToBeaconPositions[relativePosition] = std::pair{beacon, beaconPosition};
						}
					}
					beacons.emplace(beaconPosition);
				}
			}
		}
	}

	int64_t maxDistance{};
	for(auto &positionA : scanners) {
		for(auto &positionB : scanners) {
			const auto distance = manhattan_distance(*positionA, *positionB);
			if(distance > maxDistance) maxDistance = distance;
		}
	}

	return maxDistance;
}