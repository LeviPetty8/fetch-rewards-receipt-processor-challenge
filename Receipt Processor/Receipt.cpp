// Standard libraries
#include <regex>

// Program files
#include "Receipt.h"

// Define constructors

Receipt::Receipt() : retailer(""), pdate("1900-01-01"), ptime("12:00"), items(), total(0) // Default constructor
{}

Receipt::Receipt(const std::string& retailer, const std::string& pdate, const std::string& ptime, const std::vector<Item>& items, const double total) : // Parameterized constructor
	retailer(retailer), pdate(pdate), ptime(ptime), items(items), total(total)
{}

// Define public member functions

const bool Receipt::validateDate() const
{
	std::regex format(R"(^\d{4}-\d{2}-\d{2}$)");
	return std::regex_match(pdate, format);
}

const bool Receipt::validateTime() const
{
	std::regex format(R"(^\d{2}:\d{2}$)");
	return std::regex_match(ptime, format);
}

const bool Receipt::validateTotal() const
{
	double sum = 0;
	for (const Item& item : items) sum += item.price;
	return sum == total;
}

const uint Receipt::calculatePoints() const
{
	/* Rules for points:
		 * 1 point for every alphanumeric character in the retailer name.
		 * 6 points if the day in the purchase date is odd.
		 * 10 points if the time of purchase is after 2:00pm and before 4:00pm.
		 * 50 points if the total is a round dollar amount.
		 * 25 points if the total is a multiple of 0.25.
		 * 5 points for every two items on the receipt.
		 * For each item, if the trimmed description length is a multiple of 3, multiply the price by 0.2 and round the result up to the nearest integer; earn that many points.
		 */
	uint points = 0;

	// 1 point for every alphanumeric character in the retailer name
	for (const char& c : retailer)
	{
		if (isalnum(c)) points++;
	}

	// 6 points if the day in the purchase date is odd
	if ((pdate.back() - '0') % 2 != 0) points += 6;

	// 10 points if the time of purchase is after 2:00pm and before 4:00pm
	const int hour = std::stoi(ptime.substr(0, 2));
	if (hour == 14)
	{
		const int minute = std::stoi(ptime.substr(3));
		if (minute > 0) points += 10;
	}
	else if (hour == 15) points += 10;

	// 50 points if the total is a round dollar amount
	if (fmod(total, 1) == 0) points += 50;

	// 25 points if the total is a multiple of 0.25
	if (fmod(total, 0.25) == 0) points += 25;

	// 5 points for every two items on the receipt
	points += 5 * (items.size() / 2);

	// For each item, if the trimmed description length is a multiple of 3, multiply the price by 0.2 and round the result up to the nearest integer; earn that many points
	for (const Item& item : items)
	{
		if (item.trimWhiteSpace().length() % 3 == 0)
		{
			points += ceil(item.price * 0.2);
		}
	}

	return points;
}