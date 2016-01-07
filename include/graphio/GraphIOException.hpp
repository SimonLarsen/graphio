#ifndef GRAPHIO_GRAPHIOEXCEPTION_HPP
#define GRAPHIO_GRAPHIOEXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace graphio { 
	class GraphIOException : public std::runtime_error {
		public:
			GraphIOException(const std::string &message) : std::runtime_error(message) { }
	};
}

#endif
