#ifndef GRAPH_GRAPH_HPP
#define GRAPH_GRAPH_HPP 

#include <utility>
#include <boost/graph/adjacency_list.hpp>

namespace graph {
	struct Vertex {
		std::string label;
	};

	struct Edge {
		std::string label;
	};

	typedef boost::adjacency_list<
		boost::vecS,
		boost::vecS,
		boost::undirectedS,
		Vertex,
		Edge
	> Graph;
}

#endif
