#pragma once
#include <string>
#include <iostream>

#include "TypeDefs.h"

class Date
{
public:
	// Constructors
	Date();
	Date(const uint year, const uint month, const uint day);

	// Member functions
	bool validate() const; // Returns whether the Date is valid
	std::string to_string() const; // Converts the date to a string

	// Operator overloads
	friend std::ostream& operator<<(std::ostream& os, const Date& dt);

	// Member variables
	const uint year;
	const uint month;
	const uint day;

private:
	// Default variable values
	static const uint DEF_YEAR = 1900;
	static const uint DEF_MONTH = 1;
	static const uint DEF_DAY = 1;

	// Private member function
	bool is_leap_year() const; // Calculates whether the Date belongs to a leap-year
};