//
// Created by DELL on 20/08/2023.
//

#include "token.h"
std::vector<token> token::tokenize(const std::string &str)
{
    std::string word;
    std::stringstream ss(str);
    std::vector<token> tokens;
    while (ss >> word)
    {
        std::string temp;
        for (const auto &k: word)
        {
            if (get_delimiter_separation(k) == words_and_numbers)
            {
                temp += k;
            }
            else
            {
                if (!temp.empty())
                {
                    tokens.emplace_back(temp);
                    temp.clear();
                }
                if (get_delimiter_separation(k) == single_character)
                {
                    tokens.emplace_back(std::string(1, k));
                }
            }
        }
        if (!temp.empty())
        {
            tokens.emplace_back(temp);
            temp.clear();
        }
    }
    return tokens;
}
token &token::operator+=(const token &other)
{
    if (type != other.type)
    {
        throw std::runtime_error(token_type_mismatch_error_message.data());
    }
    value += other.value;
    return *this;
}
token::token(const std::string &value)
{
    this->value = value;
    //Find token type
    if (value == "(")
    {
        type = open_parenthesis;
        return;
    }
    if (value == ")")
    {
        type = close_parenthesis;
        return;
    }
    if (value == ",")
    {
        type = comma;
        return;
    }
    else if (value.length() == 1)
    {
        if (std::ranges::find(list_of_binary_operator, value[0]) != list_of_binary_operator.end())
        {
            if (value[0] == '+' || value[0] == '-')
            {
                type = additive_binary_operator;
            }
            else if (value[0] == '*' || value[0] == '/')
            {
                type = multiplicative_binary_operator;
            }
            else if (value[0] == '^')
            {
                type = exponentiation_binary_operator;
            }
            else
            {
                type = unknown;
            }
            return;
        }
        if (std::ranges::find(list_of_post_unary_operator, value[0]) != list_of_post_unary_operator.end())
        {
            type = post_unary_operator;
            return;
        }
    }
    try
    {
        std::stod(value);
        type = number;
        return;
    }
    catch (...)
    {
        type = unknown;
    }
    if (std::ranges::all_of(value, [](char c)
    { return std::isalpha(static_cast<unsigned char>(c)) || std::isdigit(static_cast<unsigned char>(c)); }))
    {
        if (std::ranges::find_if(list_of_constants, [&value](const class constant &c)
        { return c.notation == value; }) != list_of_constants.end())
        {
            type = constant;
            return;
        }
        else if(std::ranges::find_if(list_of_functions, [&value](const std::unique_ptr<function_base> &f)
        { return f->notation == value; }) != list_of_functions.end())
        {
            type = function;
            return;
        }
    }
    else
    {
        type = unknown;
        return;
    }
}
token operator+(token lhs, const token &rhs)
{
    if (lhs.type != rhs.type)
    {
        throw std::runtime_error(token::token_type_mismatch_error_message.data());
    }
    return lhs += rhs;
}
token::delimiter_separation token::get_delimiter_separation(const char &k)
{
    if (std::isalpha(static_cast<unsigned char>(k)) || std::isdigit(static_cast<unsigned char>(k)) || k == '.')
    {
        return words_and_numbers;
    }
    else
    {
        return single_character;
    }
}