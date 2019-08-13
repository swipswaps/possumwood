# Looking for boost
find_package(Boost REQUIRED COMPONENTS
	filesystem system
)
include_directories(${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

# pkg-config to find jsoncpp
find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp)

include_directories(${JSONCPP_INCLUDE_DIRS})
set(LIBS ${LIBS} ${JSONCPP_LIBRARIES})

# Building the library
include_directories(./)

file(GLOB_RECURSE sources *.cpp)

add_library(lightfields SHARED ${sources})
install(TARGETS lightfields DESTINATION lib)

target_link_libraries(lightfields ${LIBS})