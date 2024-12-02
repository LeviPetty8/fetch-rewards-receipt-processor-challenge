#include <sstream>
#include <regex>
#include "Parser.h"

// Define public member functions for POST

const Receipt Parser::parseReceipt(std::fstream& json) const
{
	std::string data;

	for (std::string line; std::getline(json, line);)
	{
		data.append(trimWhiteSpace(line));
	}

	// If file does not start with a { and end with a }, then it is malformed
	if (data.front() != '{')
	{
		std::cerr << "File format invalid; file must open with a {.";
		return Receipt();
	}
	if (data.back() != '}')
	{
		std::cerr << "File format invalid; file must open with a }.";
		return Receipt();
	}

	// Parse receipt data
	data.erase(data.begin());
	data.pop_back();
	std::stringstream ss(data);
	std::string current;

	// Expect first word to be "retailer"
	std::getline(ss, current, ':');
	if (current.compare("\"retailer\"") != 0)
	{
		std::cerr << "File format invalid; expected \"retailer\" but not found.";
		return Receipt();
	}
	// Next word is the value for retailer
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	if (current.empty())
	{
		std::cerr << "File format invalid; invalid value for \"retailer\".";
		return Receipt();
	}
	const std::string retailer = current;

	// Next is "purchaseDate"
	std::getline(ss, current, ':');
	if (current.compare("\"purchaseDate\"") != 0)
	{
		std::cerr << "File format invalid; expected \"purchaseDate\" but not found.";
		return Receipt();
	}
	// Next word is the value for purchaseDate
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	if (current.empty())
	{
		std::cerr << "File format invalid; invalid value for \"purchaseDate\". Expected format is yyyy-mm-dd.";
		return Receipt();
	}
	const Date purchaseDate = parsePurchaseDate(current);
	if (!purchaseDate.validate())
	{
		std::cerr << "File data invalid; Date is impossible.";
		return Receipt();
	}

	// Next is "purchaseTime"
	std::getline(ss, current, ':');
	if (current.compare("\"purchaseTime\"") != 0)
	{
		std::cerr << "File format invalid; expected \"purchaseTime\" but not found.";
		return Receipt();
	}
	// Next word is the value for purchaseTime
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	if (current.empty())
	{
		std::cerr << "File format invalid; invalid value for \"purchaseTime\". Expected format is hh:mm.";
		return Receipt();
	}
	const Time purchaseTime = parsePurchaseTime(current);
	if (!purchaseTime.validate())
	{
		std::cerr << "File data invalid; Time is impossible.";
		return Receipt();
	}

	// Next is "total"
	std::getline(ss, current, ':');
	if (current.compare("\"total\"") != 0)
	{
		std::cerr << "File format invalid; expected \"total\" but not found.";
		return Receipt();
	}
	// Next word is the value for total
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	std::regex totalFormat("[0-9]+.[0-9]{2}");
	if (!std::regex_match(current, totalFormat))
	{
		std::cerr << "File format invalid; invalid value for \"total\". Expected format includes cents even for exact dollar amounts.";
		return Receipt();
	}
	const double total = std::stod(current);

	// Next is "items"
	std::getline(ss, current, ':');
	if (current.compare("\"items\"") != 0)
	{
		std::cerr << "File format invalid; expected \"items\" but not found.";
		return Receipt();
	}
	// Next is the value for items
	std::getline(ss, current);
	current = trimWhiteSpace(current);
	if (current.front() != '[' ||
		current.back() != ']')
	{
		std::cerr << "File format invalid; expected items to begin with [ and end with ].";
		return Receipt();
	}
	// Remove [ and ]
	current.erase(current.begin());
	current.pop_back();

	// Setup vector of items
	std::vector<Item> items;

	// Iterate through items
	bool first = true;
	while (!current.empty())
	{
		if (!first)
		{
			if (current.front() != ',')
			{
				std::cerr << "File format invalid; items should be separated by commas.";
				return Receipt();
			}
			current.erase(current.begin()); // Remove comma at start
		}
		if (current.front() != '{')
		{
			std::cerr << "File format invalid; each item should begin with {.";
			return Receipt();
		}
		// Setup for current iteration
		const size_t end = current.find_first_of('}');
		const std::string itemStr = current.substr(1, end - 1);

		items.push_back(parseItem(itemStr));

		// Setup for next iteration
		current = current.substr(end + 1);
		first = false;
	}

	Receipt result(retailer, purchaseDate, purchaseTime, items, total);
	if (!result.validate())
	{
		std::cerr << "File data invalid; result receipt is not valid.";
		return Receipt();
	}
	return result;
}

const Date Parser::parsePurchaseDate(const std::string& dateStr) const
{
	// Make sure format is correct
	std::regex dateFormat("[0-9]{4,}-[0-9]{2}-[0-9]{2}");
	if (!std::regex_match(dateStr, dateFormat))
	{
		std::cerr << "File format invalid; invalid value for \"purchaseDate\". Expected format is yyyy-mm-dd.";
		return Date();
	}

	std::stringstream ss(dateStr);

	std::string year;
	std::getline(ss, year, '-');

	std::string month;
	std::getline(ss, month, '-');

	std::string day;
	std::getline(ss, day);

	return Date(std::stoi(year), std::stoi(month), std::stoi(day));
}

const Time Parser::parsePurchaseTime(const std::string& timeStr) const
{
	// Make sure format is correct
	std::regex timeFormat("[0-9]{2}:[0-9]{2}");
	if (!std::regex_match(timeStr, timeFormat))
	{
		std::cerr << "File format invalid; invalid value for \"purchaseTime\". Expected format is hh:mm.";
		return Time();
	}

	std::stringstream ss(timeStr);

	std::string hour;
	std::getline(ss, hour, ':');

	std::string minute;
	std::getline(ss, minute);

	return Time(std::stoi(hour), std::stoi(minute));
}

const Item Parser::parseItem(const std::string& itemStr) const
{
	std::stringstream ss(itemStr);
	std::string current;
	
	// Expect first word to be "shortDescription"
	std::getline(ss, current, ':');
	if (current.compare("\"shortDescription\"") != 0)
	{
		std::cerr << "File format invalid; expected \"shortDescription\" but not found.";
		return Item();
	}
	// Next is value for shortDescription
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	if (current.empty())
	{
		std::cerr << "File format invalid; invalid value for \"shortDescription\".";
		return Item();
	}
	const std::string desc = current;

	// Next is "price"
	std::getline(ss, current, ':');
	current = trimWhiteSpace(current);
	if (current.compare("\"price\"") != 0)
	{
		std::cerr << "File format invalid; expected \"price\" but not found.";
		return Item();
	}
	// Next is value for price
	std::getline(ss, current);
	current = trimQuotes(current);
	std::regex priceFormat("[0-9]+.[0-9]{2}");
	if (!std::regex_match(current, priceFormat))
	{
		std::cerr << "File format invalid; invalid value for \"price\". Expected format includes cents even for exact dollar amounts.";
		return Item();
	}
	const double price = std::stod(current);

	return Item(desc, price);
}

const std::string Parser::trimWhiteSpace(const std::string& s) const
{
	// Return an empty string if the string only contains whitespace
	size_t begin = s.find_first_not_of(' ');
	if (begin == std::string::npos) return "";

	return s.substr(begin, s.find_last_not_of(' ') - begin + 1);
}

const std::string Parser::trimQuotes(const std::string& s) const
{
	std::string result = trimWhiteSpace(s);
	
	// Return an empty string if the string does not begin and end with a "
	if (result.front() != '"' ||
		result.back() != '"') return "";
	else
	// Trim quotes from begin and end
	{
		result.erase(result.begin());
		result.pop_back();
	}

	// Return an empty string if the string contains a " within the middle
	if (result.find_first_of('"') != std::string::npos) return "";

	return result;
}