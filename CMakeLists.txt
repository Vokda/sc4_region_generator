cmake_minimum_required (VERSION 3.0)
project(sc4_region_gen)

set(CMAKE_MODULE_PATH $ENV{HOME}/Projects/cmake)

find_package(SDL2)
include_directories(${SDL2_INCLUDE_DIR})

#simply print the directories found
#message(STATUS "SDL2 include directory ${SDL2_INCLUDE_DIR}")
#message(STATUS "SDL2 library include directory ${SDL2_LIBRARY}")

#find_package(PNG REQUIRED)
#find_package(png++ REQUIRED)
#include_directories(${PNG_INCLUDE_DIRS})


#aux_source_directory("../" SOURCE_FILES)
#source files
file(GLOB SOURCE_FILES "*.hpp" "*.cpp")
message(STATUS "source files: ${SOURCE_FILES}")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_EXE_LINKER_FLAGS "") 
