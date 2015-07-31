#ifndef GRAPH_GRAPHREADER_HPP
#define GRAPH_GRAPHREADER_HPP

#include <string>
#include <graph/GraphException.hpp>
#include <graph/GraphTypes.hpp>
#include <graph/LEDA.hpp>
#include <graph/SIF.hpp>
#include <graph/XGMML.hpp>
#include <graph/Tab.hpp>

namespace graph {
	template<typename G>
	inline void readGraph(const std::string &filename, G &g) {
		Type type = graphFileType(filename);

		switch(type) {
			case LEDA:
				readLEDAFile(filename, g);
				break;
			case SIF:
				readSIFFile(filename, g);
				break;
			case XGMML:
				readXGMMLFile(filename, g);
				break;
			case Tab:
				readTabFile(filename, g);
				break;
			default:
				throw GraphException("Unknown filetype for file: " + filename);
		}
	}
}

#endif
