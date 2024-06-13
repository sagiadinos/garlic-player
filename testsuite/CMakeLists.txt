cmake_minimum_required(VERSION 3.5)
project(GarlicLibTests LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

# add QTest and gmock
find_package(Qt5Test REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt5 COMPONENTS Xml XmlPatterns REQUIRED)

# Automoc aktivieren
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

enable_testing()

include_directories(/home/niko/Source/github/garlic-player/src/garlic-lib/)
include_directories(/home/niko/Source/github/garlic-player/src/garlic-lib/smilparser)
include_directories(/home/niko/Source/github/garlic-player/src/garlic-lib/tools)
include_directories(/home/niko/Source/github/garlic-player/src/garlic-lib/tools/reboot)
link_directories(/home/niko/Source/github/garlic-player/src/lib/)

include_directories(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE TEST_SOURCES garlic-lib/*.cpp)
file(GLOB_RECURSE TEST_HEADERS garlic-lib/*.hpp)
add_executable(runTests ${TEST_SOURCES} ${TEST_HEADERS} main.cpp base_testsuite.cpp base_testsuite.hpp)

target_link_libraries(runTests Qt5::Test GTest::GTest GTest::Main gmock gmock_main libgarlic.so Qt5::Xml Qt5::XmlPatterns)

# register tests
add_test(NAME runTests COMMAND runTests)
