# CMakeList.txt : CMake project for socketDemo, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("socketDemo" VERSION 0.0.1)


# Add source to this project's executable.
add_executable (socketDemo 
	main.cpp
	src/Client.cpp
	src/Server.cpp
	src/ServerTest.cpp)

# TODO: Add tests and install targets if needed.
target_include_directories( ${PROJECT_NAME}  
	PRIVATE 
		${PROJECT_SOURCE_DIR}/include
)

