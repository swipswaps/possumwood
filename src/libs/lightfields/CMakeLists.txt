# Looking for boost
find_package(Boost REQUIRED COMPONENTS
	filesystem system
)
include_directories(${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

# Find opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(LIBS ${LIBS} ${OpenCV_LIBS})

# eigen
find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

# tbb
pkg_search_module(TBB REQUIRED tbb)
include_directories(${TBB_INCLUDE_DIRS})
set(LIBS ${LIBS} ${TBB_LIBRARIES})

# Building the library
include_directories(./)

file(GLOB_RECURSE sources *.cpp)

add_library(lightfields SHARED ${sources})
install(TARGETS lightfields DESTINATION lib)

target_link_libraries(lightfields ${LIBS})
