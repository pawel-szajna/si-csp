#include <iostream>
#include <sstream>

#include "nqueens.hh"

template<typename T>
T lexical_cast(std::string value)
{
	T result;
	std::stringstream stream(value);
	stream >> result;

	if (!stream) {
		throw std::invalid_argument("lexical cast failed, argument was '" + value + "'");
	}

	return result;
}

int main(int argc, char** argv)
{
	int n = 10;
	int m = nqueens::FORWARD;
	if (argc > 1) {
		n = lexical_cast<int>( argv[1] );
	}
	if (argc > 2) {
		m = lexical_cast<int>( argv[2] );
	}
	int visited, solutions;
	nqueens::solve(n, m, visited, solutions);
	std::cout << visited << " " << solutions << "\n";
	return 0;
}
