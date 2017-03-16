#include <iostream>

#include <cgra_math.hpp>

#include "test_suite.hpp"
#include "test_common_functions.hpp"
#include "test_angle_trigonometry_functions.hpp"
#include "test_exponential_functions.hpp"
#include "test_operator_overloads.hpp"

int main() {
	test_common_functions<vecT>();
	test_angle_trigonometry_functions<vecT>();
	test_exponential_functions<vecT>();
	test_operator_overloads<vecT>();
}