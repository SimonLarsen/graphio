#ifndef GRAPHIO_FORMATS_XGMML_HPP
#define GRAPHIO_FORMATS_XGMML_HPP

#include <string>
#include <map>
#include <boost/format.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <graphio/utility/basename.hpp>
#include <graphio/GraphIOException.hpp>

namespace graphio {
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
		g[boost::graph_bundle].label = pt.get<std::string>("graph.<xmlattr>.label");

		for(auto &c : pt.get_child("graph")) {
			if(c.first == "node") {
				int id = c.second.get<int>("<xmlattr>.id");
				V v = map[id];
				g[v].label = c.second.get<std::string>("<xmlattr>.label");
			}
			else if(c.first == "edge") {
				int source = c.second.get<int>("<xmlattr>.source");
				int target = c.second.get<int>("<xmlattr>.target");
				V u = map[source];
				V v = map[target];
				auto e = add_edge(u, v, g);
				g[e.first].label = c.second.get<std::string>("<xmlattr>.label");
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
		if(!file.good()) {
			throw GraphIOException(std::string("Could not open file: ") + filename);
		}
		
		file << "<?xml version=\"1.0\"?>\n";

		if(g[boost::graph_bundle].label.size() > 0) {
			file << "<graph label=\"" << g[boost::graph_bundle].label << "\" ";
		} else {
			file << "<graph label=\"" << basename(filename) << "\" ";
		}
		file << "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" ";
		file << "xmlns:xlink=\"http://www.w3.org/1999/xlink\" ";
		file << "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" ";
		file << "xmlns=\"http://www.cs.rpi.edu/XGMML\" ";
		file << "directed=\"0\">\n";

		for(size_t i = 0; i < num_vertices(g); ++i) {
			file << format("\t<node id=\"%d\" label=\"%s\">\n") % (i+1) % g[i].label;
			for(size_t a = 0; a < vv.count(); ++a) {
				file << format("\t\t<att name=\"%s\" type=\"%s\" value=\"%s\"/>\n")
					% vv.name(a) % vv.type(a) % vv.value_str(g[i], a);
			}
			file << "\t</node>\n";
		}

		for(size_t i = 0; i < num_vertices(g); ++i) {
			for(auto it = out_edges(i, g); it.first != it.second; ++it.first) {
				size_t j = target(*it.first, g);

				if(i <= j) {
					file << format("\t<edge source=\"%d\" target=\"%d\" label=\"%s\">\n")
						% (i+1) % (j+1) % g[*it.first].label;

					for(size_t a = 0; a < ev.count(); ++a) {
						file << format("\t\t<att name=\"%s\" type=\"%s\" value=\"%s\" />\n")
							% ev.name(a) % ev.type(a) % ev.value_str(g[*it.first], a);
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
