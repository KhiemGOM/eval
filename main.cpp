#include <iostream>
#include <regex>
#include "token.h"
#include "eval.h"
#include <thread>
#include "Windows.h"


int main()
{
	while (true)
	{
		std::string input;
		std::getline(std::cin, input);
		for (const auto &token: token::tokenize(input))
		{
			std::cout << token.value << ' ' << token.type << std::endl;
		}
		try
		{
			std::cout << eval_t::eval(token::tokenize(input)) << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Error: " << e.what() << std::endl;
		}
	}
}