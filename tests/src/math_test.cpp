
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <typeinfo>

#include <cgra_math.hpp>
#include "math_test.hpp"


using namespace std;
using namespace cgra;
using namespace test;


int total = 1000;

template<typename T, size_t N>
void floating_point_vec_test() {

	using vec_t = basic_vec<T, N>;

	unordered_map<string, unsigned> m;

	// Function that adds to map for given bool (if false)
	auto test_true  = [&](bool a, string s) {
		if (!a) m[s] = m[s]+1;
		return a;
	};

	// Function that adds to map for given bool (if false)
	auto test_false  = [&](bool a, string s) {
		if (a) m[s] = m[s]+1;
		return !a;
	};



	for (int i = 0; i < total; ++i) {

		// random vectors
		vec_t a = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t c = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t na = normalize(a);
		vec_t nb = normalize(b);
		vec_t nc = normalize(c);

		vec_t pos = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t neg = random<vec_t>(vec_t(-1), vec_t(1));

		// random scalars
		T sa = random<T>(-1, 1);
		T sb = random<T>(-1, 1);
		// T sc = random<T>(-1, 1);
		// T sd = random<T>(-1, 1);
		
		// set vectors
		vec_t zero = vec_t();
		vec_t one = vec_t(1);

		// set scalars
		// T sz = T(0);
		T so = T(1);


		// Operator overloads
		//
		test_true(almost_equal(-(-a), a),			"negation (double negation)");

		test_true(almost_equal(a+b, b+a),			"addition (commutativity)");
		test_true(almost_equal(a+(b+c), (a+b)+c),	"addition (associativity)");
		test_true(almost_equal(a+zero, a),			"addition (identity)");

		test_true(almost_equal(a-b, -(b-a)),			"subtraction (anticommutativity)");
		test_false(almost_equal(a-(b-one), (a-b)-one),	"subtraction (nonassociativity)");
		test_true(almost_equal(a-zero, a),				"subtraction (identity)");
		test_true(almost_equal(zero-a, -a),				"subtraction (negation)");

		test_true(almost_equal(a*b, b*a),				"multiplication (commutativity)");
		test_true(almost_equal(a*(b*c), (a*b)*c),		"multiplication (associativity)");
		test_true(almost_equal(a*(sa+sb), a*sa+a*sb),	"multiplication (distributive)");
		test_true(almost_equal(a*one, a),				"multiplication (identity)");
		test_true(almost_equal(a*zero, zero),			"multiplication (zero property)");
		test_true(almost_equal(-one*-one*a, a),			"multiplication (negation)");
		test_true(almost_equal(a*(one/a), one),			"multiplication (inverse)");

		test_true(almost_equal((a+b)/c, a/c + b/c),		"division (right distributive)");
		test_false(almost_equal(a/(b+c), a/b + a/c),	"division (left antidistributive)");


		// Angle and Trigonometry Functions
		// 
		test_true(almost_equal(degrees(radians(a)), a), "degrees / radians");
		test_true(almost_equal(radians(degrees(a)), a), "radians / degrees");

		test_true(almost_equal(asin(sin(pi * a/2)), pi * a/2),	"trigonometry (sin double inverse)");
		test_true(almost_equal(acos(cos(pi * pos)), pi * pos),	"trigonometry (cos double inverse)");
		test_true(almost_equal(atan(tan(a)), a), 				"trigonometry (tan double inverse)");

		test_true(almost_equal(asinh(sinh(pi * a/2)), pi * a/2),	"trigonometry hyperbolic (sinh double inverse)");
		test_true(almost_equal(acosh(cosh(pi * pos)), pi * pos),	"trigonometry hyperbolic (cosh double inverse)");
		test_true(almost_equal(atanh(tanh(a)), a), 					"trigonometry hyperbolic (tanh double inverse)");

		test_true(almost_equal(tan(a), sin(a)/cos(a)),					"trigonometry (quotient)");
		test_true(almost_equal(one/tan(a), (one/sin(a))/(one/cos(a))),	"trigonometry (inverse quotient)");

		test_true(almost_equal(sin(a)*sin(a) + cos(a)*cos(a), one),							"trigonometry (sin-cos pythagorean)");
		test_true(almost_equal(one+tan(a)*tan(a), (one/cos(a))*(one/cos(a))),				"trigonometry (tan-sec pythagorean)");
		test_true(almost_equal(one+(one/tan(a))*(one/tan(a)), (one/sin(a))*(one/sin(a))),	"trigonometry (tan-sec pythagorean)");


		// Exponential Functions
		// 
		test_true(almost_equal(a*a, pow(a, vec_t(2))),		"pow (double, equality)");
		test_true(almost_equal(a*a*a, pow(a, vec_t(3))),	"pow (triple, equality)");

		test_true(almost_equal(log(exp(a)), a),		"exp / log (double inverse)");
		test_true(almost_equal(exp(log(pos)), pos),	"exp / log (double inverse, reverse)");

		test_true(almost_equal(log2(exp2(a)), a),		"exp2 / log2 (double inverse)");
		test_true(almost_equal(exp2(log2(pos)), pos),	"exp2 / log2 (double inverse, reverse)");

		test_true(almost_equal(sqrt(pow(pos, vec_t(2))), pos),		"pow / sqrt (double inverse)");
		test_true(almost_equal(pow(sqrt(pos), vec_t(2)), pos),		"pow / sqrt (double inverse, reverse)");
		test_true(almost_equal(1/sqrt(pos), inversesqrt(pos)),		"sqrt / inversesqrt / abs (equality)");


		// Common Functions
		// 
		test_true(almost_equal(abs(a), abs(-a)),	"abs  (absolute and negation)");
		test_true(almost_equal(sign(zero), zero),			"sign (sign of zero is zero)");
		test_true(almost_equal(sign(a)*abs(a), a),	"sign (sign multiplied by absolute)");

		test_true(all(lessThanEqual(floor(a), a)),		"floor / all / lessThanEqual (less than equal floor)");
		test_true(almost_equal(fract(floor(a)), zero),	"floor / fract (zero fractional component)");
		test_true(all(greaterThanEqual(ceil(a), a)),	"ceil / all / greaterThanEqual (greater than equal floor)");
		test_true(almost_equal(fract(ceil(a)), zero),	"ceil / fract (zero fractional component)");


		test_true(almost_equal(mod(pos+one, so), fract(pos)),	"mod scalar / fract (mod 1 fractional part)");
		test_true(almost_equal(mod(pos+one, one),  fract(pos)),	"mod vector / fract (mod 1 fractional part)");

		test_true(all(lessThanEqual(min(a,sa), a)) && all(lessThanEqual(min(a,sa), vec_t(sa))),			"min scalar (less than equality)");
		test_true(all(lessThanEqual(min(a,b), a)) && all(lessThanEqual(min(a,b), b)),					"min vector (less than equality)");
		test_true(all(greaterThanEqual(max(a,sa), a)) && all(greaterThanEqual(max(a,sa), vec_t(sa))),	"max scalar (greater than equality)");
		test_true(all(greaterThanEqual(max(a,b), a)) && all(greaterThanEqual(max(a,b), b)),				"max vector (greater than equality)");
		test_true(almost_equal(min(a,sa)+max(a,sa),  a+sa),		"min / max scalar (summation equality)");
		test_true(almost_equal(min(a,b)+max(a,b),  a+b),		"min / max vector (summation equality)");


		test_true(almost_equal(mix(a, a, sa), a),						"mix scalar (identity)");
		test_true(almost_equal(mix(a, b, sa) + mix(b, a, sa), a+b),		"mix scalar (summation equality)");
		test_true(almost_equal(mix(a, a, pos), a),						"mix scalar (identity)");
		test_true(almost_equal(mix(a, b, pos) + mix(b, a, pos), a+b),	"mix scalar (summation equality)");


	}

	// Print out incorrect results
	if (!m.empty())  cout << " == basic_vec<" << typeid(T).name() << "," << N << "> == " << endl;
	for (auto p : m) {
		const int w_ = 4;
		cout << setw(w_) << (100 * p.second / float (total)) << "% error : " << p.first << endl;
	}
}


void vector_tests() {

	
	// floating_point_vec_test<float, 1>();
	// floating_point_vec_test<float, 2>();
	floating_point_vec_test<float, 3>();
	// floating_point_vec_test<float, 4>();
	// floating_point_vec_test<float, 5>();
	// floating_point_vec_test<double, 1>();
	// floating_point_vec_test<double, 2>();
	// floating_point_vec_test<double, 3>();
	// floating_point_vec_test<double, 4>();
	// floating_point_vec_test<double, 5>();

}




//Main program
// 
void test::run_tests() {
	vector_tests();
}