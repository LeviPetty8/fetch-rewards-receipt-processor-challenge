#include <format>
#include "Time.h"
#include "Date.h"

int main()
{
	Date birthday(9600, 2, 29);
	Time birthtime(24, 59);

	/*std::cout << "Date is " << birthday << std::endl << "Time is " << birthtime << std::endl;
	std::cout << birthday.validate() << " " << birthtime.validate();*/
	std::cout << std::format("Date is {}\nTime is {}.", birthday.to_string(), birthtime.to_string());
}