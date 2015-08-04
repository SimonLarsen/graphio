#ifndef GRAPH_ALGORITHMS_HPP
#define GRAPH_ALGORITHMS_HPP

#include <vector>
#include <stack>
#include <random>
#include <algorithm>
#include <graph/Graph.hpp>

namespace graph {
	template<class G, class V>
	inline void get_edges(const G &g, std::vector<std::pair<V,V>> &out) {
		out.clear();
		for(V u = 0; u < g.vertexCount(); ++u) {
			for(V v = u+1; v < g.vertexCount(); ++v) {
				if(g.hasEdge(u, v)) {
					out.push_back(std::pair<V,V>(u, v));
				}
			}
		}
	}

	template<class G, class V>
	inline void add_edges(const std::vector<std::pair<V,V>> &in, G &g) {
		for(auto &e : in) {
			g.addEdge(e.first, e.second);
		}
	}

	/**
	 * Creates subgraph from list of vertices.
	 */
	template<class G, class V>
	inline void subgraph(const G &g, const std::vector<V> &indices, G &out) {
		out = G(indices.size());

		for(size_t i = 0; i < indices.size(); ++i) {
			out.node(i) = g.node(indices[i]);

			for(size_t j = i+1; j < indices.size(); ++j) {
				if(g.hasEdge(indices[i], indices[j])) {
					out.addEdge(i, j);
				}
			}
		}

		for(size_t i = 0; i < indices.size(); ++i) {
			for(size_t j = 0; j < indices.size(); ++j) {
				out.edge(i, j) = g.edge(indices[i], indices[j]);
			}
		}
	}

	/**
	 * Returns mapping from vertices to connected components.
	 */
	template<class G, class V>
	inline void connectedComponents(const G &g, std::vector<V> &comp) {
		comp.resize(g.vertexCount());
		std::stack<V> stack;

		std::vector<bool> marked(g.vertexCount(), false);

		size_t covered = 0;
		int cur_comp = 0;
		while(covered < g.vertexCount()) {
			for(size_t i = 0; i < g.vertexCount(); ++i) {
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

					for(size_t u = 0; u < g.vertexCount(); ++u) {
						if(g.hasEdge(u, v)) {
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
	template<class G>
	inline void filterComponents(const G &g, int min_size, G &out) {
		typedef typename G::vertex_descriptor V;

		std::vector<V> comp;
		connectedComponents(g, comp);

		std::vector<int> count(g.vertexCount(), 0);
		std::vector<V> keep;
		
		for(auto i : comp) {
			count[i]++;
		}

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			if(count[comp[i]] >= min_size) {
				keep.push_back(i);
			}
		}

		subgraph(g, keep, out);
	}

	/**
	 * Returns list of vertices contained in largest connected component.
	 */
	template<class G, class V>
	inline void largestComponentIndices(const G &g, std::vector<V> &indices) {
		std::vector<V> comp;
		connectedComponents(g, comp);

		std::vector<int> count(g.vertexCount(), 0);
		indices.clear();

		for(auto i : comp) {
			count[i]++;
		}

		size_t largest = 0;
		for(size_t i = 0; i < g.vertexCount(); ++i) {
			if(count[i] > count[largest]) {
				largest = i;
			}
		}

		for(size_t i = 0; i < g.vertexCount(); ++i) {
			if(comp[i] == largest) {
				indices.push_back(i);
			}
		}
	}

	/**
	 * Creates new graph with only vertices contained in largest
	 * connected component.
	 */
	template<class G>
	inline void largestComponent(const G &g, G &out) {
		typedef typename G::vertex_descriptor V;
		std::vector<V> indices;
		largestComponentIndices(g, indices);
		subgraph(g, indices, out);
	}

	/**
	 * Randomizes edges of graph creating new graph
	 * with similar edge degree distribution.
	 */
	template<class G>
	inline void randomizeEndpoints(G &g, int count) {
		typedef typename G::vertex_descriptor V;
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

			if(g.hasEdge(a1, b2) || g.hasEdge(b1, a2)) continue;

			g.removeEdge(a1, a2);
			g.removeEdge(b1, b2);

			g.addEdge(a1, b2);
			g.addEdge(b1, a2);

			edges[e1] = std::make_pair(a1, b2);
			edges[e2] = std::make_pair(b1, a2);

			swaps++;
		}
	}
}

#endif
