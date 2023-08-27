#include <iostream>
#include <regex>
#include "token.h"
#include "eval.h"
int main()
{
    std::string input;
    std::getline(std::cin, input);
//    auto t = token::tokenize(input);
//    for (const auto &token: t)
//    {
//        std::cout << token.value << ' ';
//    }
    try
    {
        std::cout << eval_t::eval(token::tokenize(input));
    }
    catch(const std::exception &e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}