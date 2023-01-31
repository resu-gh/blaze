find_package (PkgConfig REQUIRED)
pkg_check_modules (GTest REQUIRED gtest)
pkg_check_modules (GLog REQUIRED libglog)

dirsl (_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
file (GLOB_RECURSE _SRCS_LEXER lexer*.cpp)
file (GLOB_RECURSE _SRCS_PARSER parser*.cpp)
file (GLOB_RECURSE _SRCS_EVALUATOR evaluator*.cpp)

add_executable (blaze_lexer_test)
target_sources (blaze_lexer_test PRIVATE ${_SRCS_LEXER})

target_compile_options (blaze_lexer_test PRIVATE
    -Wall -Werror -Wextra -Wpedantic
    $<$<CONFIG:Debug>:
    -O0 -g3>
    $<$<CONFIG:Release>:
    -O3 -march=native -mtune=native>)
target_compile_definitions (blaze_lexer_test PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>)
target_link_options (blaze_lexer_test PRIVATE
    $<$<CONFIG:Debug>:
    -fsanitize=address>
    $<$<CONFIG:Release>:>)
target_include_directories (blaze_lexer_test
    PRIVATE ${_DIRS}
    PRIVATE ${GTest_INCLUDE_DIRS}
    PRIVATE ${GLog_INCLUDE_DIRS})
target_link_libraries (blaze_lexer_test
    PRIVATE blaze_shared
    PRIVATE ${GTest_LIBRARIES}
    PRIVATE ${GLog_LIBRARIES})

add_executable (blaze_parser_test)
target_sources (blaze_parser_test PRIVATE ${_SRCS_PARSER})

target_compile_options (blaze_parser_test PRIVATE
    -Wall -Werror -Wextra -Wpedantic
    $<$<CONFIG:Debug>:
    -O0 -g3>
    $<$<CONFIG:Release>:
    -O3 -march=native -mtune=native>)
target_compile_definitions (blaze_parser_test PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>)
target_link_options (blaze_parser_test PRIVATE
    $<$<CONFIG:Debug>:
    -fsanitize=address>
    $<$<CONFIG:Release>:>)
target_include_directories (blaze_parser_test
    PRIVATE ${_DIRS}
    PRIVATE ${GTest_INCLUDE_DIRS}
    PRIVATE ${GLog_INCLUDE_DIRS})
target_link_libraries (blaze_parser_test
    PRIVATE blaze_shared
    PRIVATE ${GTest_LIBRARIES}
    PRIVATE ${GLog_LIBRARIES})

add_executable (blaze_evaluator_test)
target_sources (blaze_evaluator_test PRIVATE ${_SRCS_EVALUATOR})

target_compile_options (blaze_evaluator_test PRIVATE
    -Wall -Werror -Wextra -Wpedantic
    $<$<CONFIG:Debug>:
    -O0 -g3>
    $<$<CONFIG:Release>:
    -O3 -march=native -mtune=native>)
target_compile_definitions (blaze_evaluator_test PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>)
target_link_options (blaze_evaluator_test PRIVATE
    $<$<CONFIG:Debug>:
    -fsanitize=address>
    $<$<CONFIG:Release>:>)
target_include_directories (blaze_evaluator_test
    PRIVATE ${_DIRS}
    PRIVATE ${GTest_INCLUDE_DIRS}
    PRIVATE ${GLog_INCLUDE_DIRS})
target_link_libraries (blaze_evaluator_test
    PRIVATE blaze_shared
    PRIVATE ${GTest_LIBRARIES}
    PRIVATE ${GLog_LIBRARIES})

include (GoogleTest)
gtest_add_tests (TARGET blaze_lexer_test)
gtest_add_tests (TARGET blaze_parser_test)
gtest_add_tests (TARGET blaze_evaluator_test)
