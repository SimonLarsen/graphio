#ifndef GRAPH_VERTEXVISITOR_HPP
#define GRAPH_VERTEXVISITOR_HPP 

#include <string>

namespace graph {
	class VertexVisitor {
		public:
			inline size_t count() const {
				return 0;
			}

			inline std::string name(size_t i) const {
				return "";
			}

			inline std::string type(size_t i) const {
				return "";
			}

			template<typename T, typename V>
			inline T &value(V &v, size_t i) const {
				return 0;
			}

			template<typename T, typename V>
			inline const T &value(const V &v, size_t i) const {
				return 0;
			}

			template<typename V>
			inline std::string value_str(const V &v, size_t i) const {
				return "";
			}
	};
}

#endif
