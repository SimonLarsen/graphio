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

	template<class vertex_type = Vertex, class edge_type = Edge>
	class Graph {
		public:
			typedef typename boost::adjacency_list<
				boost::vecS,
				boost::vecS,
				boost::undirectedS,
				vertex_type,
				edge_type
			> graph_type;

			typedef typename boost::graph_traits<graph_type>::vertex_descriptor vertex_descriptor;
			typedef typename boost::graph_traits<graph_type>::vertex_iterator vertex_iterator;
			typedef typename boost::graph_traits<graph_type>::edge_descriptor edge_descriptor;
			typedef typename boost::graph_traits<graph_type>::edge_iterator edge_iterator;
			typedef typename boost::graph_traits<graph_type>::adjacency_iterator adjacency_iterator;
			typedef typename boost::graph_traits<graph_type>::degree_size_type degree_size_type;

			Graph(size_t n = 0) : g(n) { }

			inline size_t vertexCount() const {
				return num_vertices(g);
			}

			inline size_t edgeCount() const {
				return num_edges(g);
			}

			inline void addEdge(vertex_descriptor u, vertex_descriptor v) {
				add_edge(u, v, g);
			}

			inline void removeEdge(vertex_descriptor u, vertex_descriptor v) {
				remove_edge(u, v, g); 
			}

			inline degree_size_type degree(vertex_descriptor u) const {
				return out_degree(u, g);
			}

			/**
			 * Get reference to vertex's bundled properties.
			 */
			inline vertex_type &vertex(vertex_descriptor u) {
				return g[u]; 
			}

			/**
			 * Get const reference to vertex's bundled properties.
			 */
			inline const vertex_type &vertex(vertex_descriptor u) const {
				return g[u]; 
			}
			
			inline vertex_type &node(vertex_descriptor u) {
				return vertex(u);
			}

			inline const vertex_type &node(vertex_descriptor u) const {
				return vertex(u);
			}

			inline std::pair<adjacency_iterator, adjacency_iterator>
			getAdjacent(vertex_descriptor u) const {
				return adjacent_vertices(u, g);
			}

			/**
			 * Get reference to edge's bundled properties.
			 */
			inline edge_type &edge(vertex_descriptor u, vertex_descriptor v) {
				return g[boost::edge(u, v, g).first]; 
			}

			/**
			 * Get const reference to edge's bundled properties.
			 */
			inline const edge_type &edge(vertex_descriptor u, vertex_descriptor v) const {
				return g[boost::edge(u, v, g).first]; 
			}

			inline graph_type &graph() {
				return g;
			}

			inline const graph_type &graph() const {
				return g;
			}

		private:
			graph_type g;
	};
}

#endif
