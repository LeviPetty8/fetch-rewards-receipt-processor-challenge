#pragma once
#include <filesystem>
#include "Receipt.h"

namespace fs = std::filesystem;

class Parser
{
public:
	// Constructors
	Parser() {};

	// Member function for POST
	void POST(const std::string& filename) const;

	// Member function for GET
	void GET(const std::string& id) const;
private:
	// Private helper functions for POST
	const std::string generateID() const; // Generates an id for the Receipt
	const std::string intToID(int num) const; // Converts a number to an id

	static const fs::path RECEIPTS;
};