# require new behaviour of: CMP0054
cmake_minimum_required(VERSION 3.1)

# Project
project("CGRA_MATH" CXX C)

# Enable IDE Project Folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories("${PROJECT_SOURCE_DIR}/..")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# C++ latest
	add_compile_options(/std:c++latest)
	# UTF-8 source and execution charsets
	add_compile_options(/utf-8)
	# Full normal warnings, multithreaded build
	add_compile_options(/W4 /MP)
	# Disable C4800: forcing X to bool (performance warning)
	add_compile_options(/wd4800)
	# Not debug: enable intrinsics
	add_compile_options("$<$<NOT:$<CONFIG:Debug>>:/Oi>")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# C++17, full normal warnings
	add_compile_options(-std=c++1z -Wall -Wextra -pedantic)
	# Threading support
	add_compile_options(-pthread)
	# Promote missing return to error
	add_compile_options(-Werror=return-type)
	# enable coloured output if gcc >= 4.9
	# TODO remove this, use the GCC_COLORS env instead?
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
		add_compile_options(-fdiagnostics-color)
	endif()
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
	# C++17, full normal warnings
	add_compile_options(-std=c++1z -Wall -Wextra -pedantic)
	# Threading support
	add_compile_options(-pthread)
	# Promote missing return to error
	add_compile_options(-Werror=return-type)
endif()

add_subdirectory(src)

set_property(TARGET cgra_math_test PROPERTY FOLDER "CGRA")




