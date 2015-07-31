#ifndef GRAPH_GRAPHWRITER_HPP
#define GRAPH_GRAPHWRITER_HPP

#include <string>
#include <graph/LEDA.hpp>
#include <graph/SIF.hpp>
#include <graph/XGMML.hpp>
#include <graph/Tab.hpp>
#include <graph/GraphException.hpp>
#include <graph/GraphTypes.hpp>

namespace graph {
	template<typename G>
	inline void writeGraph(const G &g, const std::string &filename) {
		Type type = graphFileType(filename);

		switch(type) {
			case LEDA:
				writeLEDAFile(g, filename);
				break;
			case SIF:
				writeSIFFile(g, filename);
				break;
			case XGMML:
				writeXGMMLFile(g, filename);
				break;
			case Tab:
				writeTabFile(g, filename);
				break;
			default:
				throw GraphException("Unknown filetype for file: " + filename);
		}
	}
}

#endif
