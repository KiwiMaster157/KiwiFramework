#include <iostream>

int main()
{
	try
	{
		
	}
	catch (std::exception& exc)
	{
		std::cerr << exc.what() << std::endl;
		std::cin.ignore();
	}

	return 0;
}
