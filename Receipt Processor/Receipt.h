#pragma once
#include <vector>
#include "Item.h"

typedef unsigned int uint;

class Receipt
{
public:
	Receipt();
	Receipt(const std::string& retailer, const std::string& pdate, const std::string& ptime, const std::vector<Item>& items, const double total);

	const bool validateDate() const;

	const bool validateTime() const;

	const bool validateTotal() const;

	const uint calculatePoints() const;

private:
	const std::string retailer;
	const std::string pdate;
	const std::string ptime;
	const std::vector<Item> items;
	const double total;
};