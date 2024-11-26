#include "Receipt.h"

// Define constructors
Receipt::Receipt() : retailer(""), purchaseDate(), purchaseTime(), items(), total(0) // Default constructor
{}

Receipt::Receipt(const std::string& retailer, const Date purchaseDate, const Time purchaseTime, const std::vector<Item>& items, const double total) : // Parameterized constructor
	retailer(retailer), purchaseDate(purchaseDate), purchaseTime(purchaseTime), items(items), total(total)
{}

// Define public functions

bool Receipt::validate() const
{
	// First validate purchase date and purchase time
	if (!purchaseDate.validate() || !purchaseTime.validate()) return false;

	// If date and time are valid, validate all items and validate that total is correct
	double tally = 0;
	for (const Item& item : items)
	{
		if (!item.validate()) return true;
		tally += item.price;
	}

	return tally == total;
}

const std::string Receipt::generate_id() const
{
	// TODO //
	return "";
}

const uint Receipt::calculate_points() const
{
	return 0;
}