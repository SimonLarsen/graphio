#ifndef GRAPHIO_FORMATS_SIF_HPP
#define GRAPHIO_FORMATS_SIF_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/graph/graph_traits.hpp>
#include <graphio/utility/basename.hpp>
#include <graphio/utility/split.hpp>
#include <graphio/GraphIOException.hpp>

namespace graphio {
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

		if(!file.good()) {
			throw GraphIOException(std::string("Could not open file: ") + filename);
		}

		while(std::getline(file, line)) {
			if(line.length() == 0) continue;
			escaped_split(line, " \t", parts);

			for(size_t i = 0; i  < parts.size(); ++i) {
				if(i == 1) continue;

				it1 = map.find(parts[i]);
				if(it1 == map.end()) {
					map[parts[i]] = id;
					id++;
				}
			}
		}

		g = G(map.size());
		g[boost::graph_bundle].label = basename(filename);

		// Set vertex labels
		for(auto it = map.begin(); it != map.end(); ++it) {
			g[it->second].label = it->first;
		}

		// Add edges
		file.close();
		file.open(filename);
		while(std::getline(file, line)) {
			if(line.length() == 0) continue;
			escaped_split(line, " \t", parts);

			if(parts.size() < 3) continue;

			int id1 = map[parts[0]];
			for(size_t i = 2; i < parts.size(); ++i) {
				int id2 = map[parts[i]];
				auto e = add_edge(id1, id2, g);
				g[e.first].label = parts[1];
			}
		}
	}

	template<class G>
	inline void writeSIFFile(const G &g, const std::string &filename) {
		std::ofstream file(filename);
		if(!file.good()) {
			throw GraphIOException(std::string("Could not open file: ") + filename);
		}

		for(size_t i = 0; i < num_vertices(g); ++i) {
			for(auto it = out_edges(i, g); it.first != it.second; ++it.first) {
				size_t j = target(*it.first, g);

				if(i <= j) {
					file << g[i].label << " ";
					if(g[*it.first].label.length() > 0) {
						file << g[*it.first].label;
					} else {
						file << "?";
					}
					file << " " << g[j].label << "\n";
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
