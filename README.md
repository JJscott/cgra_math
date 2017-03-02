# cgra_math.hpp

[cgra_math.hpp](/cgra_math.hpp) is a single header math library containing data structures and functions for applications in computer graphics, computational geometry and physical simulation. It serves as a light-weight alternative to [GLM](http://glm.g-truc.net/) or [Eigen](http://eigen.tuxfamily.org/) and is inspired by the functionality and style of [GLSL](https://www.opengl.org/registry/); designed to be fast, flexible and easy-to-use.


# FAQ

#### Why did you make another math library?

There are a large number of existing libraries for linear algebra in C++ but the majority of them are slow to compile and  complicated to integrate. `cgra_math.hpp` is a single header file that provides a variety of data types and functions that most suitable for applications in Computer Graphics. It can be integrated straight into your source tree without any hassle.

#### Why C++11?

TODO

#### Why are Matrices Column-major?

TODO
Originally this library was developed for use in OpenGL which requires the data to be packed in column major order.
Array subscript operator allows you to access the matrix by column then row (x then y) which is easier for beginners to understand. A single array subscript operator also allows us to return a reinterpret cast of the data as a vector with length equal to the number of rows making it easier to modify.


# Documentation

* [Data Structures](#data-structures)
* [Operator Overloads](#operator-overloads)
* [Angle and Trigonometry Functions](#angle-and-trigonometry-functions)
* [Exponential Functions](#exponential-functions)
* [Common Functions](#common-functions)
* [Geometric Functions](#geometric-functions)
* [Vector Relational Functions](#vector-relational-functions)
* [Matrix Functions](#matrix-functions)
* [Transform Functions](#transform-functions)
* [Quaternion Functions](#quaternion-functions)
* [Higher Order Functions](#higher-order-functions)


## Data Structures

### `basic_vec<T, N>`
TODO constructors
TODO operators
TODO explicit template override

### `basic_mat<T, N>`
Data is stored in column major order
TODO constructors
TODO operators

### `basic_quat<T, N>`
TODO constructors
TODO operators

### Aliases

A number of convenient aliases, which can be brought into scope with a `using` declaration. The typedefs for `float` based vectors and matrices are shown below GLSL naming scheme (default):

|    Type    | `basic_vec<T, N>` | `basic_mat<T, N, N>` | `basic_mat<T, M, N>` | `basic_quat<T>` |
|:----------:|:-----------------:|:--------------------:|:--------------------:|:---------------:|
|   *float*  |       `vecN`      |        `matN`        |       `matMxN`       |      `quat`     |
|  *double*  |      `dvecN`      |        `dmatN`       |       `dmatMxN`      |     `dquat`     |
|    *int*   |      `ivecN`      |        `imatN`       |       `imatMxN`      |                 |
| *unsigned* |      `uvecN`      |                      |                      |                 |
|   *bool*   |      `bvecN`      |                      |                      |                 |

Initial3D naming scheme

|    Type    | `basic_vec<T, N>` | `basic_mat<T, N, N>` | `basic_mat<T, M, N>` | `basic_quat<T>` |
|:----------:|:-----------------:|:--------------------:|:--------------------:|:---------------:|
|   *float*  |      `vecNf`      |        `matNf`       |       `matMxNf`      |     `quatf`     |
|  *double*  |      `vecNd`      |        `matNd`       |       `matMxNd`      |     `quatd`     |
|    *int*   |      `vecNi`      |        `matNi`       |       `matMxNi`      |                 |
| *unsigned* |      `vecNu`      |                      |                      |                 |
|   *bool*   |      `vecNb`      |                      |                      |                 |

HLSL naming scheme (TODO)

|    Type    | `basic_vec<T, N>` | `basic_mat<T, M, N>` |
|:----------:|:-----------------:|:--------------------:|
|   *float*  |      `floatN`     |      `floatMxN`      |
|  *double*  |     `doubleN`     |      `doubleMxN`     |
|    *int*   |       `intN`      |       `intMxN`       |
| *unsigned* |      `uintN`      |                      |
|   *bool*   |      `boolN`      |                      |


## Operator Overloads

TODO

## Angle and Trigonometry Functions


| Function | Description |
|:--:|:--:|
| `T radians(T x)` | Converts degrees to radians, i.e., degrees * pi/180 |
| `T radians(T x)` | Converts radians to degrees, i.e., radians * 180/pi |
| `T angle(vecT v1, vecT v2)` | Returns the angle between 2 vectors in radians |
| `T sin(T)` <br> `vecT sin(vecT)` | The standard trigonometric sine function |
| `T func(T x)` | description |

`S` is a scalar, `VecT` is a `basic_vec<T, N>`, `T` is any type


## Exponential Functions

| Function | Description |
|:--:|:--:|
| `S pow(S x, S y)` <br> `vecT pow(vecT v1, vecT v2)` | Element-wise function for x in v1 and y in v2 <br> Returns x raised to the y power, i.e., x^y <br> Results are undefined if x < 0 <br> Results are undefined if x = 0 and y <= 0 |
| `S exp(S x)` <br> `vecT exp(vecT v)` | Element-wise function for x in v <br> Returns the natural exponentiation of x, i.e., e^x |
| `S log(S x)` <br> `vecT log(vecT v)` | Element-wise function for x in v <br> Returns the natural logarithm of x, i.e., the value y which satisfies the equation x = e^y <br> Results are undefined if x <= 0. |
| `T exp2(T x)` | exp2 for both scalar x or elements in vector x <br> Returns 2 raised to the x power, i.e., 2^x |
| `T log2(T x)` | log2 for both scalar x or elements in vector x <br> Returns the base 2 logarithm of x, i.e., returns the value <br> y which satisfies the equation x=2^y <br> Results are undefined if x <= 0 |
| `S sqrt(S x)` <br> `vecT sqrt(vecT v)` | Element-wise function for x in v <br> Returns sqrt(x) <br> Results are undefined if x < 0 |
| `T inversesqrt(T x)` | inversesqrt for both scalar x or elements in vector x <br> Returns 1/sqrt(x) <br> Results are undefined if x < 0 |


## Common Functions

| Function | Description |
|:--:|:--:|
| `S abs(S x)` <br> `vecT abs(vecT v)` | Element-wise function for x in v <br> Returns x if x >= 0; otherwise it returns –x |
| `S sign(S x)` <br> `vecT sign(vecT v)` | Element-wise function for x in v <br> Returns 1 if x > 0, 0 if x = 0, or –1 if x < 0 |
| `S floor(S x)` <br> `vecT floor(vecT v)` | Element-wise function for x in v <br> Returns a value equal to the nearest integer that is less than or equal to x |
| `S ceil(S x)` <br> `vecT ceil(vecT v)` | Element-wise function for x in v <br> Returns a value equal to the nearest integer to x whose <br> absolute value is not larger than the absolute value of x |
| `T fract(T x)` | fract for both scalar x or elements in vector x <br> Returns x – floor (x) |
| `T mod(T x)` | mod for both scalar x or elements in vector x <br> Modulus. Returns x-m*floor(x/m) |
| `S min(S x, S y)` <br> `vecT min(vecT v1, S y)` <br> `vecT min(vecT v1, vecT v2)` | Element-wise function for x in v1 and y in v2 <br> Returns y if y < x; otherwise it returns x |
| `S max(S x, S y)` <br> `vecT max(vecT v1, S y)` <br> `vecT max(vecT v1, vecT v2)` | Element-wise function for x in v1 and y in v2 <br> Returns y if y > x; otherwise it returns x |
| `T clamp(T a, T minVal, T maxVal)` | clamp for both scalar a, minVal, maxVal or elements in vector a, minVal, maxVal <br> Returns min(max(x, minVal), maxVal) <br> Results are undefined if minVal > maxVal |
| `S mix(S x, S y, S a)` <br> `vecT mix(vecT v1, vecT v2, S a)` <br> `vecT mix(vecT v1, vecT v2, vecT a)` | Element-wise function for x in v1, y in v2 and a in va <br> Returns the linear blend of x and y, i.e., x*(1−a) + y*a |
| `vecT mix(vecT v1, vecT v2, basc_vec<bool, N> va)` | Element-wise function for x in v1, y in v2 and a in va <br> Selects which vector each returned component comes from <br> For a component of a that is false, the corresponding component of x is returned <br> For a component of a that is true, the corresponding component of y is returned. <br> Components of x and y that are not selected are allowed to be invalid floating-point <br> values and will have no effect on the results <br> Thus, this provides different functionality than, for example, <br>    vecT mix(vecT x, vecT y, vecT(a)) <br> where a is a Boolean vector. |
| `S func(S x)` <br> `vecT func(vecT v)` | description |
| `S func(S x)` <br> `vecT func(vecT v)` | description |
| `S func(S x)` <br> `vecT func(vecT v)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |

## Geometric Functions

| Function | Description |
|:--:|:--:|
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |

## Vector Relational Functions

| Function | Description |
|:--:|:--:|
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |

## Matrix Functions

| Function | Description |
|:--:|:--:|
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |

## Transform Functions

| Function | Description |
|:--:|:--:|
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |

## Quaternion Functions

| Function | Description |
|:--:|:--:|
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |

## Higher Order Functions

| Function | Description |
|:--:|:--:|
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |
| `T func(T x)` | description |

# Running the Testing Suite

TODO

# Acknowledgments

TODO