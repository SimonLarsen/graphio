#ifndef GRAPH_EDGEVISITOR_HPP
#define GRAPH_EDGEVISITOR_HPP 

#include <string>

namespace graph {
	class EdgeVisitor {
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

			template<typename T, typename E>
			inline T &value(E &e, size_t i) const {
				return 0;
			}

			template<typename T, typename E>
			inline const T &value(const E &e, size_t i) const {
				return 0;
			}

			template<typename E>
			inline std::string value_str(const E &e, size_t i) const {
				return "";
			}
	};
}

#endif
