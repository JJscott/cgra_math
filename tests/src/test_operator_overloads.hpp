
#include <cgra_math.hpp>

#include "test_suite.hpp"


template <typename T>
bool equality_identity() {
	T a = random<T>(T(-1), T(1));
	return a == a;
}

template <typename T>
bool inequality_non_identity() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	return a != b;
}

template <typename T>
bool negation_double_negation() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(-(-a), a);
}

template <typename T>
bool addition_commutativity() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	return almost_equal(a+b, b+a);
}

template <typename T>
bool addition_associativity() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	T c = random<T>(T(-1), T(1));
	return almost_equal(a+(b+c), (a+b)+c);
}

template <typename T>
bool addition_identity() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(a+T(0), a);
}

template <typename T>
bool subtraction_anticommutativity() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	return almost_equal(a-b, -(b-a));
}

template <typename T>
bool subtraction_nonassociativity() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	return !almost_equal(a-(b-T(1)), (a-b)-T(1));
}

template <typename T>
bool subtraction_identity() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(a-T(0), a);
}

template <typename T>
bool subtraction_negation() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(T(0)-a, -a);
}

template <typename T>
bool multiplication_commutativity() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	return almost_equal(a*b, b*a);
}

template <typename T>
bool multiplication_associativity() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	T c = random<T>(T(-1), T(1));
	return almost_equal(a*(b*c), (a*b)*c);
}

template <typename T>
bool multiplication_distributive() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	T c = random<T>(T(-1), T(1));
	return almost_equal(a*(b+c), a*b+a*c);
}

template <typename T>
bool multiplication_identity() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(a*T(1), a);
}

template <typename T>
bool multiplication_zero_property() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(a*T(0), T(0));
}

template <typename T>
bool multiplication_negation() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(-T(1)*-T(1)*a, a);
}

template <typename T>
bool multiplication_inverse() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(a*(T(1)/a), T(1));
}

template <typename T>
bool division_right_distributive() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	T c = random<T>(T(-1), T(1));
	return almost_equal((a+b)/c, a/c + b/c);
}

template <typename T>
bool division_left_antidistributive() {
	T a = random<T>(T(-1), T(1));
	T b = random<T>(T(-1), T(1));
	T c = random<T>(T(-1), T(1));
	return !almost_equal(a/(b+c), a/b + a/c);
}



template <typename T>
void test_operator_overloads() {
	execute_test( () [] { return equality_identity<T>(); }, "equality_identity" );
	execute_test( () [] { return inequality_non_identity<T>(); }, "inequality_non_identity" );
	execute_test( () [] { return negation_double_negation<T>(); }, "negation_double_negation" );
	execute_test( () [] { return addition_commutativity<T>(); }, "addition_commutativity" );
	execute_test( () [] { return addition_associativity<T>(); }, "addition_associativity" );
	execute_test( () [] { return addition_identity<T>(); }, "addition_identity" );
	execute_test( () [] { return subtraction_anticommutativity<T>(); }, "subtraction_anticommutativity" );
	execute_test( () [] { return subtraction_nonassociativity<T>(); }, "subtraction_nonassociativity" );
	execute_test( () [] { return subtraction_identity<T>(); }, "subtraction_identity" );
	execute_test( () [] { return subtraction_negation<T>(); }, "subtraction_negation" );
	execute_test( () [] { return multiplication_commutativity<T>(); }, "multiplication_commutativity" );
	execute_test( () [] { return multiplication_associativity<T>(); }, "multiplication_associativity" );
	execute_test( () [] { return multiplication_distributive<T>(); }, "multiplication_distributive" );
	execute_test( () [] { return multiplication_identity<T>(); }, "multiplication_identity" );
	execute_test( () [] { return multiplication_zero_property<T>(); }, "multiplication_zero_property" );
	execute_test( () [] { return multiplication_negation<T>(); }, "multiplication_negation" );
	execute_test( () [] { return multiplication_inverse<T>(); }, "multiplication_inverse" );
	execute_test( () [] { return division_right_distributive<T>(); }, "division_right_distributive" );
	execute_test( () [] { return division_left_antidistributive<T>(); }, "division_left_antidistributive" );
	// TODO
	// += -= *= /= %= <<= >>= |= ^= &=
	// % << >> ! || && ~ | ^ & <
}