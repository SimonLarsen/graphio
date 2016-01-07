#ifndef GRAPHIO_GRAPHWRITER_HPP
#define GRAPHIO_GRAPHWRITER_HPP

#include <string>
#include <graphio/GraphIOException.hpp>
#include <graphio/GraphTypes.hpp>
#include <graphio/VertexVisitor.hpp>
#include <graphio/EdgeVisitor.hpp>
#include <graphio/formats/LEDA.hpp>
#include <graphio/formats/SIF.hpp>
#include <graphio/formats/XGMML.hpp>
#include <graphio/formats/Tab.hpp>

namespace graphio {
	template<typename G>
	inline void writeGraph(
		const G &g,
		const std::string &filename
	) {
		VertexVisitor vv;
		EdgeVisitor ev;
		writeGraph(g, filename, vv, ev);
	}
	
	template<typename G, typename VV, typename EV>
	inline void writeGraph(
		const G &g,
		const std::string &filename,
		const VV &vv,
		const EV &ev
	) {
		Type type = graphFileType(filename);

		switch(type) {
			case LEDA:
				writeLEDAFile(g, filename);
				break;
			case SIF:
				writeSIFFile(g, filename);
				break;
			case XGMML:
				writeXGMMLFile(g, filename, vv, ev);
				break;
			case Tab:
				writeTabFile(g, filename, vv, ev);
				break;
			default:
				throw GraphIOException("Unknown filetype for file: " + filename);
		}
	}
}

#endif
