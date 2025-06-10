cmake_minimum_required(VERSION 3.10)
project(CampusManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Crow and JSON libraries
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/nlohmann)

# Add source files
set(SOURCES
    src/main.cpp
)

# Create executable
add_executable(cms ${SOURCES})

# Link libraries
target_link_libraries(cms PRIVATE pthread) 