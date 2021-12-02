#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <algorithm>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <tuple>
#include <string>
#include <stack>
#include <chrono>
#include <fstream>
#include <iterator>
#include <iostream>
#include <variant>
#include <optional>
#include <regex>
#include <queue>
#include <numeric>
#include <sstream>
#include <bitset>

inline std::vector<std::string_view> split(std::string_view string, const std::string_view separator) {
	std::vector<std::string_view> substrings;

	while(true) {
		const auto substringIndex = string.find(separator);
		auto substring = string.substr(0, substringIndex);

		const auto lastNonSpaceIndex = substring.find_last_not_of(' ');
		const auto stringSize = substring.size();
		substring.remove_suffix(stringSize - lastNonSpaceIndex - 1);

		if(!substring.empty()) substrings.emplace_back(substring);

		string.remove_prefix(substringIndex + separator.size());
		string = string.substr(string.find_first_not_of(' '));

		if(substringIndex == std::string_view::npos) break;

	}

	return substrings;
}

struct hash_pair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2> &p) const {
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
		return hash1;
	}
};

struct hash3array {
	template <class T>
	size_t operator()(const std::array<T, 3> &p) const {
		auto hash1 = std::hash<T>{}(p[0]);
		auto hash2 = std::hash<T>{}(p[1]);
		auto hash3 = std::hash<T>{}(p[2]);
		hash1 ^= hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
		hash1 ^= hash3 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2);
		return hash1;
	}
};

template<typename Iterator, typename Value, typename Member>
Iterator find_member(Iterator begin, Iterator end, const Value &value, const Member member) {
	return std::find_if(
		begin,
		end,
		[&](const auto &element) {
		return element.*member == value;
	}
	);
}

#endif