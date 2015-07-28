#ifndef GRAPH_ALGORITHMS_HPP
#define GRAPH_ALGORITHMS_HPP

#include <vector>
#include <stack>
#include <random>
#include <algorithm>
#include <graph/Graph.hpp>

namespace graph {
	template<class Graph, class V>
	inline void get_edges(const Graph &g, std::vector<std::pair<V,V>> &out) {
		out.clear();
		for(auto it = edges(g); it.first != it.second; ++it.first) {
			V u = source(*it.first, g);
			V v = target(*it.first, g);
			out.push_back(std::pair<V,V>(u, v));
		}
	}

	template<class Graph, class V>
	inline void add_edges(const std::vector<std::pair<V,V>> &in, Graph &g) {
		for(auto &e : in) {
			add_edge(e.first, e.second, g);
		}
	}

	/**
	 * Creates subgraph from list of vertices.
	 */
	template<class Graph, class V>
	inline void subgraph(const Graph &g, const std::vector<V> &indices, Graph &out) {
		out = Graph(indices.size());

		for(size_t i = 0; i < indices.size(); ++i) {
			out[i].label = g[indices[i]].label;

			for(size_t j = i+1; j < indices.size(); ++j) {
				if(has_edge(indices[i], indices[j], g)) {
					add_edge(i, j, out);
				}
			}
		}

		for(size_t i = 0; i < indices.size(); ++i) {
			for(size_t j = 0; j < indices.size(); ++j) {
				out[edge(i, j, out).first].weight = g[edge(indices[i], indices[j], g).first].weight;
			}
		}
	}

	/**
	 * Returns mapping from vertices to connected components.
	 */
	template<class Graph, class V>
	inline void connectedComponents(const Graph &g, std::vector<V> &comp) {
		comp.resize(num_vertices(g));
		std::stack<V> stack;

		std::vector<bool> marked(num_vertices(g), false);

		size_t covered = 0;
		int cur_comp = 0;
		while(covered < num_vertices(g)) {
			for(size_t i = 0; i < num_vertices(g); ++i) {
				if(marked[i] == false) {
					stack.push(i);
					break;
				}
			}

			while(stack.empty() == false) {
				V v = stack.top();
				stack.pop();

				if(marked[v] == false) {
					marked[v] = true;
					covered++;
					comp[v] = cur_comp;

					for(size_t u = 0; u < num_vertices(g); ++u) {
						if(has_edge(u, v, g)) {
							stack.push(u);
						}
					}
				}

			}

			cur_comp++;
		}
	}

	/**
	 * Creates new graph containing only vertices in connected 
	 * components with at least min_size vertices.
	 */
	template<class Graph>
	inline void filterComponents(const Graph &g, int min_size, Graph &out) {
		typedef typename boost::graph_traits<Graph>::vertex_descriptor V;

		std::vector<V> comp;
		connectedComponents(g, comp);

		std::vector<int> count(num_vertices(g), 0);
		std::vector<V> keep;
		
		for(auto i : comp) {
			count[i]++;
		}

		for(size_t i = 0; i < num_vertices(g); ++i) {
			if(count[comp[i]] >= min_size) {
				keep.push_back(i);
			}
		}

		subgraph(g, keep, out);
	}

	/**
	 * Returns list of vertices contained in largest connected component.
	 */
	template<class Graph, class V>
	inline void largestComponentIndices(const Graph &g, std::vector<V> &indices) {
		std::vector<V> comp;
		connectedComponents(g, comp);

		std::vector<int> count(num_vertices(g), 0);
		indices.clear();

		for(auto i : comp) {
			count[i]++;
		}

		size_t largest = 0;
		for(size_t i = 0; i < num_vertices(g); ++i) {
			if(count[i] > count[largest]) {
				largest = i;
			}
		}

		for(size_t i = 0; i < num_vertices(g); ++i) {
			if(comp[i] == largest) {
				indices.push_back(i);
			}
		}
	}

	/**
	 * Creates new graph with only vertices contained in largest
	 * connected component.
	 */
	template<class Graph>
	inline void largestComponent(const Graph &g, Graph &out) {
		typedef typename boost::graph_traits<Graph>::vertex_descriptor V;
		std::vector<V> indices;
		largestComponentIndices(g, indices);
		subgraph(g, indices, out);
	}

	/**
	 * Randomizes edges of graph creating new graph
	 * with similar edge degree distribution.
	 */
	template<class Graph>
	inline void randomizeEndpoints(Graph &g, int count) {
		typedef typename boost::graph_traits<Graph>::vertex_descriptor V;
		std::vector<std::pair<V,V>> edges;
		get_edges(g, edges);

		std::default_random_engine gen;
		std::uniform_int_distribution<size_t> dist(0, edges.size()-1);

		int swaps = 0;
		while(swaps < count) {
			size_t e1 = dist(gen);
			size_t e2 = dist(gen);

			if(e1 == e2) continue;

			size_t a1, a2, b1, b2;

			if(gen() % 2 == 0) {
				a1 = edges[e1].first;
				a2 = edges[e1].second;
			} else {
				a1 = edges[e1].second;
				a2 = edges[e1].first;
			}

			if(gen() % 2 == 0) {
				b1 = edges[e2].first;
				b2 = edges[e2].second;
			} else {
				b1 = edges[e2].second;
				b2 = edges[e2].first;
			}

			if(a1 == b1 || a1 == b2
			|| a2 == b1 || a2 == b2) continue;

			if(has_edge(a1, b2, g) || has_edge(b1, a2, g)) continue;

			remove_edge(a1, a2, g);
			remove_edge(b1, b2, g);

			add_edge(a1, b2, g);
			add_edge(b1, a2, g);

			edges[e1] = std::make_pair(a1, b2);
			edges[e2] = std::make_pair(b1, a2);

			swaps++;
		}
	}
}

#endif
