#pragma once
#include <vector>
#include "Date.h"
#include "Time.h"
#include "Item.h"

class Receipt
{
public:
	// Constructors
	Receipt();
	Receipt(const std::string& retailer, const uint purchaseDate, const uint purchaseTime, const std::vector<Item>& items, const double total);

	// Member functions
	bool validate() const; // Returns whether the Receipt has valid data
	std::string generate_id() const; // Generates an id for the Receipt
	uint calculate_points() const; // Calculates the points for the Receipt

	// Member variables
	const std::string retailer;
	const uint purchaseDate;
	const uint purchaseTime;
	const std::vector<Item> items;
	const double total;
};