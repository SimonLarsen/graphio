#ifndef GRAPH_GRAPH_HPP
#define GRAPH_GRAPH_HPP 

#include <utility>
#include <boost/graph/adjacency_list.hpp>

namespace graph {
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
