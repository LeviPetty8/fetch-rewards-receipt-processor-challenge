#pragma once
#include <fstream>
#include "Receipt.h"

class Parser
{
public:
	// Constructors
	Parser() {};

	// Member functions for POST
	const Receipt parseReceipt(std::fstream& json) const;
};