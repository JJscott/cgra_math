


bool pow_double_equality() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(a*a, pow(a, T(2)));
}


bool pow_triple_equality() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(a*a*a, pow(a, T(3)));
}


bool exp_log_double_inverse() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(log(exp(a)), a);
}


bool exp_log_double_inverse_reverse() {
	T a = random<T>(T(0), T(1));
	return almost_equal(exp(log(a)), a);
}


bool exp2_log2_double_inverse() {
	T a = random<T>(T(-1), T(1));
	return almost_equal(log2(exp2(a)), a);
}


bool exp2_log2_double_inverse_reverse() {
	T a = random<T>(T(0), T(1));
	return almost_equal(exp2(log2(a)), a);
}


bool pow_sqrt_double_inverse() {
	T a = random<T>(T(0), T(1));
	return almost_equal(sqrt(pow(a, T(2))), a);
}


bool pow_sqrt_double_inverse_reverse() {
	T a = random<T>(T(0), T(1));
	return almost_equal(pow(sqrt(a), T(2)), a);
}


bool sqrt_inversesqrt_abs_equality() {
	T a = random<T>(T(0), T(1));
	return almost_equal(1/sqrt(a), inversesqrt(a));
}



template <typename T>
void test_exponential_functions() {
	execute_test( () [] { return pow_double_equality<T>(); }, "pow_double_equality" );
	execute_test( () [] { return pow_triple_equality<T>(); }, "pow_triple_equality" );
	execute_test( () [] { return exp_log_double_inverse<T>(); }, "exp_log_double_inverse" );
	execute_test( () [] { return exp_log_double_inverse_reverse<T>(); }, "	exp_log_double_inverse_reverse" );
	execute_test( () [] { return exp2_log2_double_inverse<T>(); }, "exp2_log2_double_inverse" );
	execute_test( () [] { return exp2_log2_double_inverse_reverse<T>(); }, "exp2_log2_double_inverse_reverse" );
	execute_test( () [] { return pow_sqrt_double_inverse<T>(); }, "pow_sqrt_double_inverse" );
	execute_test( () [] { return pow_sqrt_double_inverse_reverse<T>(); }, "pow_sqrt_double_inverse_reverse" );
	execute_test( () [] { return sqrt_inversesqrt_abs_equality<T>(); }, "sqrt_inversesqrt_abs_equality" );
}