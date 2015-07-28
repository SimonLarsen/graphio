#ifndef GRAPH_GRAPH_HPP
#define GRAPH_GRAPH_HPP 

#include <boost/functional/hash.hpp>
#include <boost/graph/adjacency_matrix.hpp>

template <typename D, typename VP, typename EP, typename GP, typename A, typename V>
inline bool has_edge(V u, V v, const boost::adjacency_matrix<D,VP,EP,GP,A> &g) {
	return edge(u, v, g).second;
}

#endif
