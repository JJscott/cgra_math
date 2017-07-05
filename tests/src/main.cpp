#include <iostream>

#include "math_test.hpp"

using namespace std;
using namespace cgra;

int main() {
	//test::run_basic_vec_tests();

	using vec2x3 = basic_vec<basic_vec<float, 3>, 2>;

	vec0 v0;
	auto v1 = vec3(ivec2(3), ivec4(2));
	vec2x3 v2{{1, 2, 3}, {4, 5, 6}};

	auto v3 = vec3(vec4(7));
	v3.y = 4;

	vec4 v4{vec0(), vec4(5), vec0(), vec4()};
	basic_vec<float, 5> v5;

	cout << v1 << endl;
	cout << v2 << endl;
	cout << v3 << endl;
	cout << v4 << endl;

	mat3 m1{mat4(pi)};
	mat3 m2{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

	cout << m1 << endl;
	cout << m2 << endl;

	quat q1;

	cout << q1 << endl;

	cin.get();
}
