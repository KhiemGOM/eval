//
// Created by DELL on 20/08/2023.
//

#ifndef CONSTANT_H
#define CONSTANT_H


#include <string>
#include <utility>
#include <vector>
class constant
{
public:
	constant() = default;
	constant(const constant &val) = default;
	constant(constant &&val) = default;
	constant(std::string notation, double value)
		: notation(std::move(notation)), value(value)
	{}
	constant &operator=(const constant &other) = default;
	constant &operator=(constant &&other) = default;
	~constant() = default;

	std::string notation;
	double value{};
};

static const std::vector<constant> list_of_constants{
	constant("pi", std::numbers::pi),
	constant("e", std::numbers::e),
	constant("c", 299792458),
	constant("G", 6.67430e-11),
	constant("h", 6.62607015e-34),
	constant("hbar", 1.054571817e-34),
	constant("mu0", 1.25663706212e-6),
	constant("Z0", 376.730313668),
	constant("epsilon0", 8.8541878128e-12),
	constant("k", 1.380649e-23),
	constant("ke", 8.9875517923e9),
	constant("lambda", 1.089e-52),
	constant("alpha", 7.2973525693e-3),
	constant("qe", 1.602176634e-19),
	constant("me", 9.1093837015e-31),
	constant("mp", 1.67262192369e-27),
	constant("mn", 1.67492749804e-27),
	constant("mmu", 1.883531627e-28),
	constant("mtau", 3.16754e-27),
	constant("mt", 3.0784 - 25),
	constant("Gf", 1.1663787e-5),
	constant("NA", 6.02214076e23),
	constant("R", 8.31446261815324),
	constant("F", 96485.3321233100184),
	constant("sigma", 5.670374419e-8),
	constant("re", 2.8179403262e-15),
	constant("m12C", 1.99264687992 - 26),
	constant("M12C", 11.9999999958e-3),
	constant("deltanuCs", 9192631770)
};

#endif //CONSTANT_H