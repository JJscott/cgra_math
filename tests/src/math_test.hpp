





namespace test {
	void run_tests();


	template<typename T>
	typename std::enable_if_t<!std::numeric_limits<T>::is_integer, bool> almost_equal(T a, T b, int ulp = 4) {
		if (a == b) return true;
		// the machine epsilon has to be scaled to the magnitude of the values used
		// and multiplied by the desired precision in ULPs (units in the last place)
		bool r = std::abs(a-b) < std::numeric_limits<T>::epsilon() * std::abs(a+b) * ulp
		// unless the result is subnormal
				|| std::abs(a-b) < std::numeric_limits<T>::min();

		return r;
	}

	template <typename T, size_t N>
	bool almost_equal(const cgra::basic_vec<T, N> &a, const cgra::basic_vec<T, N> &b, int ulp = 4) {
		bool r = all(zip_with([=](auto &&x, auto &&y) { return almost_equal(decltype(x)(x), decltype(y)(y), ulp); }, a, b));
		//if (!r) {
		//	cout << "Failed " << a << " != " << b << endl;
		//}
		return r;
	}



}