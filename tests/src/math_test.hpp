#pragma once

#include <cgra_math.hpp>

namespace test {
	void run_basic_vec_tests();
	// void run_mat_tests();
	// void run_quat_tests();


	inline void ouput_test(const std::string &name, float fail_fract) {
		/*if (fail_fract > 0.1) {
			std::cout << name << " has failed" << std::endl;
		}*/

		std::cout << ((fail_fract > 0.1) ? "--FAILED-- " : " --pass--  " ) << std::setw(4) << std::setprecision(4) << (fail_fract*100) << "% " << name << std::endl;
	}



	// Integer values
	template<typename T>
	typename std::enable_if_t<std::numeric_limits<T>::is_integer, bool> test_equal(T a, T b, int ulp = 0) {
		return a == b;
	}

	// Non-integer values
	template<typename T>
	typename std::enable_if_t<!std::numeric_limits<T>::is_integer, bool> test_equal(T a, T b, int ulp = 5) {
		if (a == b) return true;
		// the machine epsilon has to be scaled to the magnitude of the values used
		// and multiplied by the desired precision in ULPs (units in the last place)
		bool r = std::abs(a-b) < std::numeric_limits<T>::epsilon() * std::abs(a+b) * ulp
		// unless the result is subnormal
				|| std::abs(a-b) < std::numeric_limits<T>::min();

		return r;
	}

	// Vector values
	template <typename T, size_t N>
	bool test_equal(const cgra::basic_vec<T, N> &a, const cgra::basic_vec<T, N> &b, int ulp = 5) {
		using namespace cgra;
		bool r = all(zip_with([=](auto &&x, auto &&y) { return test_equal(decltype(x)(x), decltype(y)(y), ulp); }, a, b));
		return r;
	}
	
	// Matrix values
	template <typename T, size_t Cols, size_t Rows>
	bool test_equal(const cgra::basic_mat<T, Cols, Rows> &a, const cgra::basic_mat<T, Cols, Rows> &b, int ulp = 5) {
		using namespace cgra;
		bool r = all(zip_with([=](auto &&x, auto &&y) { return test_equal(decltype(x)(x), decltype(y)(y), ulp); }, a.as_vec(), b.as_vec()));
		return r;
	}

	// Quaternion values
	template <typename T>
	bool test_equal(const cgra::basic_quat<T> &a, const cgra::basic_quat<T> &b, int ulp = 5) {
		using namespace cgra;
		bool r = all(zip_with([=](auto &&x, auto &&y) { return test_equal(decltype(x)(x), decltype(y)(y), ulp); }, a, b));
		return r;
	}
}