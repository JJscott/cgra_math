
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <typeinfo>

#include <cgra_math.hpp>
#include "math_test.hpp"


using namespace std;
using namespace cgra;
using namespace test;


int total = 10000;





void vector_compile_test() {
	
	// constructors
	//


	// default
	basic_vec<float, 0> v0;
	basic_vec<float, 1> v1;
	basic_vec<float, 2> v2;
	basic_vec<float, 3> v3;
	basic_vec<float, 4> v4;
	basic_vec<float, 5> v5;
	v0 = basic_vec<float, 0>();
	v1 = basic_vec<float, 1>();
	v2 = basic_vec<float, 2>();
	v3 = basic_vec<float, 3>();
	v4 = basic_vec<float, 4>();
	v5 = basic_vec<float, 5>();

	// exact scalar args
	v1 = basic_vec<float, 1>(1);
	v2 = basic_vec<float, 2>(1, 2);
	v3 = basic_vec<float, 3>(1, 2, 3);
	v4 = basic_vec<float, 4>(1, 2, 3, 4);
	v5 = basic_vec<float, 5>(1, 2, 3, 4, 5);


	// 1 arg scalar broadcast args
	v0 = basic_vec<float, 0>(1);
	v1 = basic_vec<float, 1>(1);
	v2 = basic_vec<float, 2>(1);
	v3 = basic_vec<float, 3>(1);
	v4 = basic_vec<float, 4>(1);
	v5 = basic_vec<float, 5>(1);

	// 1 arg vector args
	v0 = basic_vec<float, 0>(basic_vec<int, 0>());
	v1 = basic_vec<float, 1>(basic_vec<int, 1>());
	v2 = basic_vec<float, 2>(basic_vec<int, 2>());
	v3 = basic_vec<float, 3>(basic_vec<int, 3>());
	v4 = basic_vec<float, 4>(basic_vec<int, 4>());
	v5 = basic_vec<float, 5>(basic_vec<int, 5>());

}



//Main program
// 
void test::run_tests() {
	vector_compile_test();
	//vector_tests();
	cin.get();
}