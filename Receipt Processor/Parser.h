#pragma once
#include <fstream>
#include <filesystem>
#include "Receipt.h"

namespace fs = std::filesystem;

class Parser
{
public:
	// Constructors
	Parser() {};

	// Member functions for POST
	void POST(const std::string& filename) const;
private:
	// Private helper functions for POST
	const std::string generateID() const; // Generates an id for the Receipt
	const std::string intToID(int num) const; // Converts a number to an id

	const Receipt parseReceipt(std::ifstream& json) const; // Parse all data for receipt
	const Date parsePurchaseDate(const std::string& dateStr) const; // Parse value for purchase date
	const Time parsePurchaseTime(const std::string& timeStr) const; // Parse value for purchase time
	const Item parseItem(const std::string& itemStr) const; // Parse value for an item

	const std::string trimWhiteSpace(const std::string& s) const; // Remove leading and trailing whitespace
	const std::string trimQuotes(const std::string& s) const; // Get part of string between quotes

	static const fs::path RECEIPTS;
};