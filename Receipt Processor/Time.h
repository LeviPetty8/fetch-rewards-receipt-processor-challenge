#pragma once
#include <string>
#include <iostream>

typedef unsigned int uint;

class Time
{
public:
	// Constructors
	Time();
	Time(const uint hour, const uint minute);

	// Member functions
	bool validate() const; // Returns whether the Time is valid
	std::string to_string() const; // Converts the Time to a string

	// Operator overloads
	friend std::ostream& operator<<(std::ostream& os, const Time& t);
	//void operator=(const Time& t);
	const bool operator<(const Time t) const;
	const bool operator>(const Time t) const;

	// Member variables
	const uint hour;
	const uint minute;

private:
	// Default variable values
	static const uint DEF_HOUR = 12;
	static const uint DEF_MINUTE = 0;	
};