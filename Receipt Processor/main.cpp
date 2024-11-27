#include "Receipt.h"

int main()
{
	/*Date birthday(7600, 2, 29);
	Time birthtime(22, 59);

	std::cout << std::format("Date is {}\nTime is {}.", birthday.to_string(), birthtime.to_string());*/

	Receipt rec(
		"Target",
		Date(2022, 1, 1),
		Time(13, 1),
		{
			Item("Mountain Dew 12PK", 6.49),
			Item("Emils Cheese Pizza", 12.25),
			Item("Knorr Creamy Chicken", 1.26),
			Item("Doritos Nacho Cheese", 3.35),
			Item("   Klarbrunn 12-PK 12 FL OZ  ", 12.00)
		},
		35.35
	);

	std::cout << rec.retailer << " " << rec.calculate_points() << std::endl;
	
	Receipt rec2(
		"M&M Corner Market",
		Date(2022, 3, 20),
		Time(14, 33),
		{
			Item("Gatorade", 2.25),
			Item("Gatorade", 2.25),
			Item("Gatorade", 2.25),
			Item("Gatorade", 2.25)
		},
		9.00
	);

	std::cout << rec2.retailer << " " << rec2.calculate_points() << std::endl;
}