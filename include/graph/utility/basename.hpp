#ifndef GRAPH_UTILITY_BASENAME_HPP
#define GRAPH_UTILITY_BASENAME_HPP 

#include <string>

namespace graph {
	inline std::string basename(const std::string &str) {
		size_t pos = str.find_last_of("/\\");
		if(pos == std::string::npos) return str;
		else return str.substr(pos+1);
	}
}

#endif
