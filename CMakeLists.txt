cmake_minimum_required(VERSION 3.18)
PROJECT(mothbus CXX)

include(FetchContent)

option(USE_GSL "Use GSL data types" OFF)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to be used")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "C++ standard required")

if(USE_GSL)
	FetchContent_Declare(GSL
	GIT_REPOSITORY https://github.com/microsoft/GSL.git
	GIT_TAG v4.0.0
	)
	FetchContent_MakeAvailable(GSL)
	set(SPAN_INCLUDE ${GSL_INCLUDE_DIRS})
	set(SPAN_LIB GSL)
	target_compile_definitions(mothbus PRIVATE USE_GSL)
else()
	FetchContent_Declare(span
	GIT_REPOSITORY https://github.com/tcbrindle/span
	GIT_TAG 836dc6a0efd9849cb194e88e4aa2387436bb079b
	SOURCE_SUBDIR include  # Avoid running CMake, we just need the header
	)
	FetchContent_MakeAvailable(span)
	# set(SPAN_INCLUDE ${span_INCLUDE_DIRS})
	set(SPAN_INCLUDE ${FETCHCONTENT_BASE_DIR}/span-src/include)
	set(SPAN_LIB "")
	# set(SPAN_LIB span)
endif()


if (MSVC)
	# multiprocess build + bigobj linker
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /bigobj")
endif()

file(GLOB_RECURSE PROJ_INCLUDES include/*.h)
file(GLOB_RECURSE PROJ_SRC src/*.cpp)

set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_USE_STATIC_LIBS        ON)
find_package(Boost 1.54 REQUIRED COMPONENTS system)

if (MSVC)
    add_definitions (-DBOOST_ALL_NO_LIB) # disable autolinking
endif()

# set(GSL_INCLUDE_DIRS "C:\\program files\\GSL\\include" CACHE PATH "Path to core guideline library")

include_directories(include ${Boost_INCLUDE_DIRS} ${SPAN_INCLUDE})

add_library(mothbus STATIC ${PROJ_INCLUDES} ${PROJ_SRC})
if(USE_GSL)
	target_include_directories(mothbus INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>)
else()
	target_include_directories(mothbus INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${SPAN_INCLUDE}>
		$<INSTALL_INTERFACE:include>)
endif()
target_link_libraries(mothbus INTERFACE ${Boost_LIBRARIES} ${SPAN_LIB})

enable_testing()
add_subdirectory(test)

option(Build_Examples OFF "Build examples for mothbus")
if (${Build_Examples})
	add_subdirectory(examples)
endif()