#ifndef GRAPH_SIF_HPP
#define GRAPH_SIF_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>

namespace graph {
	template<class G>
	inline void readSIFFile(const std::string &filename, G &g) {
		std::ifstream file;
		std::string line;
		std::vector<std::string> parts;
		std::map<std::string, int> map;

		// Map vertex labels to ids
		int id = 0;
		std::map<std::string, int>::iterator it1, it2;
		file.open(filename);
		while(std::getline(file, line)) {
			if(line.length() == 0) continue;
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

		g = G(map.size());
		
		// Set vertex labels
		for(auto it = map.begin(); it != map.end(); ++it) {
			g.node(it->second).label = it->first;
		}

		// Add edges
		file.close();
		file.open(filename);
		while(std::getline(file, line)) {
			if(line.length() == 0) continue;
			boost::split(parts, line, boost::is_any_of(" \t"));

			int id1 = map[parts[0]];
			int id2 = map[parts[2]];

			g.addEdge(id1, id2);
			g.edge(id1, id2).label = parts[1];
		}
	}

	template<class G>
	inline void writeSIFFile(const G &g, const std::string &filename) {
		std::ofstream file(filename);

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			for(auto it = g.getAdjacent(i); it.first != it.second; ++it.first) {
				size_t j = *it.first;
				if(i <= j) {
					file << g.node(i).label << " ";
					if(g.edge(i, j).label.length() > 0) {
						file << g.edge(i, j).label;
					} else {
						file << "?";
					}
					file << " " << g.node(j).label << "\n";
				}
			}
		}

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			if(g.degree(i) == 0) {
				file << g.node(i).label << "\n";
			}
		}
	}
}

#endif
