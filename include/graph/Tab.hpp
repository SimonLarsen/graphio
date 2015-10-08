#ifndef GRAPH_TAB_HPP
#define GRAPH_TAB_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/graph/graph_traits.hpp>
#include <boost/algorithm/string.hpp>
#include <graph/utility/basename.hpp>

namespace graph {
	template<class G>
	inline void readTabFile(const std::string &filename, G &g) {
		std::ifstream file;
		std::string line;
		std::vector<std::string> parts;
		std::map<std::string, int> map;

		file.open(filename);
		if(!file.good()) {
			throw GraphException(std::string("Could not open file: ") + filename);
		}

		// Skip header line
		std::getline(file, line);

		// Map vertex labels to ids
		int id = 0;
		std::map<std::string, int>::iterator it1, it2;
		while(std::getline(file, line)) {
			if(line.length() == 0) continue;
			boost::split(parts, line, boost::is_any_of("\t"));

			it1 = map.find(parts[0]);
			it2 = map.find(parts[1]);

			if(it1 == map.end()) {
				map[parts[0]] = id;
				id++;
			}
			else if(it2 == map.end()) {
				map[parts[1]] = id;
				id++;
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
		std::getline(file, line);
		while(std::getline(file, line)) {
			if(line.length() == 0) continue;
			boost::split(parts, line, boost::is_any_of("\t"));

			int id1 = map[parts[0]];
			int id2 = map[parts[1]];

			auto e = add_edge(id1, id2, g);

			if(parts.size() > 2) {
				g[e.first].label = parts[2];
			}
		}
		file.close();
	}

	template<class G, typename VV, typename EV>
	inline void writeTabFile(
			const G &g,
			const std::string &filename,
			const VV &vv,
			const EV &ev
		) {
		std::ofstream file(filename);
		if(!file.good()) {
			throw GraphException(std::string("Could not open file: ") + filename);
		}

		file << "INTERACTOR_A\tINTERACTOR_B\tlabel";
		for(size_t a = 0; a < ev.count(); ++a) {
			file << "\t" << ev.name(a);
		}
		file << "\n";

		for(size_t i = 0; i < num_vertices(g); ++i) {
			for(auto it = out_edges(i, g); it.first != it.second; ++it.first) {
				size_t j = target(*it.first, g);

				if(i <= j) {
					file << g[i].label << "\t" << g[j].label;
					if(g[*it.first].label.length() > 0) {
						file << "\t" << g[*it.first].label;
					} else {
						file << "\tNA";
					}
					for(size_t a = 0; a < ev.count(); ++a) {
						file << "\t" << ev.value_str(g[*it.first], a);
					}
					file << "\n";
				}
			}
		}
		file.close();
	}
}

#endif
