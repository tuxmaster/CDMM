cmake_minimum_required(VERSION 3.24)

project(cdmm LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
message(STATUS "Using Qt ${Qt6Core_VERSION}")

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

set(CPP_HEADER
  src/dmmclass.h
  src/escort97.h
  src/fluke45.h
  src/gdm703.h
  src/idm73.h
  src/iocbuvmonitor.h
  src/logger.h
  src/m9803r.h
  src/metex14bytescontinuous.h
  src/metex14bytespolling.h
  src/metex15bytescontinuous.h
  src/metexascii.h
  src/mutex.h
  src/peaktech10ascii.h
  src/port.h
  src/pt506.h
  src/qm1537.h
  src/serialport.h
  src/serversocket.h
  src/socketexception.h
  src/socketexeption.h
  src/socket.h
  src/telnetconnection.h
  src/telnetserver.h
  src/thread.h
  src/udpserver.h
  src/util.h
  src/vc820.h
  src/vc870.h
  src/vc940.h
  src/vichy12bytepolling.h
  src/vichyascii.h
)
set(CPP_SOURCES
  src/dmmclass.cpp
  src/escort97.cpp
  src/fluke45.cpp
  src/gdm703.cpp
  src/idm73.cpp
  src/iocbuvmonitor.cpp
  src/logger.cpp
  src/m9803r.cpp
  src/main.cpp
  src/metex14bytescontinuous.cpp
  src/metex14bytespolling.cpp
  src/metex15bytescontinuous.cpp
  src/metexascii.cpp
  src/mutex.cpp
  src/peaktech10ascii.cpp
  src/port.cpp
  src/pt506.cpp
  src/qm1537.cpp
  src/serialport.cpp
  src/serversocket.cpp
  src/socket.cpp
  src/telnetconnection.cpp
  src/telnetserver.cpp
  src/thread.cpp
  src/udpserver.cpp
  src/util.cpp
  src/vc820.cpp
  src/vc870.cpp
  src/vc940.cpp
  src/vichy12bytepolling.cpp
  src/vichyascii.cpp
)
if(WIN32)
  set(WIN_HEADER
    src/win_socket.h
  )
  set(WIN_SOURCES
    src/win_socket.cpp
  )
endif()


set(PROJECT_SOURCES
   ${CPP_HEADER}
   ${CPP_SOURCES}
   ${WIN_HEADER}
   ${WIN_SOURCES}
)

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
