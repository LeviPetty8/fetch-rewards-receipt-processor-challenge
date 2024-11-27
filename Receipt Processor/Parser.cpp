#include "Parser.h"

// Define public member functions for POST

const Receipt Parser::parseReceipt(std::fstream& json) const
{
	std::string current;
	while (json >> current)
	{
		std::cout << current << std::endl;
	}

	return Receipt();
}