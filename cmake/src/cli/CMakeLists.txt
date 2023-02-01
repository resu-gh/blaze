dirsl (_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
file (GLOB_RECURSE _SRCS *.cpp)

add_executable (blaze_cli)
target_sources (blaze_cli PRIVATE ${_SRCS})

set_target_properties (blaze_cli PROPERTIES
    OUTPUT_NAME "blaze")

target_compile_options (blaze_cli PRIVATE
    -Wall -Werror -Wextra -Wpedantic
    $<$<CONFIG:Debug>:
    -O0 -g3>
    $<$<CONFIG:Release>:
    -O3 -march=native -mtune=native>)

target_compile_definitions (blaze_cli PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>)

target_link_options (blaze_cli PRIVATE
    $<$<CONFIG:Debug>:
    -fsanitize=address>
    $<$<CONFIG:Release>:>)

target_include_directories (blaze_cli
    PRIVATE ${_DIRS})

target_link_libraries (blaze_cli
    PRIVATE blaze_shared)

install (TARGETS blaze_cli
    RUNTIME DESTINATION bin)
