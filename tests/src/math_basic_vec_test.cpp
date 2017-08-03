
#include <cgra_math.hpp>
#include "math_test.hpp"

using namespace std;
using namespace cgra;
using namespace test;

constexpr int max_iter = 1000;



template <typename T, size_t N>
float equality_identity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(vec_a == vec_a)) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float inequality_non_identity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(vec_a != vec_b)) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float negation_double_negation() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(-(-vec_a), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float addition_commutativity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a+vec_b, vec_b+vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float addition_associativity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_c = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a+(vec_b+vec_c), (vec_a+vec_b)+vec_c))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float addition_identity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a+vec_t(0), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float subtraction_anticommutativity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a-vec_b, -(vec_b-vec_a)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float subtraction_nonassociativity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(!test_equal(vec_a-(vec_b-vec_t(1)), (vec_a-vec_b)-vec_t(1)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float subtraction_identity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a-vec_t(0), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float subtraction_negation() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_t(0)-vec_a, -vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float multiplication_commutativity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a*vec_b, vec_b*vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float multiplication_associativity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_c = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a*(vec_b*vec_c), (vec_a*vec_b)*vec_c))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float multiplication_distributive() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_c = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a*(vec_b+vec_c), vec_a*vec_b+vec_a*vec_c))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float multiplication_identity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a*vec_t(1), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float multiplication_zero_property() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a*vec_t(0), vec_t(0)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float multiplication_negation() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(-vec_t(1)*-vec_t(1)*vec_a, vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float multiplication_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a*(vec_t(1)/vec_a), vec_t(1)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float division_right_distributive() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_c = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal((vec_a+vec_b)/vec_c, vec_a/vec_c + vec_b/vec_c))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float division_left_antidistributive() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_c = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(!test_equal(vec_a/(vec_b+vec_c), vec_a/vec_b + vec_a/vec_c))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float degrees_radians() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(degrees(radians(vec_a)), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float radians_degrees() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(radians(degrees(vec_a)), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_sin_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(asin(sin(pi * vec_a/2)), pi * vec_a/2))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_cos_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(acos(cos(pi * vec_pos)), pi * vec_pos))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_tan_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(atan(tan(vec_a)), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_hyperbolic_sinh_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(asinh(sinh(pi * vec_a/2)), pi * vec_a/2))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_hyperbolic_cosh_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(acosh(cosh(pi * vec_pos)), pi * vec_pos))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_hyperbolic_tanh_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(atanh(tanh(vec_a)), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_quotient() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(tan(vec_a), sin(vec_a)/cos(vec_a)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_inverse_quotient() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_t(1)/tan(vec_a), (vec_t(1)/sin(vec_a))/(vec_t(1)/cos(vec_a))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_sin_cos_pythagorean() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(sin(vec_a)*sin(vec_a) + cos(vec_a)*cos(vec_a), vec_t(1)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_tan_sec_pythagorean() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_t(1)+tan(vec_a)*tan(vec_a), (vec_t(1)/cos(vec_a))*(vec_t(1)/cos(vec_a))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float trigonometry_tan_sec_pythagorean_2() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_t(1)+(vec_t(1)/tan(vec_a))*(vec_t(1)/tan(vec_a)), (vec_t(1)/sin(vec_a))*(vec_t(1)/sin(vec_a))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float pow_double_equality() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a*vec_a, pow(vec_a, vec_t(2))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float pow_triple_equality() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(vec_a*vec_a*vec_a, pow(vec_a, vec_t(3))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float exp_log_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(log(exp(vec_a)), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float exp_log_double_inverse_reverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(exp(log(vec_pos)), vec_pos))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float exp2_log2_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(log2(exp2(vec_a)), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float exp2_log2_double_inverse_reverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(exp2(log2(vec_pos)), vec_pos))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float pow_sqrt_double_inverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(sqrt(pow(vec_pos, vec_t(2))), vec_pos))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float pow_sqrt_double_inverse_reverse() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(pow(sqrt(vec_pos), vec_t(2)), vec_pos))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float sqrt_inversesqrt_abs_equality() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		//if (!(test_equal(1/sqrt(vec_pos), inversesqrt(vec_pos)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float abs_negation() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(abs(vec_a), abs(-vec_a)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float sign_zero() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		
		if (!(test_equal(vec_t(sign(vec_t(0))), vec_t(0)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float sign_identity_absolute() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(sign(vec_a)*abs(vec_a), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float floor_all_less_than_equal() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(all(less_than_equal(floor(vec_a), vec_a)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float floor_fract_zero() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(fract(floor(vec_a)), vec_t(0)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float ceil_all_greater_than_equal() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(all(greater_than_equal(ceil(vec_a), vec_a)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float ceil_fract_zero() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(fract(ceil(vec_a)), vec_t(0)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float mod_scalar_fract() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(mod(vec_pos+vec_t(1), val_t(1)), fract(vec_pos)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float mod_vector_fract() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(mod(vec_pos+vec_t(1), vec_t(1)),  fract(vec_pos)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float min_scalar_greater_than_equal() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		val_t val_a = random<val_t>(val_t(-1), val_t(1));
		if (!(all(less_than_equal(min(vec_a,val_a), vec_a)) && all(less_than_equal(min(vec_a,val_a), vec_t(val_a))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float min_vector_greater_than_equal() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(all(less_than_equal(min(vec_a,vec_b), vec_a)) && all(less_than_equal(min(vec_a,vec_b), vec_b)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float max_scalar_greater_than_equal() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		val_t val_a = random<val_t>(val_t(-1), val_t(1));
		if (!(all(greater_than_equal(max(vec_a,val_a), vec_a)) && all(greater_than_equal(max(vec_a,val_a), vec_t(val_a))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float max_vector_greater_than_equal() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(all(greater_than_equal(max(vec_a,vec_b), vec_a)) && all(greater_than_equal(max(vec_a,vec_b), vec_b)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float min_max_scalar_summation_equality() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		val_t val_a = random<val_t>(val_t(-1), val_t(1));
		if (!(test_equal(min(vec_a,val_a)+max(vec_a,val_a),  vec_a+val_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float min_max_vector_summation_equality() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(min(vec_a,vec_b)+max(vec_a,vec_b),  vec_a+vec_b))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float mix_scalar_identity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		val_t val_a = random<val_t>(val_t(-1), val_t(1));
		if (!(test_equal(mix(vec_a, vec_a, val_a), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float mix_scalar_summation_equality() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		val_t val_a = random<val_t>(val_t(-1), val_t(1));
		if (!(test_equal(mix(vec_a, vec_b, val_a) + mix(vec_b, vec_a, val_a), vec_a+vec_b))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float mix_vector_identity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(mix(vec_a, vec_a, vec_pos), vec_a))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float mix_vector_summation_equality() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_pos = random<vec_t>(vec_t(0), vec_t(1));
		if (!(test_equal(mix(vec_a, vec_b, vec_pos) + mix(vec_b, vec_a, vec_pos), vec_a+vec_b))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float cross_anticommutativity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(test_equal(cross(vec_a, vec_b), -cross(vec_b, vec_a)))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float all_less_than_greater_than_equal_not_equal_exclusivity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(all(not_equal(less_than(vec_a, vec_b), greater_than_equal(vec_a, vec_b))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}


template <typename T, size_t N>
float all_less_than_equal_greater_than_not_equal_exclusivity() {
	using namespace cgra;
	using vec_t = basic_vec<T, N>;
	using val_t = typename vec_t::value_t;
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_b = random<vec_t>(vec_t(-1), vec_t(1));
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(all(not_equal(less_than_equal(vec_a, vec_b), greater_than(vec_a, vec_b))))) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}



void test::run_basic_vec_tests() {
	ouput_test("equality_identity<float, 1>", equality_identity<float, 1>());
	ouput_test("equality_identity<float, 2>", equality_identity<float, 2>());
	ouput_test("equality_identity<float, 3>", equality_identity<float, 3>());
	ouput_test("equality_identity<float, 4>", equality_identity<float, 4>());
	ouput_test("inequality_non_identity<float, 1>", inequality_non_identity<float, 1>());
	ouput_test("inequality_non_identity<float, 2>", inequality_non_identity<float, 2>());
	ouput_test("inequality_non_identity<float, 3>", inequality_non_identity<float, 3>());
	ouput_test("inequality_non_identity<float, 4>", inequality_non_identity<float, 4>());
	ouput_test("negation_double_negation<float, 1>", negation_double_negation<float, 1>());
	ouput_test("negation_double_negation<float, 2>", negation_double_negation<float, 2>());
	ouput_test("negation_double_negation<float, 3>", negation_double_negation<float, 3>());
	ouput_test("negation_double_negation<float, 4>", negation_double_negation<float, 4>());
	ouput_test("addition_commutativity<float, 1>", addition_commutativity<float, 1>());
	ouput_test("addition_commutativity<float, 2>", addition_commutativity<float, 2>());
	ouput_test("addition_commutativity<float, 3>", addition_commutativity<float, 3>());
	ouput_test("addition_commutativity<float, 4>", addition_commutativity<float, 4>());
	ouput_test("addition_associativity<float, 1>", addition_associativity<float, 1>());
	ouput_test("addition_associativity<float, 2>", addition_associativity<float, 2>());
	ouput_test("addition_associativity<float, 3>", addition_associativity<float, 3>());
	ouput_test("addition_associativity<float, 4>", addition_associativity<float, 4>());
	ouput_test("addition_identity<float, 1>", addition_identity<float, 1>());
	ouput_test("addition_identity<float, 2>", addition_identity<float, 2>());
	ouput_test("addition_identity<float, 3>", addition_identity<float, 3>());
	ouput_test("addition_identity<float, 4>", addition_identity<float, 4>());
	ouput_test("subtraction_anticommutativity<float, 1>", subtraction_anticommutativity<float, 1>());
	ouput_test("subtraction_anticommutativity<float, 2>", subtraction_anticommutativity<float, 2>());
	ouput_test("subtraction_anticommutativity<float, 3>", subtraction_anticommutativity<float, 3>());
	ouput_test("subtraction_anticommutativity<float, 4>", subtraction_anticommutativity<float, 4>());
	ouput_test("subtraction_nonassociativity<float, 1>", subtraction_nonassociativity<float, 1>());
	ouput_test("subtraction_nonassociativity<float, 2>", subtraction_nonassociativity<float, 2>());
	ouput_test("subtraction_nonassociativity<float, 3>", subtraction_nonassociativity<float, 3>());
	ouput_test("subtraction_nonassociativity<float, 4>", subtraction_nonassociativity<float, 4>());
	ouput_test("subtraction_identity<float, 1>", subtraction_identity<float, 1>());
	ouput_test("subtraction_identity<float, 2>", subtraction_identity<float, 2>());
	ouput_test("subtraction_identity<float, 3>", subtraction_identity<float, 3>());
	ouput_test("subtraction_identity<float, 4>", subtraction_identity<float, 4>());
	ouput_test("subtraction_negation<float, 1>", subtraction_negation<float, 1>());
	ouput_test("subtraction_negation<float, 2>", subtraction_negation<float, 2>());
	ouput_test("subtraction_negation<float, 3>", subtraction_negation<float, 3>());
	ouput_test("subtraction_negation<float, 4>", subtraction_negation<float, 4>());
	ouput_test("multiplication_commutativity<float, 1>", multiplication_commutativity<float, 1>());
	ouput_test("multiplication_commutativity<float, 2>", multiplication_commutativity<float, 2>());
	ouput_test("multiplication_commutativity<float, 3>", multiplication_commutativity<float, 3>());
	ouput_test("multiplication_commutativity<float, 4>", multiplication_commutativity<float, 4>());
	ouput_test("multiplication_associativity<float, 1>", multiplication_associativity<float, 1>());
	ouput_test("multiplication_associativity<float, 2>", multiplication_associativity<float, 2>());
	ouput_test("multiplication_associativity<float, 3>", multiplication_associativity<float, 3>());
	ouput_test("multiplication_associativity<float, 4>", multiplication_associativity<float, 4>());
	ouput_test("multiplication_distributive<float, 1>", multiplication_distributive<float, 1>());
	ouput_test("multiplication_distributive<float, 2>", multiplication_distributive<float, 2>());
	ouput_test("multiplication_distributive<float, 3>", multiplication_distributive<float, 3>());
	ouput_test("multiplication_distributive<float, 4>", multiplication_distributive<float, 4>());
	ouput_test("multiplication_identity<float, 1>", multiplication_identity<float, 1>());
	ouput_test("multiplication_identity<float, 2>", multiplication_identity<float, 2>());
	ouput_test("multiplication_identity<float, 3>", multiplication_identity<float, 3>());
	ouput_test("multiplication_identity<float, 4>", multiplication_identity<float, 4>());
	ouput_test("multiplication_zero_property<float, 1>", multiplication_zero_property<float, 1>());
	ouput_test("multiplication_zero_property<float, 2>", multiplication_zero_property<float, 2>());
	ouput_test("multiplication_zero_property<float, 3>", multiplication_zero_property<float, 3>());
	ouput_test("multiplication_zero_property<float, 4>", multiplication_zero_property<float, 4>());
	ouput_test("multiplication_negation<float, 1>", multiplication_negation<float, 1>());
	ouput_test("multiplication_negation<float, 2>", multiplication_negation<float, 2>());
	ouput_test("multiplication_negation<float, 3>", multiplication_negation<float, 3>());
	ouput_test("multiplication_negation<float, 4>", multiplication_negation<float, 4>());
	ouput_test("multiplication_inverse<float, 1>", multiplication_inverse<float, 1>());
	ouput_test("multiplication_inverse<float, 2>", multiplication_inverse<float, 2>());
	ouput_test("multiplication_inverse<float, 3>", multiplication_inverse<float, 3>());
	ouput_test("multiplication_inverse<float, 4>", multiplication_inverse<float, 4>());
	ouput_test("division_right_distributive<float, 1>", division_right_distributive<float, 1>());
	ouput_test("division_right_distributive<float, 2>", division_right_distributive<float, 2>());
	ouput_test("division_right_distributive<float, 3>", division_right_distributive<float, 3>());
	ouput_test("division_right_distributive<float, 4>", division_right_distributive<float, 4>());
	ouput_test("division_left_antidistributive<float, 1>", division_left_antidistributive<float, 1>());
	ouput_test("division_left_antidistributive<float, 2>", division_left_antidistributive<float, 2>());
	ouput_test("division_left_antidistributive<float, 3>", division_left_antidistributive<float, 3>());
	ouput_test("division_left_antidistributive<float, 4>", division_left_antidistributive<float, 4>());
	ouput_test("degrees_radians<float, 1>", degrees_radians<float, 1>());
	ouput_test("degrees_radians<float, 2>", degrees_radians<float, 2>());
	ouput_test("degrees_radians<float, 3>", degrees_radians<float, 3>());
	ouput_test("degrees_radians<float, 4>", degrees_radians<float, 4>());
	ouput_test("radians_degrees<float, 1>", radians_degrees<float, 1>());
	ouput_test("radians_degrees<float, 2>", radians_degrees<float, 2>());
	ouput_test("radians_degrees<float, 3>", radians_degrees<float, 3>());
	ouput_test("radians_degrees<float, 4>", radians_degrees<float, 4>());
	ouput_test("trigonometry_sin_double_inverse<float, 1>", trigonometry_sin_double_inverse<float, 1>());
	ouput_test("trigonometry_sin_double_inverse<float, 2>", trigonometry_sin_double_inverse<float, 2>());
	ouput_test("trigonometry_sin_double_inverse<float, 3>", trigonometry_sin_double_inverse<float, 3>());
	ouput_test("trigonometry_sin_double_inverse<float, 4>", trigonometry_sin_double_inverse<float, 4>());
	ouput_test("trigonometry_cos_double_inverse<float, 1>", trigonometry_cos_double_inverse<float, 1>());
	ouput_test("trigonometry_cos_double_inverse<float, 2>", trigonometry_cos_double_inverse<float, 2>());
	ouput_test("trigonometry_cos_double_inverse<float, 3>", trigonometry_cos_double_inverse<float, 3>());
	ouput_test("trigonometry_cos_double_inverse<float, 4>", trigonometry_cos_double_inverse<float, 4>());
	ouput_test("trigonometry_tan_double_inverse<float, 1>", trigonometry_tan_double_inverse<float, 1>());
	ouput_test("trigonometry_tan_double_inverse<float, 2>", trigonometry_tan_double_inverse<float, 2>());
	ouput_test("trigonometry_tan_double_inverse<float, 3>", trigonometry_tan_double_inverse<float, 3>());
	ouput_test("trigonometry_tan_double_inverse<float, 4>", trigonometry_tan_double_inverse<float, 4>());
	ouput_test("trigonometry_hyperbolic_sinh_double_inverse<float, 1>", trigonometry_hyperbolic_sinh_double_inverse<float, 1>());
	ouput_test("trigonometry_hyperbolic_sinh_double_inverse<float, 2>", trigonometry_hyperbolic_sinh_double_inverse<float, 2>());
	ouput_test("trigonometry_hyperbolic_sinh_double_inverse<float, 3>", trigonometry_hyperbolic_sinh_double_inverse<float, 3>());
	ouput_test("trigonometry_hyperbolic_sinh_double_inverse<float, 4>", trigonometry_hyperbolic_sinh_double_inverse<float, 4>());
	ouput_test("trigonometry_hyperbolic_cosh_double_inverse<float, 1>", trigonometry_hyperbolic_cosh_double_inverse<float, 1>());
	ouput_test("trigonometry_hyperbolic_cosh_double_inverse<float, 2>", trigonometry_hyperbolic_cosh_double_inverse<float, 2>());
	ouput_test("trigonometry_hyperbolic_cosh_double_inverse<float, 3>", trigonometry_hyperbolic_cosh_double_inverse<float, 3>());
	ouput_test("trigonometry_hyperbolic_cosh_double_inverse<float, 4>", trigonometry_hyperbolic_cosh_double_inverse<float, 4>());
	ouput_test("trigonometry_hyperbolic_tanh_double_inverse<float, 1>", trigonometry_hyperbolic_tanh_double_inverse<float, 1>());
	ouput_test("trigonometry_hyperbolic_tanh_double_inverse<float, 2>", trigonometry_hyperbolic_tanh_double_inverse<float, 2>());
	ouput_test("trigonometry_hyperbolic_tanh_double_inverse<float, 3>", trigonometry_hyperbolic_tanh_double_inverse<float, 3>());
	ouput_test("trigonometry_hyperbolic_tanh_double_inverse<float, 4>", trigonometry_hyperbolic_tanh_double_inverse<float, 4>());
	ouput_test("trigonometry_quotient<float, 1>", trigonometry_quotient<float, 1>());
	ouput_test("trigonometry_quotient<float, 2>", trigonometry_quotient<float, 2>());
	ouput_test("trigonometry_quotient<float, 3>", trigonometry_quotient<float, 3>());
	ouput_test("trigonometry_quotient<float, 4>", trigonometry_quotient<float, 4>());
	ouput_test("trigonometry_inverse_quotient<float, 1>", trigonometry_inverse_quotient<float, 1>());
	ouput_test("trigonometry_inverse_quotient<float, 2>", trigonometry_inverse_quotient<float, 2>());
	ouput_test("trigonometry_inverse_quotient<float, 3>", trigonometry_inverse_quotient<float, 3>());
	ouput_test("trigonometry_inverse_quotient<float, 4>", trigonometry_inverse_quotient<float, 4>());
	ouput_test("trigonometry_sin_cos_pythagorean<float, 1>", trigonometry_sin_cos_pythagorean<float, 1>());
	ouput_test("trigonometry_sin_cos_pythagorean<float, 2>", trigonometry_sin_cos_pythagorean<float, 2>());
	ouput_test("trigonometry_sin_cos_pythagorean<float, 3>", trigonometry_sin_cos_pythagorean<float, 3>());
	ouput_test("trigonometry_sin_cos_pythagorean<float, 4>", trigonometry_sin_cos_pythagorean<float, 4>());
	ouput_test("trigonometry_tan_sec_pythagorean<float, 1>", trigonometry_tan_sec_pythagorean<float, 1>());
	ouput_test("trigonometry_tan_sec_pythagorean<float, 2>", trigonometry_tan_sec_pythagorean<float, 2>());
	ouput_test("trigonometry_tan_sec_pythagorean<float, 3>", trigonometry_tan_sec_pythagorean<float, 3>());
	ouput_test("trigonometry_tan_sec_pythagorean<float, 4>", trigonometry_tan_sec_pythagorean<float, 4>());
	ouput_test("trigonometry_tan_sec_pythagorean_2<float, 1>", trigonometry_tan_sec_pythagorean_2<float, 1>());
	ouput_test("trigonometry_tan_sec_pythagorean_2<float, 2>", trigonometry_tan_sec_pythagorean_2<float, 2>());
	ouput_test("trigonometry_tan_sec_pythagorean_2<float, 3>", trigonometry_tan_sec_pythagorean_2<float, 3>());
	ouput_test("trigonometry_tan_sec_pythagorean_2<float, 4>", trigonometry_tan_sec_pythagorean_2<float, 4>());
	ouput_test("pow_double_equality<float, 1>", pow_double_equality<float, 1>());
	ouput_test("pow_double_equality<float, 2>", pow_double_equality<float, 2>());
	ouput_test("pow_double_equality<float, 3>", pow_double_equality<float, 3>());
	ouput_test("pow_double_equality<float, 4>", pow_double_equality<float, 4>());
	ouput_test("pow_triple_equality<float, 1>", pow_triple_equality<float, 1>());
	ouput_test("pow_triple_equality<float, 2>", pow_triple_equality<float, 2>());
	ouput_test("pow_triple_equality<float, 3>", pow_triple_equality<float, 3>());
	ouput_test("pow_triple_equality<float, 4>", pow_triple_equality<float, 4>());
	ouput_test("exp_log_double_inverse<float, 1>", exp_log_double_inverse<float, 1>());
	ouput_test("exp_log_double_inverse<float, 2>", exp_log_double_inverse<float, 2>());
	ouput_test("exp_log_double_inverse<float, 3>", exp_log_double_inverse<float, 3>());
	ouput_test("exp_log_double_inverse<float, 4>", exp_log_double_inverse<float, 4>());
	ouput_test("exp_log_double_inverse_reverse<float, 1>", exp_log_double_inverse_reverse<float, 1>());
	ouput_test("exp_log_double_inverse_reverse<float, 2>", exp_log_double_inverse_reverse<float, 2>());
	ouput_test("exp_log_double_inverse_reverse<float, 3>", exp_log_double_inverse_reverse<float, 3>());
	ouput_test("exp_log_double_inverse_reverse<float, 4>", exp_log_double_inverse_reverse<float, 4>());
	ouput_test("exp2_log2_double_inverse<float, 1>", exp2_log2_double_inverse<float, 1>());
	ouput_test("exp2_log2_double_inverse<float, 2>", exp2_log2_double_inverse<float, 2>());
	ouput_test("exp2_log2_double_inverse<float, 3>", exp2_log2_double_inverse<float, 3>());
	ouput_test("exp2_log2_double_inverse<float, 4>", exp2_log2_double_inverse<float, 4>());
	ouput_test("exp2_log2_double_inverse_reverse<float, 1>", exp2_log2_double_inverse_reverse<float, 1>());
	ouput_test("exp2_log2_double_inverse_reverse<float, 2>", exp2_log2_double_inverse_reverse<float, 2>());
	ouput_test("exp2_log2_double_inverse_reverse<float, 3>", exp2_log2_double_inverse_reverse<float, 3>());
	ouput_test("exp2_log2_double_inverse_reverse<float, 4>", exp2_log2_double_inverse_reverse<float, 4>());
	ouput_test("pow_sqrt_double_inverse<float, 1>", pow_sqrt_double_inverse<float, 1>());
	ouput_test("pow_sqrt_double_inverse<float, 2>", pow_sqrt_double_inverse<float, 2>());
	ouput_test("pow_sqrt_double_inverse<float, 3>", pow_sqrt_double_inverse<float, 3>());
	ouput_test("pow_sqrt_double_inverse<float, 4>", pow_sqrt_double_inverse<float, 4>());
	ouput_test("pow_sqrt_double_inverse_reverse<float, 1>", pow_sqrt_double_inverse_reverse<float, 1>());
	ouput_test("pow_sqrt_double_inverse_reverse<float, 2>", pow_sqrt_double_inverse_reverse<float, 2>());
	ouput_test("pow_sqrt_double_inverse_reverse<float, 3>", pow_sqrt_double_inverse_reverse<float, 3>());
	ouput_test("pow_sqrt_double_inverse_reverse<float, 4>", pow_sqrt_double_inverse_reverse<float, 4>());
	ouput_test("sqrt_inversesqrt_abs_equality<float, 1>", sqrt_inversesqrt_abs_equality<float, 1>());
	ouput_test("sqrt_inversesqrt_abs_equality<float, 2>", sqrt_inversesqrt_abs_equality<float, 2>());
	ouput_test("sqrt_inversesqrt_abs_equality<float, 3>", sqrt_inversesqrt_abs_equality<float, 3>());
	ouput_test("sqrt_inversesqrt_abs_equality<float, 4>", sqrt_inversesqrt_abs_equality<float, 4>());
	ouput_test("abs_negation<float, 1>", abs_negation<float, 1>());
	ouput_test("abs_negation<float, 2>", abs_negation<float, 2>());
	ouput_test("abs_negation<float, 3>", abs_negation<float, 3>());
	ouput_test("abs_negation<float, 4>", abs_negation<float, 4>());
	ouput_test("sign_zero<float, 1>", sign_zero<float, 1>());
	ouput_test("sign_zero<float, 2>", sign_zero<float, 2>());
	ouput_test("sign_zero<float, 3>", sign_zero<float, 3>());
	ouput_test("sign_zero<float, 4>", sign_zero<float, 4>());
	ouput_test("sign_identity_absolute<float, 1>", sign_identity_absolute<float, 1>());
	ouput_test("sign_identity_absolute<float, 2>", sign_identity_absolute<float, 2>());
	ouput_test("sign_identity_absolute<float, 3>", sign_identity_absolute<float, 3>());
	ouput_test("sign_identity_absolute<float, 4>", sign_identity_absolute<float, 4>());
	ouput_test("floor_all_less_than_equal<float, 1>", floor_all_less_than_equal<float, 1>());
	ouput_test("floor_all_less_than_equal<float, 2>", floor_all_less_than_equal<float, 2>());
	ouput_test("floor_all_less_than_equal<float, 3>", floor_all_less_than_equal<float, 3>());
	ouput_test("floor_all_less_than_equal<float, 4>", floor_all_less_than_equal<float, 4>());
	ouput_test("floor_fract_zero<float, 1>", floor_fract_zero<float, 1>());
	ouput_test("floor_fract_zero<float, 2>", floor_fract_zero<float, 2>());
	ouput_test("floor_fract_zero<float, 3>", floor_fract_zero<float, 3>());
	ouput_test("floor_fract_zero<float, 4>", floor_fract_zero<float, 4>());
	ouput_test("ceil_all_greater_than_equal<float, 1>", ceil_all_greater_than_equal<float, 1>());
	ouput_test("ceil_all_greater_than_equal<float, 2>", ceil_all_greater_than_equal<float, 2>());
	ouput_test("ceil_all_greater_than_equal<float, 3>", ceil_all_greater_than_equal<float, 3>());
	ouput_test("ceil_all_greater_than_equal<float, 4>", ceil_all_greater_than_equal<float, 4>());
	ouput_test("ceil_fract_zero<float, 1>", ceil_fract_zero<float, 1>());
	ouput_test("ceil_fract_zero<float, 2>", ceil_fract_zero<float, 2>());
	ouput_test("ceil_fract_zero<float, 3>", ceil_fract_zero<float, 3>());
	ouput_test("ceil_fract_zero<float, 4>", ceil_fract_zero<float, 4>());
	ouput_test("mod_scalar_fract<float, 1>", mod_scalar_fract<float, 1>());
	ouput_test("mod_scalar_fract<float, 2>", mod_scalar_fract<float, 2>());
	ouput_test("mod_scalar_fract<float, 3>", mod_scalar_fract<float, 3>());
	ouput_test("mod_scalar_fract<float, 4>", mod_scalar_fract<float, 4>());
	ouput_test("mod_vector_fract<float, 1>", mod_vector_fract<float, 1>());
	ouput_test("mod_vector_fract<float, 2>", mod_vector_fract<float, 2>());
	ouput_test("mod_vector_fract<float, 3>", mod_vector_fract<float, 3>());
	ouput_test("mod_vector_fract<float, 4>", mod_vector_fract<float, 4>());
	ouput_test("min_scalar_greater_than_equal<float, 1>", min_scalar_greater_than_equal<float, 1>());
	ouput_test("min_scalar_greater_than_equal<float, 2>", min_scalar_greater_than_equal<float, 2>());
	ouput_test("min_scalar_greater_than_equal<float, 3>", min_scalar_greater_than_equal<float, 3>());
	ouput_test("min_scalar_greater_than_equal<float, 4>", min_scalar_greater_than_equal<float, 4>());
	ouput_test("min_vector_greater_than_equal<float, 1>", min_vector_greater_than_equal<float, 1>());
	ouput_test("min_vector_greater_than_equal<float, 2>", min_vector_greater_than_equal<float, 2>());
	ouput_test("min_vector_greater_than_equal<float, 3>", min_vector_greater_than_equal<float, 3>());
	ouput_test("min_vector_greater_than_equal<float, 4>", min_vector_greater_than_equal<float, 4>());
	ouput_test("max_scalar_greater_than_equal<float, 1>", max_scalar_greater_than_equal<float, 1>());
	ouput_test("max_scalar_greater_than_equal<float, 2>", max_scalar_greater_than_equal<float, 2>());
	ouput_test("max_scalar_greater_than_equal<float, 3>", max_scalar_greater_than_equal<float, 3>());
	ouput_test("max_scalar_greater_than_equal<float, 4>", max_scalar_greater_than_equal<float, 4>());
	ouput_test("max_vector_greater_than_equal<float, 1>", max_vector_greater_than_equal<float, 1>());
	ouput_test("max_vector_greater_than_equal<float, 2>", max_vector_greater_than_equal<float, 2>());
	ouput_test("max_vector_greater_than_equal<float, 3>", max_vector_greater_than_equal<float, 3>());
	ouput_test("max_vector_greater_than_equal<float, 4>", max_vector_greater_than_equal<float, 4>());
	ouput_test("min_max_scalar_summation_equality<float, 1>", min_max_scalar_summation_equality<float, 1>());
	ouput_test("min_max_scalar_summation_equality<float, 2>", min_max_scalar_summation_equality<float, 2>());
	ouput_test("min_max_scalar_summation_equality<float, 3>", min_max_scalar_summation_equality<float, 3>());
	ouput_test("min_max_scalar_summation_equality<float, 4>", min_max_scalar_summation_equality<float, 4>());
	ouput_test("min_max_vector_summation_equality<float, 1>", min_max_vector_summation_equality<float, 1>());
	ouput_test("min_max_vector_summation_equality<float, 2>", min_max_vector_summation_equality<float, 2>());
	ouput_test("min_max_vector_summation_equality<float, 3>", min_max_vector_summation_equality<float, 3>());
	ouput_test("min_max_vector_summation_equality<float, 4>", min_max_vector_summation_equality<float, 4>());
	ouput_test("mix_scalar_identity<float, 1>", mix_scalar_identity<float, 1>());
	ouput_test("mix_scalar_identity<float, 2>", mix_scalar_identity<float, 2>());
	ouput_test("mix_scalar_identity<float, 3>", mix_scalar_identity<float, 3>());
	ouput_test("mix_scalar_identity<float, 4>", mix_scalar_identity<float, 4>());
	ouput_test("mix_scalar_summation_equality<float, 1>", mix_scalar_summation_equality<float, 1>());
	ouput_test("mix_scalar_summation_equality<float, 2>", mix_scalar_summation_equality<float, 2>());
	ouput_test("mix_scalar_summation_equality<float, 3>", mix_scalar_summation_equality<float, 3>());
	ouput_test("mix_scalar_summation_equality<float, 4>", mix_scalar_summation_equality<float, 4>());
	ouput_test("mix_vector_identity<float, 1>", mix_vector_identity<float, 1>());
	ouput_test("mix_vector_identity<float, 2>", mix_vector_identity<float, 2>());
	ouput_test("mix_vector_identity<float, 3>", mix_vector_identity<float, 3>());
	ouput_test("mix_vector_identity<float, 4>", mix_vector_identity<float, 4>());
	ouput_test("mix_vector_summation_equality<float, 1>", mix_vector_summation_equality<float, 1>());
	ouput_test("mix_vector_summation_equality<float, 2>", mix_vector_summation_equality<float, 2>());
	ouput_test("mix_vector_summation_equality<float, 3>", mix_vector_summation_equality<float, 3>());
	ouput_test("mix_vector_summation_equality<float, 4>", mix_vector_summation_equality<float, 4>());
	ouput_test("cross_anticommutativity<float, 3>", cross_anticommutativity<float, 3>());
	ouput_test("all_less_than_greater_than_equal_not_equal_exclusivity<float, 1>", all_less_than_greater_than_equal_not_equal_exclusivity<float, 1>());
	ouput_test("all_less_than_greater_than_equal_not_equal_exclusivity<float, 2>", all_less_than_greater_than_equal_not_equal_exclusivity<float, 2>());
	ouput_test("all_less_than_greater_than_equal_not_equal_exclusivity<float, 3>", all_less_than_greater_than_equal_not_equal_exclusivity<float, 3>());
	ouput_test("all_less_than_greater_than_equal_not_equal_exclusivity<float, 4>", all_less_than_greater_than_equal_not_equal_exclusivity<float, 4>());
	ouput_test("all_less_than_equal_greater_than_not_equal_exclusivity<float, 1>", all_less_than_equal_greater_than_not_equal_exclusivity<float, 1>());
	ouput_test("all_less_than_equal_greater_than_not_equal_exclusivity<float, 2>", all_less_than_equal_greater_than_not_equal_exclusivity<float, 2>());
	ouput_test("all_less_than_equal_greater_than_not_equal_exclusivity<float, 3>", all_less_than_equal_greater_than_not_equal_exclusivity<float, 3>());
	ouput_test("all_less_than_equal_greater_than_not_equal_exclusivity<float, 4>", all_less_than_equal_greater_than_not_equal_exclusivity<float, 4>());
}



