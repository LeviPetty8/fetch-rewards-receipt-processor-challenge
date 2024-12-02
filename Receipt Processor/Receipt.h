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
	Receipt(const std::string& retailer, const Date purchaseDate, const Time purchaseTime, const std::vector<Item>& items, const double total);

	// Member functions
	bool validate() const; // Returns whether the Receipt has valid data
	const uint calculatePoints() const; // Calculates the points for the Receipt

	// Member variables
	const std::string retailer;
	const Date purchaseDate;
	const Time purchaseTime;
	const std::vector<Item> items;
	const double total;

private:
	static const Time TARGET_TIME_BEGIN;
	static const Time TARGET_TIME_END;
};