//
// Created by DELL on 20/08/2023.
//

#ifndef FUNCTION_BASE_H
#define FUNCTION_BASE_H

#include<type_traits>
#include <utility>
#include <cmath>
#include <memory>
#include <functional>
#include <string>
#include "function.h"
class function_base
{
public:
	virtual ~function_base() = default;
	explicit function_base(std::string notation)
		: notation(std::move(notation))
	{};
	std::string notation;
	virtual double operator()(const std::vector<double> &inputs) = 0;
};
#endif //FUNCTION_BASE_H