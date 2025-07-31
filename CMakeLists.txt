cmake_minimum_required(VERSION 3.16)
project(stdf_parser VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compile options for debugging and warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Add code coverage support (gcov)
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# Find required packages
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Set output directories (must be before target creation)
# Place binaries in project root bin/ folder, not in build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# Library source files (excluding main applications)
set(LIB_SOURCES
    src/stdf_types.cpp
    src/stdf_parser.cpp
    src/database.cpp
)

file(GLOB_RECURSE HEADERS "include/*.h")

# Create the library
add_library(stdf_lib STATIC ${LIB_SOURCES})
target_link_libraries(stdf_lib SQLite::SQLite3)

# Create the main parser executable
add_executable(stdf_parser src/main.cpp)
target_link_libraries(stdf_parser stdf_lib)

# Create the sample file generator executable
add_executable(stdf_generator src/stdf_generator.cpp)
target_link_libraries(stdf_generator stdf_lib)

# Installation
install(TARGETS stdf_parser stdf_generator DESTINATION bin)
install(TARGETS stdf_lib DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include/stdf)

# Create necessary directories
add_custom_target(create_directories ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/lib
    COMMENT "Creating data, bin, and lib directories"
)

# Google Test (gtest) integration for unit tests
include(CTest)
find_package(GTest REQUIRED)

file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/test/*.cpp")
if(TEST_SOURCES)
    add_executable(stdf_tests ${TEST_SOURCES})
    target_link_libraries(stdf_tests stdf_lib GTest::gtest GTest::gtest_main pthread)
    add_test(NAME stdf_tests COMMAND stdf_tests)
endif()

# Clean targets for comprehensive cleanup
# Note: 'make clean' is automatically provided by CMake and cleans built objects/executables

# Clean all CMake-generated files (keeps source and data)
add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*.cmake
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/lib
    COMMENT "Removing all CMake-generated files and build artifacts"
)

# Clean generated data files (STDF files and databases)
add_custom_target(clean-data
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/data/*.stdf
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/data/*.db
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*.stdf
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/*.db
    COMMENT "Removing generated STDF files and databases"
)

# Complete cleanup - removes everything except source code
add_custom_target(clean-all
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND ${CMAKE_MAKE_PROGRAM} distclean
    COMMAND ${CMAKE_MAKE_PROGRAM} clean-data
    COMMENT "Complete cleanup: removing all generated files, build artifacts, and data"
)

# Help target to show available clean options
add_custom_target(clean-help
    COMMAND ${CMAKE_COMMAND} -E echo "Available clean targets:"
    COMMAND ${CMAKE_COMMAND} -E echo "  make clean       - Remove built objects and executables"
    COMMAND ${CMAKE_COMMAND} -E echo "  make distclean   - Remove all CMake-generated files and build artifacts"
    COMMAND ${CMAKE_COMMAND} -E echo "  make clean-data  - Remove generated STDF files and databases"
    COMMAND ${CMAKE_COMMAND} -E echo "  make clean-all   - Complete cleanup - all of the above"
    COMMAND ${CMAKE_COMMAND} -E echo "  make clean-help  - Show this help message"
    COMMENT "Displaying clean target help"
)

# Code coverage targets
if(ENABLE_COVERAGE)
    # Clean coverage data
    add_custom_target(coverage-clean
        COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete
        COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcno" -delete
        COMMENT "Cleaning coverage data files"
    )
    
    # Generate coverage report
    add_custom_target(coverage-report
        COMMAND ${CMAKE_COMMAND} -E echo "Generating coverage report..."
        COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --output-file coverage.info
        COMMAND lcov --remove coverage.info '/usr/*' --output-file coverage.info
        COMMAND lcov --remove coverage.info '*/test/*' --output-file coverage.info
        COMMAND genhtml coverage.info --output-directory coverage_html
        COMMAND ${CMAKE_COMMAND} -E echo "Coverage report generated in coverage_html/"
        COMMAND ${CMAKE_COMMAND} -E echo "Open coverage_html/index.html in a browser to view"
        DEPENDS stdf_tests
        COMMENT "Generating HTML coverage report"
    )
    
    # Simple text coverage summary
    add_custom_target(coverage-summary
        COMMAND ${CMAKE_COMMAND} -E echo "Coverage Summary:"
        COMMAND gcov -r -o ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/src/*.cpp | grep -E "File|Lines executed"
        DEPENDS stdf_tests
        COMMENT "Generating text coverage summary"
    )
endif()