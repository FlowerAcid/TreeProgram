cmake_minimum_required(VERSION 3.22)
project(treeProgram)

add_library(pugixml SHARED
	/usr/local/lib/pugixml/src/pugixml.cpp
)
find_path(HEADER_PATH pugixml.hpp
	HINTS /usr/local/lib/pugixml/src
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(SRC_LIST 
	src/main.cpp 
	src/treeClass.cpp 
	src/treeClass.hpp 
	${HEADER_PATH}/pugixml.hpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} PUBLIC pugixml)
target_include_directories(${PROJECT_NAME} 
	PRIVATE src
	PRIVATE ${HEADER_PATH}
)
