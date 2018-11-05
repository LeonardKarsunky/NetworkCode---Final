#include "network.h"
#include "random.h"
#include <iterator>
#include <algorithm>
#include <iostream>

void Network::resize(const size_t& n) {
	values.resize(n);
	RandomNumbers RNG;
	for (size_t i(0); i < n; ++i) {
		values[i] = RNG.normal(0,1);
	}
}

bool Network::add_link(const size_t& a, const size_t& b) {
	if ((a < values.size()) and (b < values.size()) and (a != b)) {
		for (auto nb : neighbors(a)) {
			if (nb == b) return false;
		}
		links.insert(std::pair<size_t, size_t>(a, b));
		links.insert(std::pair<size_t, size_t>(b, a));
		return true;
	}
	return false;
}

size_t Network::random_connect(const double& mean_deg) {
	links.clear();
	RandomNumbers RNG;
	
	for (size_t n(0); n < values.size(); ++n) {
		size_t tmp(RNG.poisson(mean_deg));
		for (size_t i(0); i < tmp; ++i) {
			size_t random_node(RNG.uniform_double(0, values.size()-1));
			while (not add_link(n, random_node)) {
				random_node = RNG.uniform_double(0, values.size()-1);
			}
		}
	}
	return links.size()/2;
}

size_t Network::set_values(const std::vector<double>& new_values) {
	size_t old_size(values.size());
	size_t new_size(new_values.size());
	
	if (new_size <= old_size) {
		for (size_t i(0); i < new_size; ++i) {
			values[i] = new_values[i];
		}
		return new_size;
	} else {
		for (size_t i(0); i < old_size; ++i) {
			values[i] = new_values[i];
		}
		return old_size;
	}
}

size_t Network::size() const {
	return values.size();
}

size_t Network::degree(const size_t &n) const {
	size_t nb_degrees(0);
	auto result = links.equal_range(n);
	for (auto it(result.first); it != result.second; it++) {
		++nb_degrees;
	}
	return nb_degrees;
}

double Network::value(const size_t &n) const {
	return values[n];
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> tmp = values;
	std::sort (tmp.begin(), tmp.end());
	std::reverse (tmp.begin(), tmp.end());
	return tmp;
}

std::vector<size_t> Network::neighbors(const size_t& n) const {
	std::vector<size_t> neighbors;
	auto result = links.equal_range(n);
		for (auto it(result.first); it != result.second; it++) {
			neighbors.push_back(it->second);
		}
	return neighbors;
}
	
