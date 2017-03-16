		// Common Functions
		// 
		test_true(almost_equal(abs(a), abs(-a)),	"abs  (absolute and negation)");
		test_true(almost_equal(sign(zero), zero),	"sign (sign of zero is zero)");
		test_true(almost_equal(sign(a)*abs(a), a),	"sign (sign multiplied by absolute)");

		test_true(all(lessThanEqual(floor(a), a)),		"floor / all / lessThanEqual (less than equal floor)");
		test_true(almost_equal(fract(floor(a)), zero),	"floor / fract (zero fractional component)");
		test_true(all(greaterThanEqual(ceil(a), a)),	"ceil / all / greaterThanEqual (greater than equal floor)");
		test_true(almost_equal(fract(ceil(a)), zero),	"ceil / fract (zero fractional component)");

		// TODO trunc round roundEven

		test_true(almost_equal(mod(pos+one, so), fract(pos)),	"mod scalar / fract (mod 1 fractional part)");
		test_true(almost_equal(mod(pos+one, one),  fract(pos)),	"mod vector / fract (mod 1 fractional part)");
		// TODO modf

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

		// TODO step, smoothstep, isnan, isinf
		// TODO floatBitsToInt floatBitsToUint intBitsToFloat uintBitsToFloat
		// TODO fma frexp ldexp
		// TODO sum product
