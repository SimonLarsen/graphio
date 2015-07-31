#ifndef GRAPH_XGMML_HPP
#define GRAPH_XGMML_HPP

#include <string>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace graph {
	template<class G>
	inline void readXGMMLFile(const std::string &filename, G &g) {
		typedef typename G::vertex_descriptor V;
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

		g = G(n);

		for(auto &c : pt.get_child("graph")) {
			if(c.first == "node") {
				int id = c.second.get<int>("<xmlattr>.id");
				V v = map[id];
				std::string label = c.second.get<std::string>("<xmlattr>.label");
				g.node(v).label = label;
			}
			else if(c.first == "edge") {
				int source = c.second.get<int>("<xmlattr>.source");
				int target = c.second.get<int>("<xmlattr>.target");
				V u = map[source];
				V v = map[target];
				g.addEdge(u, v);
			}
		}
	}

	template<class G>
	inline void writeXGMMLFile(const G &g, const std::string &filename) {
		std::ofstream file(filename);
		
		file << "<?xml version=\"1.0\"?>\n";

		file << "<graph label=\"" << filename << "\" ";
		file << "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" ";
		file << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
		file << "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" ";
		file << "xmlns=\"http://www.cs.rpi.edu/XGMML\" ";
		file << "directed=\"0\">\n";

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			file << "\t<node id=\"" << i+1 << "\" label=\"" << g.node(i).label << "\" />\n";
		}

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			for(size_t j = i+1; j < g.vertexCount(); ++j) {
				if(g.hasEdge(i, j)) {
					file << "\t<edge source=\"" << i+1 << "\" target=\"" << j+1 << "\" label=\"\">\n";
					file << "\t\t<att name=\"exceptions\" type=\"integer\" value=\"";
					file << g.edge(i, j).weight << "\" />\n";
					file << "\t</edge>\n";
				}
			}
		}
		file << "</graph>\n";
	}
}

#endif
