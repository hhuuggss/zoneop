cmake_minimum_required(VERSION 3.22)

project(
	zoneop
	VERSION 2.2.0
	DESCRIPTION "zone operations for zone magic"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

set(headers
	src/PCH.h
	src/Papyrus.h
	src/ZoneDataHandler.h
)

set(sources
	src/main.cpp
	src/Papyrus.cpp
	src/ZoneDataHandler.cpp
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	FILES
	${headers}
	${sources}
)

find_package(CommonLibSSE CONFIG REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME}
	AUTHOR hhuuggss
	SOURCES
		${headers}
		${sources}
		${CMAKE_CURRENT_BINARY_DIR}/version.rc
		.clang-format
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
	$<INSTALL_INTERFACE:src>
)

target_precompile_headers(${PROJECT_NAME}
	PRIVATE
	src/PCH.h
)

install(TARGETS ${PROJECT_NAME} DESTINATION "${CMAKE_INSTALL_LIBDIR}")