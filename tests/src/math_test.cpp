
#include <cmath>
#include <sstream>
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

	// magic ctors (this is where the fun is)
	//v0 = basic_vec<float, 0>(1, v0);
	//v0 = basic_vec<float, 0>(v0, 1, v0);
	//v1 = basic_vec<float, 1>(1, v0);
	//v1 = basic_vec<float, 1>(v0, 1, v0);
	//v2 = basic_vec<float, 2>(1, v1);
	//v2 = basic_vec<float, 2>(v0, 1, v1);
	v3 = basic_vec<float, 3>(1, v2);
	v3 = basic_vec<float, 3>(v1, 1, v2);
	v4 = basic_vec<float, 4>(1, v3);
	v4 = basic_vec<float, 4>(v2, 1, v3);
	v5 = basic_vec<float, 5>(1, v4);
	v5 = basic_vec<float, 5>(v3, 1, v4);



	// array, static and data access operator
	//
	float v = 1;
	v = v1[0];
	v = v1.x;
	v = v2[0] + v2[1];
	v = v2.x + v2.y;
	v = v3[0] + v3[1] + v3[2];
	v = v3.x + v3.y + v3.z;
	v = v4[0] + v4[1] + v4[2] + v4[3];
	v = v4.x + v4.y + v4.z + v4.w;
	v = v5[0] + v5[1] + v5[2] + v5[3] + v5[4];

	v1[0] = v;
	v1.x = v;
	v2[0] = v2[1] = v;
	v2.x = v2.y = v;
	v3[0] = v3[1] = v3[2] = v;
	v3.x = v3.y = v3.z = v;
	v4[0] = v4[1] = v4[2] = v4[3] = v;
	v4.x = v4.y = v4.z = v4.w = v;
	v5[0] = v5[1] = v5[2] = v5[3] = v5[4] = v;

	float *vp;
	vp = v0.data();
	vp = v1.data();
	vp = v2.data();
	vp = v3.data();
	vp = v4.data();
	vp = v5.data();


	// printing out
	//
	ostringstream ss;
	ss << v0 << v1 << v2 << v3 << v4 << v5;

	// iteration (begin end)
	//
	for (float f : v3);
	for (float &f : v3);
	for (const float f : v3);
	for (const float &f : v3);


	// operator overloads
	int iv = 1;
	basic_vec<int, 3> iv3(1);

	v3 += v;
	v3 += v3;

	v3 -= v;
	v3 -= v3;

	v3 *= v;
	v3 *= v3;

	v3 /= v;
	v3 /= v3;

	iv3 %= iv;
	iv3 %= iv3;

	iv3 <<= iv;
	iv3 <<= iv3;

	iv3 >>= iv;
	iv3 >>= iv3;

	iv3 |= iv;
	iv3 |= iv3;

	iv3 ^= iv;
	iv3 ^= iv3;

	iv3 &= iv;
	iv3 &= iv3;

	v3 = -v3;

	v3 =  v3 + v3;
	v3 = iv3 + v3;
	v3 =  v3 + v;
	v3 =   v + v3;

	v3 =  v3 - v3;
	v3 = iv3 - v3;
	v3 =  v3 - v;
	v3 =   v - v3;

	v3 =  v3 * v3;
	v3 = iv3 * v3;
	v3 =  v3 * v;
	v3 =   v * v3;

	v3 =  v3 - v3;
	v3 = iv3 - v3;
	v3 =  v3 - v;
	v3 =   v - v3;

	iv3 = iv3 % iv3;
	iv3 = iv3 % iv;
	iv3 =  iv % iv3;

	iv3 = iv3 << iv3;
	iv3 = iv3 << iv;
	iv3 =  iv << iv3;

	iv3 = iv3 >> iv3;
	iv3 = iv3 >> iv;
	iv3 =  iv >> iv3;

	iv3 = !iv3;

	iv3 = iv3 || iv3;
	iv3 = iv3 || iv;
	iv3 =  iv || iv3;

	iv3 = iv3 && iv3;
	iv3 = iv3 && iv;
	iv3 =  iv && iv3;

	iv3 = ~iv3;

	iv3 = iv3 | iv3;
	iv3 = iv3 | iv;
	iv3 =  iv | iv3;

	iv3 = iv3 ^ iv3;
	iv3 = iv3 ^ iv;
	iv3 =  iv ^ iv3;

	iv3 = iv3 & iv3;
	iv3 = iv3 & iv;
	iv3 =  iv & iv3;

	bool b = v3 == v3;
	b = v3 != v3;
	b = v3 < v3;


	

	// Angle and trig
	//
	v = radians(v);
	v3 = radians(v3);

	v = degrees(v);
	v3 = degrees(v3);

	v = angle(v3, v3);

	v = sin(v);
	v3 = sin(v3);

	v = cos(v);
	v3 = cos(v3);

	v = tan(v);
	v3 = tan(v3);

	v = asin(v);
	v3 = asin(v3);

	v = acos(v);
	v3 = acos(v3);

	v = atan(v);
	v3 = atan(v3);

	v = atan(v, v);
	v3 = atan(v3, v3);

	v = sinh(v);
	v3 = sinh(v3);

	v = cosh(v);
	v3 = cosh(v3);

	v = tanh(v);
	v3 = tanh(v3);

	v = asinh(v);
	v3 = asinh(v3);

	v = acosh(v);
	v3 = acosh(v3);

	v = atanh(v);
	v3 = atanh(v3);

	v = csc(v);
	v3 = csc(v3);

	v = sec(v);
	v3 = sec(v3);

	v = cot(v);
	v3 = cot(v3);

	v = acsc(v);
	v3 = acsc(v3);

	v = asec(v);
	v3 = asec(v3);

	v = acot(v);
	v3 = acot(v3);

	v = csch(v);
	v3 = csch(v3);

	v = sech(v);
	v3 = sech(v3);

	v = coth(v);
	v3 = coth(v3);

	v = acsch(v);
	v3 = acsch(v3);

	v = asech(v);
	v3 = asech(v3);

	v = acoth(v);
	v3 = acoth(v3);


	// exponential functions
	//
	v = pow(v, v);
	v3 = pow(v3, v3);

	v = exp(v);
	v3 = exp(v3);

	v = log(v);
	v3 = log(v3);

	v = exp2(v);
	v3 = exp2(v3);

	v = log2(v);
	v3 = log2(v3);

	v = sqrt(v);
	v3 = sqrt(v3);

	v = sqrt(v);
	v3 = sqrt(v3);


	// Common functions
	//

	// TODO







}


void random_test() {



	//mat4 m = rotate3y<mat4>(1.0);

	//cout << m << endl << endl;
	//cout << inverse(m) << endl << endl;
	//cout << inverse(inverse(m)) << endl << endl;
	//cout << inverse(m) * m << endl << endl;

	dvec4 v{ 0,0,0,1 };
	quat q0( v );
	dquat q1(q0);

	quat q = q0 * q1;
	q = conjugate(q);
	q = inverse(q);
	
	
	auto qw = q * 5;

	float d = dot(q, q);
	float ilen2 = 1 / d;
	q = conjugate(q) * ilen2;


	cout << q << endl << endl;

}


//Main program
// 
void test::run_tests() {
	//vector_compile_test();
	random_test();
	cin.get();
}