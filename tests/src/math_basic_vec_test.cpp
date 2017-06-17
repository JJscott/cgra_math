
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
	using val_t = T; // same as T
	int fail_count = 0;
	for (int i = 0; i < max_iter; ++i) {
		vec_t vec_a = random<vec_t>(vec_t(-1), vec_t(1));
		if (!(vec_a == vec_a)) fail_count++;
	}
	float fail_fract = float(fail_count) / max_iter;
	return fail_fract;
}

void test::run_basic_vec_tests() {
	ouput_test("equality_identity", equality_identity<float, 1>());
	ouput_test("equality_identity", equality_identity<float, 2>());
	ouput_test("equality_identity", equality_identity<float, 3>());
	ouput_test("equality_identity", equality_identity<float, 4>());
}



