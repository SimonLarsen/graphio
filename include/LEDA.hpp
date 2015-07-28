#ifndef GRAPH_LEDA_HPP
#define GRAPH_LEDA_HPP

#include <string>
#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <graph/Graph.hpp>
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

	template<class Graph>
	inline void readLEDAFile(const std::string &filename, Graph &g) {
		std::string line;
		size_t n, m;
		std::ifstream file(filename);

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

		g = Graph(n);

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
			add_edge(u, v, g);
		}
	}

	template<class Graph>
	inline void writeLEDAFile(const Graph &g, const std::string &filename) {
		std::ofstream file(filename);

		file << "LEDA.GRAPH" << std::endl;
		file << "string" << std::endl;
		file << "string" << std::endl;
		file << "-2" << std::endl;

		file << num_vertices(g) << std::endl;
		for(size_t i = 0; i < num_vertices(g); ++i) {
			file << "|{" << g[i].label << "}|" << std::endl;
		}

		file << num_edges(g) << std::endl;
		for(size_t i = 0; i < num_vertices(g); ++i) {
			for(size_t j = i+1; j < num_vertices(g); ++j) {
				if(has_edge(i, j, g)) {
					file << i+1 << " " << j+1 << " 0 |{}|\n";
				}
			}
		}
	}
}

#endif
