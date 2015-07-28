#ifndef GRAPH_TAB_HPP
#define GRAPH_TAB_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>

namespace graph {
	template<class Graph>
	inline void readTabFile(const std::string &filename, Graph &g) {
		std::ifstream file;
		std::string line;
		std::vector<std::string> parts;
		std::map<std::string, int> map;

		file.open(filename);

		// Skip header line
		std::getline(file, line);

		// Map vertex labels to ids
		int id = 0;
		std::map<std::string, int>::iterator it1, it2;
		while(std::getline(file, line)) {
			boost::split(parts, line, boost::is_any_of("\t"));
			it1 = map.find(parts[0]);
			it2 = map.find(parts[1]);

			if(it1 == map.end()) {
				map[parts[0]] = id;
				id++;
			}
			if(it2 == map.end()) {
				map[parts[1]] = id;
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
		std::getline(file, line);
		while(std::getline(file, line)) {
			boost::split(parts, line, boost::is_any_of("\t"));
			int id1 = map[parts[0]];
			int id2 = map[parts[1]];

			add_edge(id1, id2, g);
		}
	}

	template<class Graph>
	inline void writeTabFile(const Graph &g, const std::string &filename) {
		std::ofstream file(filename);

		file << "INTERACTOR_A\tINTERACTOR_B\n";
		for(size_t i = 0; i < num_vertices(g); ++i) {
			for(size_t j = i+1; j < num_vertices(g); ++j) {
				if(has_edge(i, j, g)) {
					file << g[i].label << "\t" << g[j].label << "\n";
				}
			}
		}
	}
}

#endif
