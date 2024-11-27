#include <format>
//#include "Time.h"
//#include "Date.h"
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

	std::cout << rec.calculate_points();
}