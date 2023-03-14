# Bombela - 2008
# <bombela@free.fr>

# $Id: CMakeLists.txt 88 2008-11-16 17:00:12Z bombela $

cmake_minimum_required(VERSION 2.4)

PROJECT(MeriseAcide C CXX)

# Configuration
# -----------------------------------------------------------------------------
SET(CPPFILES
	main.cpp
	Utils.cpp
	Options.cpp
	Cardinality.cpp
	CardinalityValue.cpp
	MCDParser.cpp
	MCDParserAttribute.cpp
	MCDParserAttributeContainer.cpp
	MCDParserEntity.cpp
	MCDParserAssociation.cpp
	MCDParserCardinality.cpp
	MCDParserCardinalityValue.cpp
	MCDDatabase.cpp
	MCDAttribute.cpp
	MCDEntity.cpp
	MCDAssociation.cpp
	MPDDatabase.cpp
	MPDTable.cpp
	MPDField.cpp
	MPDLink.cpp
	DateTime.cpp
	SQLPrint.cpp
	SQLPrint_MySQL.cpp
	SQLPrint_SQLite.cpp
	SQLPrint_PostgreSQL.cpp
	Graphviz.cpp
	DataDic.cpp
	DataDic_Plain.cpp
	DataDic_Latex.cpp
	CheckParser.cpp
	)

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
		FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(NOT BUILD_GPROF)
	SET(BUILD_GPROF CACHE BOOL
		"Build with profiler (GProf)"
		FORCE)
ENDIF(NOT BUILD_GPROF)

ADD_DEFINITIONS(-Wall)

# Arch size
# -----------------------------------------------------------------------------
IF    (CMAKE_SIZEOF_VOID_P MATCHES "8")
	# 64 bits
	SET(ARCH_S 64)
ELSE  (CMAKE_SIZEOF_VOID_P MATCHES "8")
	# 32 bits
	SET(ARCH_S 32)
ENDIF (CMAKE_SIZEOF_VOID_P MATCHES "8")
# -----------------------------------------------------------------------------

# Program name
# -----------------------------------------------------------------------------
SET(PRGNAME ${CMAKE_PROJECT_NAME}_${CMAKE_SYSTEM_NAME}${ARCH_S})
# -----------------------------------------------------------------------------

# Libraries
# -----------------------------------------------------------------------------
#CFLAGS="-DENABLE_PANGO_MARKUP" ../graphviz-2.17.20080206.0540/configure --prefix="$PWD"/../install && make -j3 && make install-strip

#SET(MYGRAPHVIZ_PREFIX ${CMAKE_SOURCE_DIR}/libgraphiz_hack/install)

IF(NOT MYGRAPHVIZ_PREFIX)
	SET(MYGRAPHVIZ_PREFIX
		${CMAKE_SOURCE_DIR}/libgraphiz_hack/install_${CMAKE_SYSTEM_NAME}${ARCH_S}
		CACHE STRING
		"The prefix for graphviz base dir."
		FORCE)
ENDIF(NOT MYGRAPHVIZ_PREFIX)

IF(NOT MYGRAPHVIZ_LIB)
	SET(MYGRAPHVIZ_LIB ${MYGRAPHVIZ_PREFIX}/lib CACHE STRING
		"The graphviz lib dir."
		FORCE)
ENDIF(NOT MYGRAPHVIZ_LIB)

IF(WIN32)
	INCLUDE_DIRECTORIES(
		${MYGRAPHVIZ_PREFIX}/include
		)
ELSE(WIN32)
	INCLUDE_DIRECTORIES(
		${MYGRAPHVIZ_PREFIX}/include/graphviz
		)
ENDIF(WIN32)

LINK_DIRECTORIES(
	${MYGRAPHVIZ_LIB}
	${MYGRAPHVIZ_LIB}/graphviz
	)

LINK_LIBRARIES(
	gvc
	graph
	)
# -----------------------------------------------------------------------------

# Target definition
# -----------------------------------------------------------------------------
ADD_EXECUTABLE(${PRGNAME}
	${CPPFILES}
	)

IF(BUILD_GPROF)
	SET_TARGET_PROPERTIES(${PRGNAME} PROPERTIES
		COMPILE_FLAGS "-pg"
		LINK_FLAGS "-pg")
ENDIF(BUILD_GPROF)

IF(CMAKE_BUILD_TYPE MATCHES "Release")
	SET_TARGET_PROPERTIES(${PRGNAME} PROPERTIES
		LINK_FLAGS "-s")
ENDIF(CMAKE_BUILD_TYPE MATCHES "Release")
# -----------------------------------------------------------------------------
