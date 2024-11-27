#include <format>
#include "Time.h"

// Define constructors

Time::Time() : Time(DEF_HOUR, DEF_MINUTE) // Default constructor
{}

Time::Time(const uint hour, const uint minute) : // Parameterized constructor
	hour(hour), minute(minute)
{}

// Define public functions

bool Time::validate() const
{
	return hour < 24 && minute < 60; // 24-hour day
}

std::string Time::to_string() const
{
	std::string hourStr = std::to_string(hour);
	hourStr.insert(0, 2 - hourStr.length(), '0');

	std::string minuteStr = std::to_string(minute);
	minuteStr.insert(0, 2 - minuteStr.length(), '0');

	return std::format("{}:{}", hourStr, minuteStr);
}

// Define operator overloads

std::ostream& operator<<(std::ostream& os, const Time& t)
{
	os << t.to_string();
	return os;
}

bool Time::operator<(const Time& t)
{
	return hour < t.hour || minute < t.minute;
}

bool Time::operator>(const Time& t)
{
	return hour > t.hour || minute > t.minute;
}