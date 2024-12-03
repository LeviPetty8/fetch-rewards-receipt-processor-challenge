#include <sstream>
#include <regex>
#include "Parser.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

namespace json = rapidjson;

const std::nullopt_t null = std::nullopt;

// Define static member variables
const fs::path Parser::RECEIPTS(fs::current_path() / "receipts");

// Define public member function for POST

void Parser::POST(const std::string& filename) const
{
	const fs::path file(RECEIPTS / "process" / filename);

	if (!fs::exists(file))
	{
		std::cerr << std::format("File {} not found.", filename) << std::endl;
		return;
	}

	// Read JSON file
	FILE* ifile;
	fopen_s(&ifile, file.string().c_str(), "r");
	char ibuf[65536];
	json::FileReadStream is(ifile, ibuf, sizeof(ibuf));
	json::Document doc;
	doc.ParseStream(is);
	fclose(ifile);

	// Parse data
	const std::string retailer = doc["retailer"].GetString();
	const std::string pdate = doc["purchaseDate"].GetString();
	const std::string ptime = doc["purchaseTime"].GetString();
	const double total = std::stod(doc["total"].GetString());
	// Parse items
	std::vector<Item> items;
	for (unsigned int i = 0; i < doc["items"].Size(); i++)
	{
		const std::string desc = doc["items"][i]["shortDescription"].GetString();
		const double price = std::stod(doc["items"][i]["price"].GetString());
		items.push_back(Item(desc, price));
	}
	if (items.empty())
	{
		std::cerr << "Invalid data: Items list cannot be empty." << std::endl;
		return;
	}
	// Final result of parsing data
	const Receipt2 receipt(retailer, pdate, ptime, items, total);
	if (!receipt.validateDate())
	{
		std::cerr << "Invalid data: Date format is invalid." << std::endl;
		return;
	}
	if (!receipt.validateTime())
	{
		std::cerr << "Invalid data: Time format is invalid." << std::endl;
		return;
	}
	if (!receipt.validateTotal())
	{
		std::cerr << "Invalid data: Total in \"total\" field doesn't match actual total item prices." << std::endl;
		return;
	}

	// Create new directory for file output
	const std::string id = generateID();

	// Prep data to write
	json::Document score;
	score.SetObject();
	score.AddMember("points", receipt.calculatePoints(), score.GetAllocator());

	// Write JSON file
	const fs::path points(RECEIPTS / id / "points.json");
	FILE* ofile;
	fopen_s(&ofile, points.string().c_str(), "w");
	char obuf[65536];
	json::FileWriteStream os(ofile, obuf, sizeof(obuf));
	json::Writer<json::FileWriteStream> writer(os);
	score.Accept(writer);
	fclose(ofile);

	// Output ID
	std::cout << "{\"id\": \"" << id << "\"}" << std::endl;
}

// Define public member function for GET

void Parser::GET(const std::string& id) const
{
	const fs::path path(RECEIPTS / id);
	if (!fs::exists(path))
	{
		std::cerr << std::format("ID {} not found.", id) << std::endl;
		return;
	}
	const fs::path file(path / "points.json");

	// Setup for file input
	std::ifstream fin(file);
	if (!fin.is_open())
	{
		std::cerr << "File points.json failed to open." << std::endl;
		return;
	}

	// Perform file input
	const std::string result = parseScore(fin);
	fin.close();

	// Print output
	if (result.empty())
	{
		std::cerr << "Invalid file format." << std::endl;
		return;
	}
	std::cout << result << std::endl;
}

// Define private helper member functions for POST

const std::string Parser::generateID() const
{
	const fs::path receipts(fs::current_path() / "receipts");

	// Find first id that doesn't have an existing directory
	for (int i = 0; true; i++)
	{
		std::string id = intToID(i);
		if (id.size() < 16) id.insert(0, 16 - id.size(), '0');

		if (!fs::exists(receipts / id))
		{
			fs::create_directory(receipts / id);
			return id;
		}
	}

	return "";
}

const std::string Parser::intToID(int num) const
{
	std::string result("");

	while (num > 0)
	{
		int remainder = num % 36;
		char digit = (remainder < 10) ? '0' + remainder : 'a' + remainder - 10;
		result += digit;
		num /= 36;
	}

	reverse(result.begin(), result.end());
	return result;
}

const std::optional<Receipt> Parser::parseReceipt(std::ifstream& json) const
{
	std::string data;

	for (std::string line; std::getline(json, line);)
	{
		data.append(trimWhiteSpace(line));
	}

	// If file does not start with a { and end with a }, then it is malformed
	if (data.front() != '{')
	{
		std::cerr << "File format invalid; file must open with a {." << std::endl;
		return null;
	}
	if (data.back() != '}')
	{
		std::cerr << "File format invalid; file must close with a }." << std::endl;
		return null;
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
		std::cerr << "File format invalid; expected \"retailer\" but not found." << std::endl;
		return null;
	}
	// Next word is the value for retailer
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	if (current.empty())
	{
		std::cerr << "File format invalid; invalid value for \"retailer\"." << std::endl;
		return null;
	}
	const std::string retailer = current;

	// Next is "purchaseDate"
	std::getline(ss, current, ':');
	if (current.compare("\"purchaseDate\"") != 0)
	{
		std::cerr << "File format invalid; expected \"purchaseDate\" but not found." << std::endl;
		return null;
	}
	// Next word is the value for purchaseDate
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	if (current.empty())
	{
		std::cerr << "File format invalid; invalid value for \"purchaseDate\". Expected format is yyyy-mm-dd." << std::endl;
		return null;
	}
	const std::optional<Date> purchaseDate = parsePurchaseDate(current);
	if (!purchaseDate.has_value()) return null;
	if (!purchaseDate.value().validate())
	{
		std::cerr << "File data invalid; Date is impossible." << std::endl;
		return null;
	}

	// Next is "purchaseTime"
	std::getline(ss, current, ':');
	if (current.compare("\"purchaseTime\"") != 0)
	{
		std::cerr << "File format invalid; expected \"purchaseTime\" but not found." << std::endl;
		return null;
	}
	// Next word is the value for purchaseTime
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	if (current.empty())
	{
		std::cerr << "File format invalid; invalid value for \"purchaseTime\". Expected format is hh:mm." << std::endl;
		return null;
	}
	const std::optional<Time> purchaseTime = parsePurchaseTime(current);
	if (!purchaseTime.has_value()) return null;
	if (!purchaseTime.value().validate())
	{
		std::cerr << "File data invalid; Time is impossible." << std::endl;
		return null;
	}

	// Next is "total"
	std::getline(ss, current, ':');
	if (current.compare("\"total\"") != 0)
	{
		std::cerr << "File format invalid; expected \"total\" but not found." << std::endl;
		return null;
	}
	// Next word is the value for total
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	std::regex totalFormat("[0-9]+.[0-9]{2}");
	if (!std::regex_match(current, totalFormat))
	{
		std::cerr << "File format invalid; invalid value for \"total\". Expected format includes cents even for exact dollar amounts." << std::endl;
		return null;
	}
	const double total = std::stod(current);

	// Next is "items"
	std::getline(ss, current, ':');
	if (current.compare("\"items\"") != 0)
	{
		std::cerr << "File format invalid; expected \"items\" but not found." << std::endl;
		return null;
	}
	// Next is the value for items
	std::getline(ss, current);
	current = trimWhiteSpace(current);
	if (current.front() != '[' ||
		current.back() != ']')
	{
		std::cerr << "File format invalid; expected items to begin with [ and end with ]." << std::endl;
		return null;
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
				std::cerr << "File format invalid; items should be separated by commas." << std::endl;
				return null;
			}
			current.erase(current.begin()); // Remove comma at start
		}
		if (current.front() != '{')
		{
			std::cerr << "File format invalid; each item should begin with {." << std::endl;
			return null;
		}
		// Setup for current iteration
		const size_t end = current.find_first_of('}');
		const std::string itemStr = current.substr(1, end - 1);

		const std::optional<Item> item = parseItem(itemStr);
		if (item.has_value()) items.push_back(item.value());

		// Setup for next iteration
		current = current.substr(end + 1);
		first = false;
	}

	// Items list should not be empty
	if (items.empty())
	{
		std::cerr << "File data invalid; no items in receipt." << std::endl;
		return null;
	}

	Receipt result(retailer, purchaseDate.value(), purchaseTime.value(), items, total);
	if (!result.validate())
	{
		// Should only happen if total doesn't add up
		std::cerr << "File data invalid; total doesn't match item data." << std::endl;
		return null;
	}
	return result;
}

const std::optional<Date> Parser::parsePurchaseDate(const std::string& dateStr) const
{
	// Make sure format is correct
	std::regex dateFormat("[0-9]{4,}-[0-9]{2}-[0-9]{2}");
	if (!std::regex_match(dateStr, dateFormat))
	{
		std::cerr << "File format invalid; invalid value for \"purchaseDate\". Expected format is yyyy-mm-dd." << std::endl;
		return null;
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

const std::optional<Time> Parser::parsePurchaseTime(const std::string& timeStr) const
{
	// Make sure format is correct
	std::regex timeFormat("[0-9]{2}:[0-9]{2}");
	if (!std::regex_match(timeStr, timeFormat))
	{
		std::cerr << "File format invalid; invalid value for \"purchaseTime\". Expected format is hh:mm." << std::endl;
		return null;
	}

	std::stringstream ss(timeStr);

	std::string hour;
	std::getline(ss, hour, ':');

	std::string minute;
	std::getline(ss, minute);

	return Time(std::stoi(hour), std::stoi(minute));
}

const std::optional<Item> Parser::parseItem(const std::string& itemStr) const
{
	std::stringstream ss(itemStr);
	std::string current;
	
	// Expect first word to be "shortDescription"
	std::getline(ss, current, ':');
	if (current.compare("\"shortDescription\"") != 0)
	{
		std::cerr << "File format invalid; expected \"shortDescription\" but not found." << std::endl;
		return null;
	}
	// Next is value for shortDescription
	std::getline(ss, current, ',');
	current = trimQuotes(current);
	if (current.empty())
	{
		std::cerr << "File format invalid; invalid value for \"shortDescription\"." << std::endl;
		return null;
	}
	const std::string desc = current;

	// Next is "price"
	std::getline(ss, current, ':');
	current = trimWhiteSpace(current);
	if (current.compare("\"price\"") != 0)
	{
		std::cerr << "File format invalid; expected \"price\" but not found." << std::endl;
		return null;
	}
	// Next is value for price
	std::getline(ss, current);
	current = trimQuotes(current);
	std::regex priceFormat("[0-9]+.[0-9]{2}");
	if (!std::regex_match(current, priceFormat))
	{
		std::cerr << "File format invalid; invalid value for \"price\". Expected format includes cents even for exact dollar amounts." << std::endl;
		return null;
	}
	const double price = std::stod(current);

	return Item(desc, price);
}

// Define private helper member functions for GET
const std::string Parser::parseScore(std::ifstream& json) const
{
	std::string data;
	
	for (std::string line; std::getline(json, line);)
	{
		data.append(trimWhiteSpace(line));
	}
	const std::string result = data;

	// File should begin with { and end with }
	if (data.front() != '{' ||
		data.back() != '}') return "";

	data.erase(data.begin());
	data.pop_back();
	std::stringstream ss(trimWhiteSpace(data));
	std::string current;

	// First word should be "score"
	std::getline(ss, current, ':');
	current = trimQuotes(current);
	if (current.compare("points") != 0)
	{
		return "";
	}

	// Second word should be a number
	std::getline(ss, current);
	current = trimWhiteSpace(current);
	for (const char& c : current)
	{
		if (!std::isdigit(c)) return "";
	}

	return result;
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