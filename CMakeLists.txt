cmake_minimum_required(VERSION 3.16)

project(coursework
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(COURSEWORK_ENABLE_TESTS "Build tests" ON)

# ============================================================
# ML library
# ============================================================

set(ML_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(ML_ENABLE_SANITIZERS OFF CACHE BOOL "" FORCE)
set(ML_ENABLE_COVERAGE OFF CACHE BOOL "" FORCE)
set(ML_ENABLE_CLANG_TIDY OFF CACHE BOOL "" FORCE)

add_subdirectory(
    ../ml
    ${CMAKE_BINARY_DIR}/ml
)

# ============================================================
# Coursework core
# ============================================================

add_library(coursework_core STATIC
    src/model_manager.cpp
    src/inference_engine.cpp
    src/prediction_service.cpp
    src/request_handler.cpp
)

target_include_directories(coursework_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(coursework_core
    PUBLIC
        ml
)

target_compile_features(coursework_core
    PUBLIC
        cxx_std_17
)

# ============================================================
# Main executable
# ============================================================

add_executable(coursework
    src/main.cpp
)

target_link_libraries(coursework
    PRIVATE
        coursework_core
)

# ============================================================
# Tests
# ============================================================

if(COURSEWORK_ENABLE_TESTS)
    include(CTest)

    if(BUILD_TESTING)
        include(FetchContent)

        FetchContent_Declare(
            googletest
            URL
                https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )

        set(
            gtest_force_shared_crt
            ON
            CACHE BOOL ""
            FORCE
        )

        FetchContent_MakeAvailable(googletest)

        file(
            GLOB_RECURSE TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
        )

        if(TEST_SOURCES)
            add_executable(coursework_tests
                ${TEST_SOURCES}
            )

            target_link_libraries(coursework_tests
                PRIVATE
                    coursework_core
                    GTest::gtest_main
            )

            include(GoogleTest)
            gtest_discover_tests(coursework_tests)
        endif()
    endif()
endif()

# ============================================================
# clang-format
# ============================================================

find_program(CLANG_FORMAT_EXE NAMES clang-format)

if(CLANG_FORMAT_EXE)

    file(
        GLOB_RECURSE ALL_SOURCE_FILES
        CONFIGURE_DEPENDS

        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"

        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.tpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.h"
    )

    if(ALL_SOURCE_FILES)

        add_custom_target(coursework-format
            COMMAND
                ${CLANG_FORMAT_EXE}
                -style=file
                -i
                ${ALL_SOURCE_FILES}

            COMMENT
                "Formatting coursework source files"

            WORKING_DIRECTORY
                ${CMAKE_CURRENT_SOURCE_DIR}
        )

        add_custom_target(coursework-format-check
            COMMAND
                ${CLANG_FORMAT_EXE}
                -style=file
                --dry-run
                --Werror
                ${ALL_SOURCE_FILES}

            COMMENT
                "Checking coursework source formatting"

            WORKING_DIRECTORY
                ${CMAKE_CURRENT_SOURCE_DIR}
        )

    endif()

else()

    message(
        STATUS
        "clang-format not found: coursework format targets disabled"
    )

endif()