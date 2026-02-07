cmake_minimum_required(VERSION 3.16)
project( 2D_SVG_RENDERER CXX) 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
	nodepp
	GIT_REPOSITORY   https://github.com/NodeppOfficial/nodepp
	GIT_TAG          origin/main
	GIT_PROGRESS     ON
)
FetchContent_MakeAvailable(nodepp)

include(FetchContent)
FetchContent_Declare(
	nodepp-express
	GIT_REPOSITORY   https://github.com/NodeppOfficial/nodepp-express
	GIT_TAG          origin/main
	GIT_PROGRESS     ON
)
FetchContent_MakeAvailable(nodepp-express)

include(FetchContent)
FetchContent_Declare(
	nodepp-raylib
	GIT_REPOSITORY   https://github.com/NodeppOfficial/nodepp-raylib
	GIT_TAG          origin/main
	GIT_PROGRESS     ON
)
FetchContent_MakeAvailable(nodepp-raylib)

include(FetchContent)
FetchContent_Declare(
	nodepp-jwt
	GIT_REPOSITORY   https://github.com/NodeppOfficial/nodepp-jwt
	GIT_TAG          origin/main
	GIT_PROGRESS     ON
)
FetchContent_MakeAvailable(nodepp-jwt)

add_executable( main ./main.cpp )
target_link_libraries( main PUBLIC nodepp nodepp-express nodepp-raylib nodepp-jwt )
