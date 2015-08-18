#ifndef GRAPH_XGMML_HPP
#define GRAPH_XGMML_HPP

#include <string>
#include <map>
#include <boost/format.hpp>
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
				g.node(v).label = c.second.get<std::string>("<xmlattr>.label");
			}
			else if(c.first == "edge") {
				int source = c.second.get<int>("<xmlattr>.source");
				int target = c.second.get<int>("<xmlattr>.target");
				V u = map[source];
				V v = map[target];
				auto e = g.addEdge(u, v);
				g.edge(e).label = c.second.get<std::string>("<xmlattr>.label");
			}
		}
	}

	template<class G, typename VV, typename EV>
	inline void writeXGMMLFile(
		const G &g,
		const std::string &filename,
		const VV &vv,
		const EV &ev
	) {
		using boost::format;

		std::ofstream file(filename);
		
		file << "<?xml version=\"1.0\"?>\n";

		file << "<graph label=\"" << filename << "\" ";
		file << "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" ";
		file << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
		file << "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" ";
		file << "xmlns=\"http://www.cs.rpi.edu/XGMML\" ";
		file << "directed=\"0\">\n";

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			file << format("\t<node id=\"%d\" label=\"%s\">\n") % (i+1) % g.node(i).label;
			for(size_t a = 0; a < vv.count(); ++a) {
				file << format("\t\t<att name=\"%s\" type=\"%s\" value=\"%s\"/>\n")
					% vv.name(a) % vv.type(a) % vv.value_str(g.vertex(i), a);
			}
			file << "\t</node>\n";
		}

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			for(auto it = boost::out_edges(i, g.graph()); it.first != it.second; ++it.first) {
				size_t j = target(*it.first, g.graph());

				if(i <= j) {
					file << format("\t<edge source=\"%d\" target=\"%d\" label=\"%s\">\n")
						% (i+1) % (j+1) % g.edge(*it.first).label;

					for(size_t a = 0; a < ev.count(); ++a) {
						file << format("\t\t<att name=\"%s\" type=\"%s\" value=\"%s\" />\n")
							% ev.name(a) % ev.type(a) % ev.value_str(g.edge(*it.first), a);
					}
					file << "\t</edge>\n";
				}
			}
		}
		file << "</graph>";
		file.close();
	}
}

#endif
