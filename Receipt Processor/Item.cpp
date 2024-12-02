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
	size_t begin = desc.find_first_not_of(' ');
	return desc.substr(begin, desc.find_last_not_of(' ') - begin + 1);
}