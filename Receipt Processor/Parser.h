#pragma once
// Standard libraries
#include <filesystem>

// Program files
#include "Receipt.h"

namespace fs = std::filesystem;

class Parser
{
public:
	// Constructors
	Parser() {};

	// Public member functions
	void POST(const std::string& filename) const; // Posts the point value for the receipt within {filename} to a new file in a new ID folder
	void GET(const std::string& id) const; // Retrieves the point value for the receipt with a given ID
private:
	// Private helper functions for POST
	const std::string generateID() const; // Generates an ID for the receipt
	const std::string intToID(int num) const; // Converts a number to an ID

	static const fs::path RECEIPTS;
};