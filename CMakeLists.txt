cmake_minimum_required(VERSION 3.24)

project(cdmm LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia
)

if(DOXYGEN_FOUND)
	set(DOXYGEN_GENERATE_HTML YES)
	set(DOXYGEN_QT_AUTOBRIEF YES)
	set(DOXYGEN_EXTRACT_PRIVATE YES)
	set(DOXYGEN_SHOW_FILES NO)
	set(DOXYGEN_SHOW_INCLUDE_FILES NO)
	set(DOXYGEN_VERBATIM_HEADERS NO)
	set(DOXYGEN_HTML_DYNAMIC_SECTIONS YES)
	set(DOXYGEN_DOT_IMAGE_FORMAT svg)
	set(DOXYGEN_INTERACTIVE_SVG YES)
	doxygen_add_docs(doxygen
		WORKING_DIRECTORY ../src
		COMMENT "Build documentation"
	)
endif()

add_executable(cdmm
  
)
target_link_libraries(cdmm Qt${QT_VERSION_MAJOR}::Core)