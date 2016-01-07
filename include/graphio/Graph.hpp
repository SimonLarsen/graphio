#ifndef GRAPHIO_GRAPH_HPP
#define GRAPHIO_GRAPH_HPP 

#include <utility>
#include <boost/graph/adjacency_list.hpp>

namespace graphio {
	struct LabeledVertex {
		std::string label;
	};

	struct LabeledEdge {
		std::string label;
	};

	struct LabeledGraph {
		std::string label;
	};
}

#endif
