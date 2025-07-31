cmake_minimum_required(VERSION 3.16)
project(stdf_parser VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compile options for debugging and warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

# Find required packages
find_package(SQLite3 REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

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

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Optional: Add custom target to create data directory
add_custom_target(create_data_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/data
    COMMENT "Creating data directory"
)

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