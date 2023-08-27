//
// Created by DELL on 20/08/2023.
//

#ifndef FUNCTION_H
#define FUNCTION_H


#include <string>
#include <functional>
#include <memory>
#include <cmath>
#include <utility>
#include<type_traits>
#include "function_base.h"
#include <format>

template<class T>
auto move_to_unique(T &&t)
{
	return std::make_unique<std::remove_reference_t<T>>(std::move(t)); // NOLINT(*-move-forwarding-reference)
}
template<class V, class ... Args>
auto make_vector_unique(Args ... args)
{
	std::vector<std::unique_ptr<V>> rv;
	(rv.push_back(move_to_unique(args)), ...);
	return rv;
}

template<typename ...Input>
class function: public function_base
{
public:
	function() = default;
	function(const function &val) = default;
	function(function &&val) = default;
	function(std::string notation, std::function<double(Input...)> func)
		: function_base(std::move(notation)), func(std::move(func))
	{}
	function &operator=(const function &other) = default;
	function &operator=(function &&other) = default;
	~function() override = default;

	//Fields
	std::function<double(Input...)> func;
	static constexpr const std::string_view number_of_inputs_error_message = "{} requires {} parameters";

	//Methods
	static std::size_t get_number_of_inputs()
	{
		return sizeof...(Input);
	}
	double operator()(Input... input)
	{
		return func(input...);
	}
	double operator()(const std::vector<double> &inputs) override
	{
		if (inputs.size() != get_number_of_inputs())
		{
			throw std::runtime_error(std::format(number_of_inputs_error_message, notation, get_number_of_inputs()));
		}
		return apply(std::index_sequence_for<Input...>(), inputs);
	}
private:
	template<std::size_t... Is>
	double apply(std::index_sequence<Is...>, const std::vector<double> &inputs)
	{
		return func(inputs[Is]...);
	}
};

static const std::vector<std::unique_ptr<function_base>> list_of_functions = make_vector_unique<function_base>(
	function<double>("sin", [](double input)
	{ return std::sin(input); }),
	function<double>("cos", [](double input)
	{ return std::cos(input); }),
	function<double>("tan", [](double input)
	{ return std::tan(input); }),
	function<double>("asin", [](double input)
	{ return std::asin(input); }),
	function<double>("acos", [](double input)
	{ return std::acos(input); }),
	function<double>("atan", [](double input)
	{ return std::atan(input); }),
	function<double>("sinh", [](double input)
	{ return std::sinh(input); }),
	function<double>("cosh", [](double input)
	{ return std::cosh(input); }),
	function<double>("tanh", [](double input)
	{ return std::tanh(input); }),
	function<double>("asinh", [](double input)
	{ return std::asinh(input); }),
	function<double>("acosh", [](double input)
	{ return std::acosh(input); }),
	function<double>("atanh", [](double input)
	{ return std::atanh(input); }),
	function<double>("exp", [](double input)
	{ return std::exp(input); }),
	function<double>("log10", [](double input)
	{ return std::log10(input); }),
	function<double>("log2", [](double input)
	{ return std::log2(input); }),
	function<double>("ln", [](double input)
	{ return std::log(input); }),
	function<double>("sqrt", [](double input)
	{ return std::sqrt(input); }),
	function<double>("cbrt", [](double input)
	{ return std::cbrt(input); }),
	function<double>("ceil", [](double input)
	{ return std::ceil(input); }),
	function<double>("floor", [](double input)
	{ return std::floor(input); }),
	function<double>("abs", [](double input)
	{ return std::abs(input); }),
	function<double>("round", [](double input)
	{ return std::round(input); }),
	function<double>("trunc", [](double input)
	{ return std::trunc(input); }),
	function<double>("sign", [](double input)
	{ return std::signbit(input) ? -1 : 1; }),
	function<double>("gamma", [](double input)
	{ return std::tgamma(input); }),
	function<double>("factorial", [](double input)
	{ return std::tgamma(input + 1); }),
	function<double>("erf", [](double input)
	{ return std::erf(input); }),
	function<double>("erfc", [](double input)
	{ return std::erfc(input); }),
	function<double, double>("pow", [](double input1, double input2)
	{ return std::pow(input1, input2); }),
	function<double, double>("hypot", [](double input1, double input2)
	{ return std::hypot(input1, input2); }),
	function<double, double>("mod", [](double input1, double input2)
	{ return std::fmod(input1, input2); }),
	function<double, double>("atan2", [](double input1, double input2)
	{ return std::atan2(input1, input2); }),
	function<double, double>("max", [](double input1, double input2)
	{ return std::max(input1, input2); }),
	function<double, double>("min", [](double input1, double input2)
	{ return std::min(input1, input2); }),
	function<double, double>("log", [](double input1, double input2)
	{ return std::log(input2) / std::log(input1); }),
	function<double, double>("root", [](double input1, double input2)
	{ return std::pow(input1, 1 / input2); }),
	function<double, double, double>("lerp", [](double input1, double input2, double input3)
	{ return std::lerp(input1, input2, input3); }),
	function<double, double, double>("clamp", [](double input1, double input2, double input3)
	{ return std::clamp(input1, input2, input3); })
);

#endif //FUNCTION_H