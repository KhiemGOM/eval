#include <iostream>
#include <regex>
#include "function.h"
#include "token.h"
#include "eval.h"
int main()
{
    std::string input;
    std::getline(std::cin, input);
//    auto t = token::tokenize(input);
//    for (const auto &token: eval_t::simplify_sign_unary_operator(t))
//    {
//        std::cout << token.value << ' ';
//    }
    std::cout << eval_t::eval(token::tokenize(input));
}