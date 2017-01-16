
// #include <cmath>
// #include <cstdlib>
// #include <iostream>
// #include <string>
// #include <unordered_map>
// #include <unordered_set>
// #include <random>

// #include <cgra_math.hpp>
// #include "math_test.hpp"


// using namespace std;
// using namespace cgra;


// int w_ = 4, total = 1000;
// float EPSILON = 0.001;



// // Square error metrics
// //
// template<typename T>
// auto vsqerr(const T &a, const T &b) {
// 	T c = a - b;
// 	return dot(c, c);
// }

// template<typename T>
// auto qsqerr(const T &a, const T &b) {
// 	T c = a - b;
// 	return dot(c, c);
// }


// template<typename T>
// void floating_point_vector_test(const string &header="<type-missing>") {
// 	using U = typename T::value_t;

// 	unordered_map<string, unsigned> m;
	
// 	// Function that adds to map for ((a-b)^2 < epsilon)
// 	auto test = [&](T a, T b, string s, bool f = true) {
// 		if ((vsqerr(a, b) < U(EPSILON)) != f) {
// 			m[s] = m[s]+1;
// 			return false;
// 		}
// 		return true;
// 	};

// 	// Function that adds to map for given bool (if false)
// 	auto test_bool  = [&](bool a, string s) {
// 		if (!a) m[s] = m[s]+1;
// 		return a;
// 	};


// 	for (int i = 0; i < total; ++i) {

// 		// random vectors
// 		T a = T::random(-1, 1);
// 		T b = T::random(-1, 1);
// 		T c = T::random(-1, 1);
// 		T na = normalize(a);
// 		T nb = normalize(b);
// 		T nc = normalize(c);
// 		T pos = T::random(); // [0,1)
// 		T neg = T::random(-1, 0);

// 		// random scalars
// 		U sa = random<U>();
// 		U sb = random<U>();
// 		U sc = random<U>();
// 		U sd = random<U>();
		
// 		// set vectors
// 		T z = T();
// 		T o = T(1);

// 		// set scalars
// 		U sz = U(0);
// 		U so = U(1);


// 		// Typical Math Functions
// 		//
// 		test(-(-a), a,			"negation (double negation)");

// 		test(a+b, b+a,			"addition (commutativity)");
// 		test(a+(b+c), (a+b)+c,	"addition (associativity)");
// 		test(a+z, a,			"addition (identity)");

// 		test(a-b, -(b-a),		"subtraction (anticommutativity)");
// 		test(a-(b-o), (a-b)-o,	"subtraction (nonassociativity)", false);
// 		test(a-z, a,			"subtraction (identity)");
// 		test(z-a, -a,			"subtraction (negation)");

// 		test(a*b, b*a,				"multiplication (commutativity)");
// 		test(a*(b*c), (a*b)*c,		"multiplication (associativity)");
// 		test(a*(sa+sb), a*sa+a*sb,	"multiplication (distributive)");
// 		test(a*o, a,				"multiplication (identity)");
// 		test(a*z, z,				"multiplication (zero property)");
// 		test(-o*-o*a, a,			"multiplication (negation)");
// 		test(a*(o/a), o,			"multiplication (inverse)");

// 		test((a+b)/c, a/c + b/c,	"division (right distributive)");
// 		test(a/(b+c), a/b + a/c,	"division (left antidistributive)", false);


// 		// Angle and Trigonometry Functions
// 		// 
// 		test(degrees(radians(a)), a, "degrees / radians");
// 		test(radians(degrees(a)), a, "radians / degrees");

// 		test(asin(sin(pi * a/2)), pi * a/2, "trigonometry (sin double inverse)");
// 		test(acos(cos(pi * pos)), pi * pos, "trigonometry (cos double inverse)");
// 		test(atan(tan(a)), a, 				"trigonometry (tan double inverse)");

// 		test(asinh(sinh(pi * a/2)), pi * a/2, "trigonometry hyperbolic (sinh double inverse)");
// 		test(acosh(cosh(pi * pos)), pi * pos, "trigonometry hyperbolic (cosh double inverse)");
// 		test(atanh(tanh(a)), a, 			  "trigonometry hyperbolic (tanh double inverse)");

// 		test(tan(a), sin(a)/cos(a),				"trigonometry (quotient)");
// 		test(o/tan(a), (o/sin(a))/(o/cos(a)),	"trigonometry (inverse quotient)");

// 		test(sin(a)*sin(a) + cos(a)*cos(a), o,					"trigonometry (sin-cos pythagorean)");
// 		test(o+tan(a)*tan(a), (o/cos(a))*(o/cos(a)),			"trigonometry (tan-sec pythagorean)");
// 		test(o+(o/tan(a))*(o/tan(a)), (o/sin(a))*(o/sin(a)),	"trigonometry (tan-sec pythagorean)");


// 		// Exponential Functions
// 		// 
// 		test(a*a, pow(a, T(2)),		"pow (double, equality)");
// 		test(a*a*a, pow(a, T(3)),	"pow (triple, equality)");

// 		test(log(exp(a)), a,		"exp / log (double inverse)");
// 		test(exp(log(pos)), pos,	"exp / log (double inverse, reverse)");

// 		test(log2(exp2(a)), a,		"exp2 / log2 (double inverse)");
// 		test(exp2(log2(pos)), pos,	"exp2 / log2 (double inverse, reverse)");

// 		test(sqrt(pow(pos, T(2))), pos,		"pow / sqrt (double inverse)");
// 		test(pow(sqrt(pos), T(2)), pos,		"pow / sqrt (double inverse, reverse)");
// 		test(1/sqrt(pos), inversesqrt(pos),	"sqrt / inversesqrt / abs (equality)");


// 		// Common Functions
// 		// 
// 		test(abs(a), abs(-a),	"abs  (absolute and negation)");
// 		test(sign(z), z,		"sign (sign of zero is zero)");
// 		test(sign(a)*abs(a), a,	"sign (sign multiplied by absolute)");

// 		test_bool(all(lessThanEqual(floor(a), a)),	"floor / all / lessThanEqual (less than equal floor)");
// 		test(fract(floor(a)), z,					"floor / fract (zero fractional component)");
// 		test_bool(all(greaterThanEqual(ceil(a), a)),"ceil / all / greaterThanEqual (greater than equal floor)");
// 		test(fract(ceil(a)), z,						"ceil / fract (zero fractional component)");


// 		test(mod(pos+o, so), fract(pos),	"mod scalar / fract (mod 1 fractional part)");
// 		test(mod(pos+o, o),  fract(pos),	"mod vector / fract (mod 1 fractional part)");

// 		test_bool(all(lessThanEqual(min(a,sa), a)) && all(lessThanEqual(min(a,sa), T(sa))),			"min scalar (less than equality)");
// 		test_bool(all(lessThanEqual(min(a,b), a)) && all(lessThanEqual(min(a,b), b)),				"min vector (less than equality)");
// 		test_bool(all(greaterThanEqual(max(a,sa), a)) && all(greaterThanEqual(max(a,sa), T(sa))),	"max scalar (greater than equality)");
// 		test_bool(all(greaterThanEqual(max(a,b), a)) && all(greaterThanEqual(max(a,b), b)),			"max vector (greater than equality)");
// 		test(min(a,sa)+max(a,sa),  a+sa,	"min / max scalar (summation equality)");
// 		test(min(a,b)+max(a,b),  a+b,		"min / max vector (summation equality)");


// 		test(mix(a, a, sa), a, "mix scalar (identity)");
// 		test(mix(a, b, sa) + mix(b, a, sa), a+b, "mix scalar (summation equality)");
// 		test(mix(a, a, pos), a, "mix scalar (identity)");
// 		test(mix(a, b, pos) + mix(b, a, pos), a+b, "mix scalar (summation equality)");


// 		// TODO test step and smooth step
// 		// TODO test isnan and isinf


// 		// test(dot(na, na), so, "dot normalize (identity)");
// 		// test(dot(na, -na), -so, "dot normalize (negitive identity)");

// 	}

// 	// Print out incorrect results
// 	if (!m.empty())  cout << " == " << header << " == " << endl;
// 	for (auto p : m) {
// 		cout << setw(w_) << (100 * p.second / float (total)) << "% error : " << p.first << endl;
// 	}
// }

// // testIntegerVector() {
	
// // }

// // testBooleanVector() {
	
// // }

// template<typename T>
// void floating_point_quaternion_test(const string &header="<type-missing>") {
// 	using U = typename T::value_t;
// 	// unordered_map<string, unsigned> m;
	
// 	// // Function that adds to map for ((a-b)^2 < epsilon)
// 	// auto test = [&](T a, T b, string s, bool f = true) {
// 	// 	if ((qsqerr(a, b) < U(EPSILON)) != f) m[s] = m[s]+1;
// 	// };

// 	// // Function that adds to map for given bool (if false)
// 	// auto test_bool  = [&](bool a, string s) {
// 	// 	if (!a) m[s] = m[s]+1;
// 	// };


// 	// for (int i = 0; i < total; ++i) {

// 	// 	// random quaternions
// 	// 	T a = T::random();
// 	// 	T b = T::random();
// 	// 	T c = T::random();

// 	// 	// random scalars
// 	// 	U sa = random<U>();
// 	// 	U sb = random<U>();
// 	// 	U sc = random<U>();

// 	// 	vector3<T> va = T::random(-1, 1);
// 	// 	vector3<T> vb = T::random(-1, 1);
		
// 	// 	// set vectors
// 	// 	T z = T();
// 	// 	T o = T(1);

// 	// 	// set scalars
// 	// 	U sz = U(0);
// 	// 	U so = U(1);



// 	// 	test(dot(na, na), so, "dot normalize (identity)");


// 	// 	// test(dot(na, na), so, "dot normalize (identity)");
// 	// 	// test(dot(na, -na), -so, "dot normalize (negitive identity)");

// 	// }

// 	// // Print out incorrect results
// 	// if (!m.empty())  cout << " == " << header << " == " << endl;
// 	// for (auto p : m) {
// 	// 	cout << setw(w_) << (100 * p.second / float (total)) << "% error : " << p.first << endl;
// 	// }
// }


// //template<typename T>
// //void testHash() {
// //	for (int i = 0; i < total; ++i) {
// //		T v = T::random();
// //		std::hash<T> hasher();
// //		if (!(v == v) || hasher(v) != hasher(v)) {
// //			
// //			break;
// //		}
// //	}
// //
// //}


// void vector_tests() {
// 	// correctness tests
// 	floating_point_vector_test<vector1<float>>("vector1<float>");
// 	floating_point_vector_test<vector2<float>>("vector2<float>");
// 	floating_point_vector_test<vector3<float>>("vector3<float>");
// 	floating_point_vector_test<vector4<float>>("vector4<float>");

// 	// meta function test
// 	static_assert(is_same<vec0f, meta::vec_by_size_t<float, 0>>::value, "vec_by_size");
// 	static_assert(is_same<vec1f, meta::vec_by_size_t<float, 1>>::value, "vec_by_size");
// 	static_assert(is_same<vec2f, meta::vec_by_size_t<float, 2>>::value, "vec_by_size");
// 	static_assert(is_same<vec3f, meta::vec_by_size_t<float, 3>>::value, "vec_by_size");
// 	static_assert(is_same<vec4f, meta::vec_by_size_t<float, 4>>::value, "vec_by_size");

// 	// hashing test
// 	hash<vector2<float>> vec2f_hash;
// 	hash<vector3<float>> vec3f_hash;
// 	hash<vector4<float>> vec4f_hash;
	
// }


// void matrix_tests() {
// 	// correctness tests
// 	// TODO

// 	// meta function test
// 	static_assert(is_same<mat2f, meta::mat_by_size_t<float, 2, 2>>::value, "mat_by_size");
// 	static_assert(is_same<mat3f, meta::mat_by_size_t<float, 3, 3>>::value, "mat_by_size");
// 	static_assert(is_same<mat4f, meta::mat_by_size_t<float, 4, 4>>::value, "mat_by_size");

// 	// hashing test
// 	hash<matrix2<float>> mat2f_hash;
// 	hash<matrix3<float>> mat3f_hash;
// 	hash<matrix4<float>> mat4f_hash;
// }


// void quaternion_tests() {
	

// 	// hashing test
// 	hash<quaternion<float>> quatf_hash;
// }


// //Main program
// // 
// void run_tests() {
// 	vector_tests();
// 	matrix_tests();
// 	quaternion_tests();
// }