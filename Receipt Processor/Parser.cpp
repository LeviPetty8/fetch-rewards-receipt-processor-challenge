// Standard libraries
#include <iostream>

// rapidjson libraries
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

// Program files
#include "Parser.h"

namespace json = rapidjson;

// Define static member variables
const fs::path Parser::RECEIPTS(fs::current_path() / "receipts");

// Define public member functions

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
	const Receipt receipt(retailer, pdate, ptime, items, total);
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

void Parser::GET(const std::string& id) const
{
	const fs::path file(RECEIPTS / id / "points.json");

	if (!fs::exists(file))
	{
		std::cerr << std::format("ID {} not found.", id) << std::endl;
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

	// Output result
	const int result = doc["points"].GetInt();
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