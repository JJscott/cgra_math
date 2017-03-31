#include <iostream>

#include "math_test.hpp"

using namespace std;
using namespace cgra;

int main() {
	//test::run_tests();

	mat3 i{ vec3(2,1,1), vec3(0,2,0) , vec3(0,0,2) };
	vec3 v(1, 2, 3);

	cout << i << endl;
	cout << v << endl;
	cout << i * v << endl;

	cin.get();

}