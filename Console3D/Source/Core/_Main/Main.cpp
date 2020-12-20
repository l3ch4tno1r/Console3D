#include <iostream>

#include "Core/Application.h"

int main(int argc, char** argv)
{
	try
	{
		Application::Get().Run();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error occured : " << e.what() << std::endl;

		std::cin.get();

		return EXIT_FAILURE;
	}
}