#include <iostream>
#include <graphio/Graph.hpp>
#include <graphio/GraphReader.hpp>
#include <graphio/GraphWriter.hpp>

int main(int argc, const char *argv[]) {
	boost::adjacency_list<
		boost::setS,
		boost::vecS,
		boost::undirectedS,
		graphio::LabeledVertex,
		graphio::LabeledEdge,
		graphio::LabeledGraph
	> g(0);

	if(argc != 3) {
		std::cerr << "error: Invalid number of arguments." << std::endl;
		std::cerr << "Usage: " << argv[0] << " INPUTFILE OUTPUTFILE" << std::endl;
		return 1;
	}

	graphio::readGraph(argv[1], g);
	graphio::writeGraph(g, argv[2]);

	return 0;
}
