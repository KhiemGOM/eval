//
// Created by DELL on 20/08/2023.
//

#ifndef EVAL_H
#define EVAL_H

#include <vector>
#include "token.h"
class eval_t
{
private:
    template<typename T, typename Iter>
    static std::vector<T> subVector(Iter start, Iter end)
    {
        return std::vector<T>(start, end);
    }
    template<typename T, typename Iter, typename V>
    static void replace(std::vector<T> &v, Iter start, Iter end, const V &replacement)
    {
        v.erase(start, end);
        v.insert(start, replacement);
    }
    template<typename T, typename Iter>
    [[maybe_unused]] static void replace(std::vector<T> &v, Iter start, Iter end, const std::vector<T> &replacement)
    {
        v.erase(start, end);
        v.insert(start, replacement.begin(), replacement.end());
    }
    static double eval_function(auto func, const std::vector<double> &inputs)
    {
        return (**func)(inputs);
    }
    template<typename T, typename P>
    static auto to_iterator(const std::vector<T> &v, P val)
    {
        auto it = v.begin();
        std::advance(it, std::distance(&*v.begin(), val));
        return it;
    }
    static auto get_parenthesis(const std::vector<token> &tokens)
    {
        auto open = tokens.end();
        for (const auto &k: tokens)
        {
            if (k.type == token::open_parenthesis)
            {
                open = to_iterator(tokens, &k);
            }
            if (k.type == token::close_parenthesis)
            {
                return std::make_pair(open, to_iterator(tokens, &k));
            }
        }
        return std::make_pair(open, tokens.end());
    }
    static auto get_first_of_type(const std::vector<token> &tokens, token::token_type type)
    {
        for (const auto &k: tokens)
        {
            if (k.type == type)
            {
                return to_iterator(tokens, &k);
            }
        }
        return tokens.end();
    }

    static bool is_integral(token::token_type type)
    {
        return type == token::number || type == token::constant;
    }
    static auto get_function(const token &f)
    {
        return std::ranges::find_if(list_of_functions, [&f](const auto &func)
        {
            return func->notation == f.value;
        });
    }
    static auto get_constant(const token &c)
    {
        return std::ranges::find_if(list_of_constants, [&c](const auto &constant)
        {
            return constant.notation == c.value;
        });
    }

    static double to_double(const token &t)
    {
        if (t.type == token::number)
        {
            return std::stod(t.value);
        }
        else if (t.type == token::constant)
        {
            auto constant = get_constant(t);
            if (constant == list_of_constants.end())
            {
                throw std::runtime_error(unknown_symbol_error_message.data());
            }
            return constant->value;
        }
        else
        {
            throw std::runtime_error(unknown_symbol_error_message.data());
        }
    }

    //Error messages when eval
    static constexpr const std::string_view missing_parenthesis_error_message = "Missing parenthesis";
    static constexpr const std::string_view unknown_symbol_error_message = "Unknown symbol";
    static constexpr const std::string_view invalid_expression_error_message = "Invalid expression";
public:

    static std::vector<token> &simplify_sign_unary_operator(std::vector<token> &tokens)
    {
        int sign = 1;
        bool is_prev_binary_op = false;
        auto first_binary_op = tokens.begin();
        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            if (it->type == token::additive_binary_operator)
            {
                if (!is_prev_binary_op)
                {
                    first_binary_op = it;
                }
                is_prev_binary_op = true;
                if (it->value == "+")
                {
                    sign *= 1;
                }
                else if (it->value == "-")
                {
                    sign *= -1;
                }
            }
            else if (is_prev_binary_op)
            {
                is_prev_binary_op = false;
                if (sign == -1)
                {
                    replace(tokens, first_binary_op, it, token("-"));
                    it = first_binary_op;
                }
                else
                {
                    replace(tokens, first_binary_op, it, token("+"));
                    it = first_binary_op;
                }
                sign = 1;
            }
        }
        return tokens;
    }
/*
     *  Recursive algorithm
     *  1. Evaluate expression inside closest parenthesis
     *  2. Evaluate function
     *  3. Evaluate unary operator
     *  4. Evaluate ^ operator
     *  5. Evaluate * and / operator
     *  6. Evaluate + and - operator
     */
    static double eval(std::vector<token> tokens)
    {
        //Parenthesis
        auto [open, close] = get_parenthesis(tokens);
        if (open != tokens.end() && close == tokens.end() || open == tokens.end() && close != tokens.end())
        {
            throw std::runtime_error(missing_parenthesis_error_message.data());
        }
        else if (open != tokens.end() && close != tokens.end())
        {
            if ((open - 1)->type == token::function)
            {
                //Get all parameter of the function
                //Get all subvector between delimiter comma between the 2 parenthesis
                std::vector<std::vector<token>> parameters;
                std::vector<token> temp;
                for (auto it = open + 1; it <= close; ++it)
                {
                    if (it->type == token::comma || it->type == token::close_parenthesis)
                    {
                        parameters.push_back(temp);
                        temp.clear();
                    }
                    else
                    {
                        temp.push_back(*it);
                    }
                }
                //Evaluate each parameter first
                std::vector<double> parameters_value;
                std::ranges::transform(parameters, std::back_inserter(parameters_value), [](const auto &a)
                { return eval(a); });
                //Evaluate the whole function
                auto func = get_function(*(open - 1));
                if (func == list_of_functions.end())
                {
                    throw std::runtime_error(unknown_symbol_error_message.data());
                }
                replace(tokens,
                        open - 1,
                        close + 1,
                        token(eval_function(func, parameters_value)));
                return eval(tokens);
            }
            if (is_integral((open - 1)->type))
            {
                //Multiplication shorthand
                //Multiply the whole parenthesis with the number before binary_op
                replace(tokens,
                        open - 1,
                        close + 1,
                        token(eval(subVector<token>(open + 1, close)) * to_double(*(open - 1))));
                return eval(tokens);
            }
            replace(tokens, open, close + 1, token(eval(subVector<token>(open + 1, close))));
            return eval(tokens);
        }
        //Simplify all sign unary operator

        tokens = simplify_sign_unary_operator(tokens);
        //Pre sign unary operator
        auto binary_op = get_first_of_type(tokens, token::additive_binary_operator);
        if (binary_op != tokens.end())
        {
            if ((binary_op == tokens.begin() || !is_integral((binary_op - 1)->type))
                && is_integral((binary_op + 1)->type))
            {
                //Unary +/-
                if (binary_op->value == "+")
                {
                    replace(tokens,
                            binary_op,
                            binary_op + 2,
                            token(to_double(*(binary_op + 1))));
                }
                else if (binary_op->value == "-")
                {
                    replace(tokens,
                            binary_op,
                            binary_op + 2,
                            token(-to_double(*(binary_op + 1))));
                }
                return eval(tokens);
            }
        }
        //Exponentiation
        binary_op = get_first_of_type(tokens, token::exponentiation_binary_operator);
        if (binary_op != tokens.end())
        {
            if (binary_op == tokens.begin() || binary_op == tokens.end() - 1)
            {
                throw std::runtime_error(invalid_expression_error_message.data());
            }
            if (!is_integral((binary_op - 1)->type) || !is_integral((binary_op + 1)->type))
            {
                throw std::runtime_error(invalid_expression_error_message.data());
            }
            replace(tokens,
                    binary_op - 1,
                    binary_op + 2,
                    token(std::pow(to_double(*(binary_op - 1)), to_double(*(binary_op + 1)))));
            return eval(tokens);
        }
        //Multiplication shorthand for constant
        auto c = get_first_of_type(tokens, token::constant);
        if (c != tokens.end())
        {
            if (c != tokens.begin() && (c - 1)->type == token::number)
            {
                replace(tokens,
                        c - 1,
                        c + 1,
                        token(to_double(*(c - 1)) * to_double(*c)));
                return eval(tokens);
            }
        }
        //Multiplication and division
        binary_op = get_first_of_type(tokens, token::multiplicative_binary_operator);
        if (binary_op != tokens.end())
        {
            if (binary_op == tokens.begin() || binary_op == tokens.end() - 1)
            {
                throw std::runtime_error(invalid_expression_error_message.data());
            }
            if (!is_integral((binary_op - 1)->type) || !is_integral((binary_op + 1)->type))
            {
                throw std::runtime_error(invalid_expression_error_message.data());
            }
            if (binary_op->value == "*")
            {
                replace(tokens,
                        binary_op - 1,
                        binary_op + 2,
                        token(to_double(*(binary_op - 1)) * to_double(*(binary_op + 1))));
            }
            else if (binary_op->value == "/")
            {
                replace(tokens,
                        binary_op - 1,
                        binary_op + 2,
                        token(to_double(*(binary_op - 1)) / to_double(*(binary_op + 1))));
            }
            return eval(tokens);
        }
        //Addition and subtraction
        binary_op = get_first_of_type(tokens, token::additive_binary_operator);
        if (binary_op != tokens.end())
        {
            if ((binary_op == tokens.begin() || !is_integral((binary_op - 1)->type))
                && is_integral((binary_op + 1)->type))
            {
                //Unary +/-
                if (binary_op->value == "+")
                {
                    replace(tokens,
                            binary_op,
                            binary_op + 2,
                            token(to_double(*(binary_op + 1))));
                }
                else if (binary_op->value == "-")
                {
                    replace(tokens,
                            binary_op,
                            binary_op + 2,
                            token(-to_double(*(binary_op + 1))));
                }
            }
            if (!is_integral((binary_op - 1)->type) || !is_integral((binary_op + 1)->type))
            {
                throw std::runtime_error(invalid_expression_error_message.data());
            }
            if (binary_op->value == "+")
            {
                replace(tokens,
                        binary_op - 1,
                        binary_op + 2,
                        token(to_double(*(binary_op - 1)) + to_double(*(binary_op + 1))));
            }
            else if (binary_op->value == "-")
            {
                replace(tokens,
                        binary_op - 1,
                        binary_op + 2,
                        token(to_double(*(binary_op - 1)) - to_double(*(binary_op + 1))));
            }
            return eval(tokens);
        }
        //Pass all checks -> must be irreducible
        if (tokens.size() == 1)
        {
            if (tokens[0].type == token::number)
            {
                return to_double(tokens[0]);
            }
            else if (tokens[0].type == token::constant)
            {
                auto constant = get_constant(tokens[0]);
                if (constant == list_of_constants.end())
                {
                    throw std::runtime_error(unknown_symbol_error_message.data());
                }
                return constant->value;
            }
            else
            {
                throw std::runtime_error(unknown_symbol_error_message.data());
            }
        }
        throw std::runtime_error(invalid_expression_error_message.data());
    }
};


#endif //EVAL_H