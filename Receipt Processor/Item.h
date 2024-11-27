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
	std::string trimWhiteSpace() const; // Remove leading and trailing whitespace from the description

	// Member variables
	const std::string desc;
	const double price;
};