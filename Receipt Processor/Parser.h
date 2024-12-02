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

private:
	const Date parsePurchaseDate(const std::string& dateStr) const; // Parse value for purchase date
	const Time parsePurchaseTime(const std::string& timeStr) const; // Parse value for purchase time
	const Item parseItem(const std::string& itemStr) const; // Parse value for an item

	const std::string trimWhiteSpace(const std::string& s) const; // Remove leading and trailing whitespace
	const std::string trimQuotes(const std::string& s) const; // Get part of string between quotes
};