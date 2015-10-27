#ifndef GRAPH_UTILITY_SPLIT_HPP
#define GRAPH_UTILITY_SPLIT_HPP 

#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

namespace graph {
	void escaped_split(const std::string &str, const std::string &sep, std::vector<std::string> &parts) {
		boost::escaped_list_separator<char> els("\\", sep, "\"");
		boost::tokenizer<boost::escaped_list_separator<char>> tokens(str, els);

		parts.clear();
		for(auto it = tokens.begin(); it != tokens.end(); ++it) {
			if((*it).length() > 0) {
				parts.push_back(boost::trim_copy(*it));
			}
		}
	}
}

#endif
