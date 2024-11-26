#pragma once
#include <string>

class Item
{
public:
	// Constructors
	Item();
	Item(const std::string& desc, const double price);

	// Member functions
	bool validate() const; // Returns whether Item is valid

	// Member variables
	const std::string desc;
	const double price;
};