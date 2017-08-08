#include <iostream>

//#define CGRA_NO_ANONYMOUS_STRUCT

#include "math_test.hpp"

using namespace std;
using namespace cgra;

//#define constexpr

class foobar {
public:
	foobar() {
		cout << "hello foobar" << endl;
	}

	~foobar() {
		cout << "goodbye foobar" << endl;
	}
};

int main() {
	//test::run_basic_vec_tests();
	
	using vec2x3 = basic_vec<basic_vec<float, 3>, 2>;

	vec0 v0;
	auto v1a = ivec2{3, 4};
	auto v1b = ivec4(2);
	auto v1 = vec3(v1a, v1b);
	
	auto v2a = vec3{1, 2, 3};
	auto v2b = vec3{4, 5, 6};
	vec2x3 v2c;

	vec2x3 v2{v2a, v2b, v2c}; // {vec3{1, 2, 3}, vec3{4, 5, 6}};

	auto v3 = vec3(vec4(7));
	v3.y = 4;
	v3 = {8, 5, 8};
	auto v3a = refract(v3, v3, 7);
	auto v3b = vec_cast<4>(vec3());
	auto v3c = vec_cast<bool>(vec3());

	vec4 v4{vec0(), vec4(5), vec0(), vec4()};

	basic_vec<float, 5> v5{1, 2, 3};

	cout << v1 << endl;
	cout << v2 << endl;
	cout << v3 << endl;
	cout << v4 << endl;
	cout << v5 << endl;
	cout << outer_product(vec3(1, 2, 3), vec3(4, 5, 6)) << endl;

	mat3 m1{mat4(pi)};
	mat3 m2{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	mat3 m3 = m2;
	auto m3a = mat_cast<4, 4>(m3);

	auto m4 = zip_with<type_to_mat>(detail::op::add(), m2, m3);

	auto m5 = m3 * m4 + m1;
	auto m6 = 1 / m5;

	auto v6 = m6 * v3;
	auto v7 = v3 * transpose(m6);

	cout << v6 << endl;
	cout << v7 << endl;

	cout << m1 << endl;
	cout << m2 << endl;
	cout << m3 << endl;
	cout << m4 << endl;
	cout << m5 << endl;
	cout << m6 << endl;
	cout << (m5 * m6) << endl;

	auto m7 = scale2(ivec2{1, 2});
	auto m8 = look_at(vec3(1, 2, 3), vec3(4, 5, 6), vec3(7, 8, 9));
	cout << (m8 * vec4(1, 2, 3, 1)) << endl;

	quat q1{vec4(v5)};

	cout << q1 << endl;
	cout << q1.w << endl;
	cout << (axisangle(vec3(0, 0, 1), pi / 2) * vec3(1, 0, 0)) << endl;

	basic_vec<foobar, 2> fb;

	cin.get();
}
