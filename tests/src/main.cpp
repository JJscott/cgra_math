#include <iostream>

#include "math_test.hpp"

using namespace std;
using namespace cgra;

int main() {
	//test::run_basic_vec_tests();

	using vec2x3 = basic_vec<basic_vec<float, 3>, 2>;

	auto v1 = vec3(ivec2(3), ivec4(2));
	vec2x3 v2{{1, 2, 3}, {4, 5, 6}};

	v1 = vec3(vec4(3));

	cout << v1 << endl;
	cout << v2 << endl;

	cin.get();
}
