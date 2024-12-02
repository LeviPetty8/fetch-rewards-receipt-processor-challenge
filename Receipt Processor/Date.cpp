#include <format>
#include "Date.h"

// Define constructors

Date::Date() : Date(DEF_YEAR, DEF_MONTH, DEF_DAY) // Default constructor
{}

Date::Date(const uint year, const uint month, const uint day) : // Parameterized constructor
	year(year), month(month), day(day)
{}

// Define public functions

bool Date::validate() const
{
	if (month > 12) return false;

	// Number of days in the month
	uint days = 0;

	// Calculate number of days in the month based on the month
	if (month > 7) // After July, even months have 31 days and odd months have 30 days
	{
		days = (month % 2 == 0) ? 31 : 30;
	}
	else if (month != 2) // Before and including July, even months have 30 days and odd months have 31 days
	{
		days = (month % 2 == 0) ? 30 : 31;
	}
	else // Special case for February
	{
		days = is_leap_year() ? 29 : 28;
	}

	return day <= days;
}

std::string Date::to_string() const
{
	std::string yearStr = std::to_string(year);
	if (yearStr.length() < 4) yearStr.insert(0, 4 - yearStr.length(), '0'); // 4 digit year
	
	std::string monthStr = std::to_string(month);
	monthStr.insert(0, 2 - monthStr.length(), '0'); // 2 digit month

	std::string dayStr = std::to_string(day);
	dayStr.insert(0, 2 - dayStr.length(), '0'); // 2 digit day

	return std::format("{}-{}-{}", yearStr, monthStr, day); // yyyy-mm-dd
}

// Define operator overloads

std::ostream& operator<<(std::ostream& os, const Date& dt)
{
	os << dt.to_string();
	return os;
}

// Define private functions

bool Date::is_leap_year() const
{
	if (year % 400 == 0) return true; // Years divisible by 400 are always leap years
	else if (year % 100 == 0) return false; // Years divisible by 100 but not 400 are not leap years
	else return year % 4 == 0; // Years divisible by 4 but not 100 are always leap years
}