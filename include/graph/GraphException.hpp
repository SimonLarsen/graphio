#ifndef GRAPH_GRAPHEXCEPTION_HPP
#define GRAPH_GRAPHEXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace graph { 
	class GraphException : public std::runtime_error {
		public:
			GraphException(const std::string &message) : std::runtime_error(message) { }
	};
}

#endif
