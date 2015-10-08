#ifndef GRAPH_LEDA_HPP
#define GRAPH_LEDA_HPP

#include <string>
#include <fstream>
#include <vector>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/graph_traits.hpp>
#include <graph/utility/basename.hpp>
#include <graph/GraphException.hpp>

namespace graph {
	namespace {
		inline std::istream &LEDAReadLine(std::istream &is, std::string &str) {
			do {
				std::getline(is, str);
				boost::algorithm::trim(str);
			} while(str[0] == '#' || str.length() == 0);

			return is;
		}
	}

	template<class G>
	inline void readLEDAFile(const std::string &filename, G &g) {
		std::string line;
		size_t n, m;
		std::ifstream file(filename);
		if(!file.good()) {
			throw GraphException(std::string("Could not open file: ") + filename);
		}

		if(file.is_open() == false) {
			throw GraphException("Could not open file: " + filename);
		}

		// Look for header string
		LEDAReadLine(file, line);
		if(boost::algorithm::equals(line, "LEDA.GRAPH") == false) {
			throw GraphException("\"LEDA.GRAPH\" header not found");
		}

 		// Node type
		LEDAReadLine(file, line);
		// Edge type
		LEDAReadLine(file, line);
		// Directed/undirected
		LEDAReadLine(file, line);

		// Node count
		LEDAReadLine(file, line);
		n = boost::lexical_cast<size_t>(line);

		g = G(n);
		g[boost::graph_bundle].label = basename(filename);

		// Read nodes
		for(size_t i = 0; i < n; ++i) {
			LEDAReadLine(file, line);

			size_t begin = line.find("|{") + 2;
			size_t end = line.find("}|");

			if(begin == std::string::npos || end == std::string::npos || end <= begin) {
				throw GraphException("Malformed node label: " + line);
			}

			g[i].label = line.substr(begin, end-begin);
		}

		// Edge count
		LEDAReadLine(file, line);
		m = boost::lexical_cast<size_t>(line);

		// Read edges
		for(size_t i = 0; i < m; ++i) {
			LEDAReadLine(file, line);

			std::vector<std::string> parts;
			boost::split(parts, line, boost::is_any_of(" \t"));
			int u = boost::lexical_cast<int>(parts[0]) - 1;
			int v = boost::lexical_cast<int>(parts[1]) - 1;

			size_t begin = parts[3].find("|{") + 2;
			size_t end = parts[3].find("}|");

			auto e = add_edge(u, v, g);
			g[e.first].label = parts[3].substr(begin, end-begin);

		}
	}

	template<class G>
	inline void writeLEDAFile(const G &g, const std::string &filename) {
		using boost::format;

		std::ofstream file(filename);
		if(!file.good()) {
			throw GraphException(std::string("Could not open file: ") + filename);
		}

		file << "LEDA.GRAPH" << std::endl;
		file << "string" << std::endl;
		file << "string" << std::endl;
		file << "-2" << std::endl;

		file << num_vertices(g) << std::endl;
		for(size_t i = 0; i < num_vertices(g); ++i) {
			file << format("|{%s}|\n") % g[i].label;
		}

		file << num_edges(g) << std::endl;

		for(size_t i = 0; i < num_vertices(g); ++i) {
			for(auto it = out_edges(i, g); it.first != it.second; ++it.first) {
				size_t j = target(*it.first, g);

				if(i <= j) {
					file << format("%d %d 0 |{%s}|\n")
						% (i+1) % (j+1) % g[*it.first].label;
				}
			}
		}
	}
}

#endif
