#pragma once
#include <optional>
#include <fstream>
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

	const std::optional<Receipt> parseReceipt(std::ifstream& json) const; // Parse all data for receipt
	const std::optional<Date> parsePurchaseDate(const std::string& dateStr) const; // Parse value for purchase date
	const std::optional<Time> parsePurchaseTime(const std::string& timeStr) const; // Parse value for purchase time
	const std::optional<Item> parseItem(const std::string& itemStr) const; // Parse value for an item

	// Private helper functions for GET
	const std::string parseScore(std::ifstream& json) const; // Parse score

	const std::string trimWhiteSpace(const std::string& s) const; // Remove leading and trailing whitespace
	const std::string trimQuotes(const std::string& s) const; // Get part of string between quotes

	static const fs::path RECEIPTS;
};