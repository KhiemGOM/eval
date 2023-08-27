#include <iostream>
#include <regex>
#include "token.h"
#include "eval.h"
#include <thread>
#include "Windows.h"

void detect_esc()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			exit(0);
		}
	}
}

int main()
{
	std::thread t(detect_esc);
	if (t.joinable())
	{
		t.detach();
	}
	std::string input;
	while (true)
	{
		std::getline(std::cin, input);
//	for (const auto &token: token::tokenize(input))
//	{
//		std::cout << token.value << ' ' << token.type << std::endl;
//	}
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