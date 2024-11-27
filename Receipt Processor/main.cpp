#include "Parser.h"

int main()
{
	const std::string path = "C:/Users/levip/Documents/GitHub/fetch-rewards-receipt-processor-challenge/Receipt Processor/receipts/process";

	Parser p;

	const std::string filename = path + "/simple-receipt.json";

	std::fstream fin(filename);
	Receipt r = p.parseReceipt(fin);

	return 0;
}