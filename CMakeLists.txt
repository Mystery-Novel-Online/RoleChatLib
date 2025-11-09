cmake_minimum_required(VERSION 3.16)
project(rolechat-lib LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PARAM_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE PARAM_HEADERS CONFIGURE_DEPENDS include/*.h)

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3  
)

FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(sqlite3 GIT_REPOSITORY https://github.com/sjinks/sqlite3-cmake GIT_TAG v3.49.1)
FetchContent_MakeAvailable(sqlite3)

add_library(rolechat-lib ${PARAM_SOURCES} ${PARAM_HEADERS})
target_link_libraries(rolechat-lib PUBLIC nlohmann_json::nlohmann_json SQLite::SQLite3)
target_include_directories(rolechat-lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> )

# Test Executable
include(CTest)
add_executable(rolechat-tests tests/json_test.cpp )
target_link_libraries(rolechat-tests PRIVATE rolechat-lib)
target_include_directories(rolechat-tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
