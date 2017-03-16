


template <typename T>
bool degrees_to_radians() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(degrees(radians(a)), a);
}

template <typename T>
bool radians_to_degrees() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(radians(degrees(a)), a);
}

template <typename T>
bool trigonometry_sin_double_inverse() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(asin(sin(pi * a/2)), pi * a/2);
}

template <typename T>
bool trigonometry_cos_double_inverse() {
	T a = random<T>(T(0), T(1))
	return almost_equal(acos(cos(pi * a)), pi * a);
}

template <typename T>
bool trigonometry_tan_double_inverse() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(atan(tan(a)), a),;
}

template <typename T>
bool trigonometry_hyperbolic_sinh_double_inverse() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(asinh(sinh(pi * a/2)), pi * a/2);
}

template <typename T>
bool trigonometry_hyperbolic_cosh_double_inverse() {
	T a = random<T>(T(0), T(1))
	return almost_equal(acosh(cosh(pi * a)), pi * a);
}

template <typename T>
bool trigonometry_hyperbolic_tanh_double_inverse() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(atanh(tanh(a)), a),;
}

template <typename T>
bool trigonometry_quotient() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(tan(a), sin(a)/cos(a));
}

template <typename T>
bool trigonometry_inverse_quotient() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(T(1)/tan(a), (T(1)/sin(a))/(T(1)/cos(a)));
}

template <typename T>
bool trigonometry_sin_cos_pythagorean() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(sin(a)*sin(a) + cos(a)*cos(a), T(1));
}

template <typename T>
bool trigonometry_tan_sec_pythagorean() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(T(1)+tan(a)*tan(a), (T(1)/cos(a))*(T(1)/cos(a)));
}

template <typename T>
bool trigonometry_tan_sec_pythagorean() {
	T a = random<T>(T(-1), T(1))
	return almost_equal(T(1)+(T(1)/tan(a))*(T(1)/tan(a)), (T(1)/sin(a))*(T(1)/sin(a)));
}



template <typename T>
void test_operator_overloads() {
	execute_test( [] () { return degrees_to_radians<T>(); }, "degrees_to_radians" );
	execute_test( [] () { return radians_to_degrees<T>(); }, "radians_to_degrees" );
	//TODO angle
	execute_test( [] () { return trigonometry_sin_double_inverse<T>(); }, "trigonometry_sin_double_inverse" );
	execute_test( [] () { return trigonometry_cos_double_inverse<T>(); }, "trigonometry_cos_double_inverse" );
	execute_test( [] () { return trigonometry_tan_double_inverse<T>(); }, "trigonometry_tan_double_inverse" );
	execute_test( [] () { return trigonometry_hyperbolic_sinh_double_inverse<T>(); }, "trigonometry_hyperbolic_sinh_double_inverse" );
	execute_test( [] () { return trigonometry_hyperbolic_cosh_double_inverse<T>(); }, "trigonometry_hyperbolic_cosh_double_inverse" );
	execute_test( [] () { return trigonometry_hyperbolic_tanh_double_inverse<T>(); }, "trigonometry_hyperbolic_tanh_double_inverse" );
	execute_test( [] () { return trigonometry_quotient<T>(); }, "trigonometry_quotient" );
	execute_test( [] () { return trigonometry_inverse_quotient<T>(); }, "trigonometry_inverse_quotient" );
	execute_test( [] () { return trigonometry_sin_cos_pythagorean<T>(); }, "trigonometry_sin_cos_pythagorean" );
	execute_test( [] () { return trigonometry_tan_sec_pythagorean<T>(); }, "trigonometry_tan_sec_pythagorean" );
	execute_test( [] () { return trigonometry_tan_sec_pythagorean<T>(); }, "trigonometry_tan_sec_pythagorean" );
	// TODO csc, sec, cot, acsc, asec, acot
	// TODO all h-variants
}