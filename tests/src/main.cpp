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

	constexpr vec0 v0;
	constexpr auto v1a = ivec2{3, 4};
	constexpr auto v1b = ivec4(2);
	constexpr auto v1 = vec3(v1a, v1b);
	
	constexpr auto v2a = vec3{1, 2, 3};
	constexpr auto v2b = vec3{4, 5, 6};
	vec2x3 v2c;

	vec2x3 v2{v2a, v2b, v2c}; // {vec3{1, 2, 3}, vec3{4, 5, 6}};

	auto v3 = vec3(vec4(7));
	v3.y = 4;

	vec4 v4{vec0(), vec4(5), vec0(), vec4()};

	constexpr basic_vec<float, 5> v5{1, 2, 3};

	cout << v1 << endl;
	cout << v2 << endl;
	cout << v3 << endl;
	cout << v4 << endl;
	cout << v5 << endl;

	mat3 m1{mat4(pi)};
	mat3 m2{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
	mat3 m3 = m2;

	cout << m1 << endl;
	cout << m2 << endl;

	constexpr quat q1;

	cout << q1 << endl;

	basic_vec<foobar, 2> fb;

	cin.get();
}
