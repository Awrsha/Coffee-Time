cmake_minimum_required(VERSION 3.20.1)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(main-tic)

# Define source files
set(NAME_SRC
    src/Main.cpp  
    src/start.cpp  
)

# Define header files
set(NAME_HEADERS        
    include/Ai_module.h  
    include/Main.h  
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Create executable
add_executable(main-tic ${NAME_SRC} ${NAME_HEADERS})
