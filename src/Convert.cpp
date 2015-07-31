#include <iostream>
#include <graph/Graph.hpp>
#include <graph/GraphReader.hpp>
#include <graph/GraphWriter.hpp>

int main(int argc, const char *argv[]) {
	graph::Graph<> g;

	if(argc != 3) {
		std::cerr << "error: Invalid number of arguments." << std::endl;
		std::cerr << "Usage: " << argv[0] << " INPUTFILE OUTPUTFILE" << std::endl;
		return 1;
	}

	graph::readGraph(argv[1], g);
	graph::writeGraph(g, argv[2]);

	return 0;
}
