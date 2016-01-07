#ifndef GRAPHIO_GRAPHREADER_HPP
#define GRAPHIO_GRAPHREADER_HPP

#include <string>
#include <graphio/GraphIOException.hpp>
#include <graphio/GraphTypes.hpp>
#include <graphio/formats/LEDA.hpp>
#include <graphio/formats/SIF.hpp>
#include <graphio/formats/XGMML.hpp>
#include <graphio/formats/Tab.hpp>

namespace graphio {
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
				throw GraphIOException("Unknown filetype for file: " + filename);
		}
	}
}

#endif
