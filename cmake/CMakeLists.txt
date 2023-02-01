cmake_minimum_required (
    VERSION 3.24)

project (blaze
    VERSION 0.1.0
    LANGUAGES CXX)

if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    option (_BLAZE_CPPCHECK "" OFF)
    find_program (_BLAZE_X_CPPCHECK "cppcheck")
    if (_BLAZE_X_CPPCHECK AND _BLAZE_CPPCHECK)
        set (CMAKE_CXX_CPPCHECK cppcheck
            --suppress=missingInclude
            --std=c++17
            --enable=all
            --verbose
            --quiet)
    endif ()

    option (_BLAZE_CLANG_TIDY "" OFF)
    find_program (_BLAZE_X_CLANG_TIDY "clang-tidy")
    if (_BLAZE_X_CLANG_TIDY AND _BLAZE_CLANG_TIDY)
        set (CMAKE_CXX_CLANG_TIDY clang-tidy
            -p ${CMAKE_BINARY_DIR})
    endif ()
endif ()

find_program (_BLAZE_X_CLANG_FORMAT "clang-format")
if (_BLAZE_X_CLANG_FORMAT)
    file (GLOB_RECURSE _ALL_HPP src/*.hpp src/*.ipp)
    file (GLOB_RECURSE _ALL_CPP src/*.cpp)
    add_custom_target (format
        COMMAND clang-format
        -i --verbose
        ${_ALL_HPP}
        ${_ALL_CPP})
endif ()

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

option (_BLAZE_CLI "" OFF)
option (_BLAZE_TEST "" OFF)

macro (dirsl r cd)
  file (GLOB cs ${cd}/*)
  set (l ${cd})
  foreach (c ${cs})
    if (IS_DIRECTORY ${c})
      list (APPEND l ${c})
    endif ()
  endforeach ()
  set (${r} ${l})
endmacro ()

add_subdirectory (src/blaze)

if (_BLAZE_CLI)
    add_subdirectory (src/cli)
endif ()

if (_BLAZE_TEST)
    enable_testing ()
    add_subdirectory (src/test)
endif ()
