#include "Parser.h"

//namespace fs = std::filesystem;

int main()
{
	const Parser P;
	//const fs::path RECEIPTS(fs::current_path() / "receipts");

	//const fs::path PROCESS(RECEIPTS / "process");

	std::cout << "Usage: \n"
		"	POST {filename} - Generates an id for the receipt in receipts/process/{filename} and creates a directory receipts/{id} and a new file in that directory that stores the point value of the receipt.\n"
		"	GET {id} - Looks up the point value for the receipt with id {id}.\n"
		"	exit - Exits the program.\n"
		"No command / blank entry also exits the program.\n";

	while (true)
	{
		// Take in command
		std::string command;
		std::cout << ">> ";
		std::getline(std::cin, command);

		std::stringstream ss(command);

		// Parse command
		ss >> command;
		if (command.compare("POST") == 0)
		{
			ss >> command;
			P.POST(command);
		}
		else if (command.compare("exit") == 0 || command.empty())
		{
			break;
		}
		else
		{
			std::cout << std::format("Command {} not recognized.", command) << std::endl;
		}
	}

	//const fs::path file = process / "morning-receipt.json";

	//std::fstream fin(file);
	//Receipt r = p.parseReceipt(fin);

	

	return 0;
}