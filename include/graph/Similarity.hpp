#ifndef GRAPH_SIMILARITY_HPP
#define GRAPH_SIMILARITY_HPP

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <graph/SimilarityMatrix.hpp>
#include <graph/GraphException.hpp>

namespace graph {
	template<class G>
	inline void readSimilarityFile(
		const std::string &filename,
		const G &g1,
		const G &g2,
		SimilarityMatrix &sim,
		float min_value = 0.0f,
		float max_value = 1.0f
	) {
		std::ifstream file(filename);
		if(!file.good()) {
			throw std::runtime_error(
				std::string("Could not open file: ") + filename
			);
		}

		std::string line;
		std::vector<std::string> parts;

		std::map<std::string,int> map1, map2;

		// Make map of all node labels
		for(size_t i = 0; i < num_vertices(g1); ++i) {
			map1[g1[i].label] = i;
		}

		for(size_t i = 0; i < num_vertices(g2); ++i) {
			map2[g2[i].label] = i;
		}

		// Zero similarity matrix
		sim.resize(num_vertices(g1), num_vertices(g2));
		for(auto it1 = sim.begin1(); it1 != sim.end1(); ++it1) {
			std::fill(it1.begin(), it1.end(), 0.0f);
		}

		// Read similarities from file
		while(std::getline(file, line)) {
			boost::split(parts, line, boost::is_any_of(" ,\t"));
			float value = boost::lexical_cast<float>(parts[2]);
			if(value < 0.0f || value > 1.0f) {
				throw GraphException("Invalid similarity. Must be in [0,1]");
			}

			std::map<std::string,int>::iterator it1, it2;

			it1 = map1.find(parts[0]);
			it2 = map2.find(parts[1]);
			if(it1 != map1.end() && it2 != map2.end()) {
				int u = it1->second;
				int v = it2->second;

				sim(u, v) = value;
			}

			it1 = map1.find(parts[1]);
			it2 = map2.find(parts[2]);
			if(it1 != map1.end() && it2 != map2.end()) {
				int u = it1->second;
				int v = it2->second;

				sim(u, v) = value;
			}
		}

		// Cap all values to [0,1]
		for(auto it = sim.begin1(); it != sim.end1(); ++it) {
			std::transform(it.begin(), it.end(), it.begin(),
			[min_value,max_value](float &v) {
				return std::max(std::min(v, max_value), min_value);
			});
		}
	}
}

#endif
