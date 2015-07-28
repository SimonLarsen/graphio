#ifndef GRAPH_XGMML_HPP
#define GRAPH_XGMML_HPP

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace graph {
	template<class Graph>
	inline void readXGMMLFile(const std::string &filename, Graph &g) {
		typedef typename boost::graph_traits<Graph>::vertex_descriptor V;
		boost::property_tree::ptree pt;
		std::map<int,V> map;

		boost::property_tree::read_xml(filename, pt);
		V n = 0;
		for(auto &c : pt.get_child("graph")) {
			if(c.first == "node") {
				int id = c.second.get<int>("<xmlattr>.id");
				map[id] = n++;
			}
		}

		g = Graph(n);

		for(auto &c : pt.get_child("graph")) {
			if(c.first == "node") {
				int id = c.second.get<int>("<xmlattr>.id");
				V v = map[id];
				std::string label = c.second.get<std::string>("<xmlattr>.label");
				g[v].label = label;
			}
			else if(c.first == "edge") {
				int source = c.second.get<int>("<xmlattr>.source");
				int target = c.second.get<int>("<xmlattr>.target");
				V u = map[source];
				V v = map[target];
				add_edge(u, v, g);
			}
		}
	}

	template<class Graph>
	inline void writeXGMMLFile(const Graph &g, const std::string &filename) {
		std::ofstream file(filename);
		
		file << "<?xml version=\"1.0\"?>\n";

		file << "<graph label=\"" << filename << "\" ";
		file << "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" ";
		file << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
		file << "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" ";
		file << "xmlns=\"http://www.cs.rpi.edu/XGMML\" ";
		file << "directed=\"0\">\n";

		for(size_t i = 0; i < num_vertices(g); ++i) {
			file << "\t<node id=\"" << i+1 << "\" label=\"" << g[i].label << "\" />\n";
		}

		for(size_t i = 0; i < num_vertices(g); ++i) {
			for(size_t j = i+1; j < num_vertices(g); ++j) {
				if(has_edge(i, j, g)) {
					file << "\t<edge source=\"" << i+1 << "\" target=\"" << j+1 << "\" label=\"\">\n";
					file << "\t\t<att name=\"exceptions\" type=\"integer\" value=\"";
					file << g[edge(i, j, g).first].weight << "\" />\n";
					file << "\t</edge>\n";
				}
			}
		}
		file << "</graph>\n";
	}
}

#endif
