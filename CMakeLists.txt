cmake_minimum_required(VERSION 3.22)
project(treeProgram)

find_library(LIB_PATH pugixml
	HINTS /home/nikubuntu/program/tree/libs)
find_path(HEADER_PATH pugixml.hpp
	HINTS /usr/lib/pugixml/src)

set(pugixml_USE_STATIC_LIBS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(SRC_LIST src/main.cpp src/treeClass.cpp src/treeClass.hpp ${HEADER_PATH}/pugixml.hpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_PATH})
target_include_directories(${PROJECT_NAME} 
	PRIVATE src
	PRIVATE ${HEADER_PATH})
