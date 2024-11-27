#include "Item.h"

// Define constructors

Item::Item() : Item("", 0) // Default constructor
{}

Item::Item(const std::string& desc, const double price) : // Parameterized constructor
	desc(desc), price(price)
{}

// Define public functions

bool Item::validate() const
{
	return price >= 0; // Price cannot be negative
}

const std::string Item::trimWhiteSpace() const
{
	std::string result = desc.substr(desc.find_first_not_of(' ')); // Remove leading whitespaces
	result = result.substr(0, result.find_last_not_of(' ')+1); // Remove trailing whitespaces
	return result;
}