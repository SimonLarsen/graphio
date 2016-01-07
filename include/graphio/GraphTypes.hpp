#ifndef GRAPHIO_GRAPHTYPES_HPP
#define GRAPHIO_GRAPHTYPES_HPP

#include <string>
#include <boost/algorithm/string/predicate.hpp>

namespace graphio {
	enum Type {
		NONE,
		LEDA,
		SIF,
		XGMML,
		Tab
	};

	inline Type graphFileType(const std::string &filename) {
		if(boost::algorithm::iends_with(filename, ".gw")
		|| boost::algorithm::iends_with(filename, ".leda")) {
			return Type::LEDA;
		}
		if(boost::algorithm::iends_with(filename, ".sif")) {
			return Type::SIF;
		}
		if(boost::algorithm::iends_with(filename, ".xgmml")) {
			return Type::XGMML;
		}
		if(boost::algorithm::iends_with(filename, ".tab")) {
			return Type::Tab;
		}

		return Type::NONE;
	}
}

#endif
