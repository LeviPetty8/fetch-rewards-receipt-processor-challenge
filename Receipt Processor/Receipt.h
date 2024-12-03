#pragma once
// Standard libraries
#include <vector>

// Program files
#include "Item.h"

typedef unsigned int uint;

class Receipt
{
public:
	// Constructors
	Receipt();
	Receipt(const std::string& retailer, const std::string& pdate, const std::string& ptime, const std::vector<Item>& items, const double total);

	// Public member functions
	const bool validateDate() const; // Validate the date string
	const bool validateTime() const; // Validate the time string
	const bool validateTotal() const; // Validate the total double

	const uint calculatePoints() const; // Calculate score for the receipt

private:
	const std::string retailer;
	const std::string pdate;
	const std::string ptime;
	const std::vector<Item> items;
	const double total;
};