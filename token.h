//
// Created by DELL on 20/08/2023.
//

#ifndef TOKEN_H
#define TOKEN_H


#include <string>
#include <utility>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <sstream>
#include "function.h"
#include "constant.h"
;

static const std::vector<char> list_of_binary_operator = {'+', '-', '*', '/', '^'};

static const std::vector<char> list_of_post_unary_operator = {'!'};

class token
{

public:
	enum token_type
	{
		unknown,
		number,
		constant,
		additive_binary_operator,
		multiplicative_binary_operator,
		exponentiation_binary_operator,
		post_unary_operator,
		open_parenthesis,
		close_parenthesis,
		comma,
		function
	};
	//Constructors
	token() = default;
	token(const token &val) = default;
	token(token &&val) = default;
	explicit token(const token_type &type, std::string value)
		: type(type), value(std::move(value))
	{};
	explicit token(const std::string &value);
	explicit token(double val)
		: type(number), value(std::to_string(val))
	{};
	token &operator=(const token &other) = default;
	token &operator=(token &&other) = default;
	~token() = default;

	//Fields
	token_type type;
	std::string value;
	static const constexpr std::string_view unknown_symbol_error_message = "Unknown symbol: {}";

	//Const var
	constexpr static const std::string_view
		token_type_mismatch_error_message = "Cannot combine token of different type";

	//Operator
	token &operator+=(const token &other);
	friend token operator+(token lhs, const token &rhs);

	//Methods

	enum delimiter_separation
	{
		words_and_numbers,
		single_character
	};

	static delimiter_separation get_delimiter_separation(const char &k);
	static std::vector<token> tokenize(const std::string &str);
private:
	[[nodiscard]] std::vector<token> separateWordsAndNumberToken() const;
	static void tokenize_words_and_numbers(std::vector<token> &tokens, std::string &temp);
};

#endif //TOKEN_H