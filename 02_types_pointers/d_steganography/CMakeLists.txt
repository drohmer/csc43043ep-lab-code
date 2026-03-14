# Generic CMake setup for CGP library use
cmake_minimum_required(VERSION 3.16)

project(project
    DESCRIPTION "Project"
    LANGUAGES CXX
)

# Collect sources automatically
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]pp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/src"
)

# Compile definitions (macros)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    SOLUTION
)

# Compiler standard
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)


# Compiler warnings
if(MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}$<0:>
        VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SRC_FILES})
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /wd4127 /wd4267 /wd4244 /wd4996)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic
        -Wfatal-errors -Wno-pragmas -Wno-unknown-warning-option
        -Wno-sign-compare -Wno-type-limits
    )
endif()

