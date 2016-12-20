# require new behaviour of: CMP0054
cmake_minimum_required(VERSION 3.1)

# Project Name
set(CGRA_PROJECT "base" CACHE STRING "CGRA Project Name")

# Project
project("CGRA_PROJECT_${CGRA_PROJECT}" CXX C)

# Enable IDE Project Folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories("${PROJECT_SOURCE_DIR}/..")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# Full normal warnings
	add_compile_options(/W4)
	# Ignore C4800: forcing X to bool (performance warning)
	add_compile_options(/wd4800)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# C++14
	add_compile_options(-std=c++1y)
	# Full normal warnings
	add_compile_options(-Wall -Wextra -pedantic)
	# Promote missing return to error
	add_compile_options(-Werror=return-type)
	# enable coloured output if gcc >= 4.9
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
		add_compile_options(-fdiagnostics-color)
	endif()
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
	# C++14
	add_compile_options(-std=c++1y)
	# Full normal warnings
	add_compile_options(-Wall -Wextra -pedantic)
	# Promote missing return to error
	add_compile_options(-Werror=return-type)
endif()

# Set source groups to mirror real directory tree
function(target_source_group_tree target)
	get_target_property(sources ${target} SOURCES)
	foreach(source ${sources})
		get_filename_component(source ${source} ABSOLUTE)
		string(REPLACE ${PROJECT_SOURCE_DIR}/ "" rel ${source})
		if(rel)
			string(REGEX REPLACE "/([^/]*)$" "" rel ${rel})
			if(NOT rel STREQUAL source)
				string(REPLACE "/" "\\\\" rel ${rel})
				source_group(${rel} FILES ${source})
			endif()
		endif()
	endforeach()
endfunction()

# Add sources to a target relative to current CMakeLists.txt directory (CMAKE_CURRENT_SOURCE_DIR)
function(target_relative_sources target)
	foreach(source ${ARGN})
		target_sources(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
	endforeach()
endfunction()

add_subdirectory(src) # Primary source files

set_property(TARGET ${CGRA_PROJECT} PROPERTY FOLDER "CGRA")