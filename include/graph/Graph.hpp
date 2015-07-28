#ifndef GRAPH_GRAPH_HPP
#define GRAPH_GRAPH_HPP 

#include <boost/functional/hash.hpp>
#include <boost/graph/adjacency_matrix.hpp>

template<class Graph, class V>
inline bool has_edge(V u, V v, const Graph &g) {
	return edge(u, v, g).second;
}

#endif
