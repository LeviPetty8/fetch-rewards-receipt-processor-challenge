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
	const Date parsePurchaseDate(const std::string& dateStr) const;
	const Time parsePurchaseTime(const std::string& timeStr) const;

	const std::string trimWhiteSpace(std::string s) const; // Remove leading and trailing whitespace
	const std::string trimQuotes(std::string s) const; // Get part of string between quotes
};