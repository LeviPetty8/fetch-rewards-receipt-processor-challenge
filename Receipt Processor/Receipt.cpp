#include <ctype.h>
#include "Receipt.h"

const Time Receipt::TARGET_TIME_BEGIN(2, 0);
const Time Receipt::TARGET_TIME_END(4, 0);

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
	/* Rules for points:
	 * 1 point for every alphanumeric character in the retailer name.
	 * 6 points if the day in the purchase date is odd.
	 * 10 points if the time of purchase is after 2:00pm and before 4:00pm.
	 * 50 points if the total is a round dollar amount.
	 * 25 points if the total is a multiple of 0.25.
	 * 5 points for every two items on the receipt.
	 * For each item, if the description is a multiple of 3, multiply the price by 0.2 and round the result up to the nearest integer; earn that many points.
	 */
	uint points = 0;

	// 1 point for every alphanumeric character in the retailer name
	for (const char c : retailer)
	{
		if (isalnum(c)) points++;
	}

	// 6 points if the day in the purchase date is odd
	if (purchaseDate.day % 2 != 0) points += 6;

	// 10 points if the time of purchase is after 2:00pm and before 4:00pm
	//if (purchaseTime > TARGET_TIME_BEGIN && purchaseTime < TARGET_TIME_END) points += 10;

	return 0;
}