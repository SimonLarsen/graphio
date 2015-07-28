#ifndef GRAPH_SIF_HPP
#define GRAPH_SIF_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>

namespace graph {
	template<class Graph>
	inline void readSIFFile(const std::string &filename, Graph &g) {
		std::ifstream file;
		std::string line;
		std::vector<std::string> parts;
		std::map<std::string, int> map;

		// Map vertex labels to ids
		int id = 0;
		std::map<std::string, int>::iterator it1, it2;
		file.open(filename);
		while(std::getline(file, line)) {
			boost::split(parts, line, boost::is_any_of(" \t"));
			it1 = map.find(parts[0]);
			it2 = map.find(parts[2]);

			if(it1 == map.end()) {
				map[parts[0]] = id;
				id++;
			}
			if(it2 == map.end()) {
				map[parts[2]] = id;
				id++;
			}
		}

		g = Graph(map.size());
		
		// Set vertex labels
		for(auto it = map.begin(); it != map.end(); ++it) {
			g[it->second].label = it->first;
		}

		// Add edges
		file.close();
		file.open(filename);
		while(std::getline(file, line)) {
			boost::split(parts, line, boost::is_any_of(" \t"));
			int id1 = map[parts[0]];
			int id2 = map[parts[2]];

			add_edge(id1, id2, g);
		}
	}

	template<class Graph>
	inline void writeSIFFile(const Graph &g, const std::string &filename) {
		std::ofstream file(filename);

		for(size_t i = 0; i < num_vertices(g); ++i) {
			for(size_t j = i+1; j < num_vertices(g); ++j) {
				if(has_edge(i, j, g)) {
					file << g[i].label << " ? " << g[j].label << "\n";
				}
			}
		}

		for(size_t i = 0; i < num_vertices(g); ++i) {
			if(out_degree(i, g) == 0) {
				file << g[i].label << "\n";
			}
		}
	}
}

#endif
