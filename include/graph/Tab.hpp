#ifndef GRAPH_TAB_HPP
#define GRAPH_TAB_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/algorithm/string.hpp>

namespace graph {
	template<class G>
	inline void readTabFile(const std::string &filename, G &g) {
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
			if(line.length() == 0) continue;
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

		g = G(map.size());
		
		// Set vertex labels
		for(auto it = map.begin(); it != map.end(); ++it) {
			g.node(it->second).label = it->first;
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

			g.addEdge(id1, id2);

			if(parts.size() > 2) {
				g.edge(id1, id2).label = parts[2];
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

		file << "INTERACTOR_A\tINTERACTOR_B\tlabel";
		for(size_t a = 0; a < ev.count(); ++a) {
			file << "\t" << ev.name(a);
		}
		file << "\n";

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			for(size_t j = i+1; j < g.vertexCount(); ++j) {
				if(g.hasEdge(i, j)) {
					file << g.node(i).label << "\t" << g.node(j).label;
					if(g.edge(i, j).label.length() > 0) {
						file << "\t" << g.edge(i, j).label;
					} else {
						file << "\tNA";
					}
					for(size_t a = 0; a < ev.count(); ++a) {
						file << "\t" << ev.value_str(g.edge(i, j), a);
					}
					file << "\n";
				}
			}
		}
		file.close();
	}
}

#endif
