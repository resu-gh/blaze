find_package (PkgConfig REQUIRED)
pkg_check_modules (OpenSSL REQUIRED openssl)

dirsl (_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
file (GLOB_RECURSE _HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp *.ipp)
file (GLOB_RECURSE _SRCS *.cpp)

add_library (blaze_shared SHARED)
target_sources (blaze_shared PRIVATE ${_SRCS})

set_target_properties (blaze_shared PROPERTIES
    OUTPUT_NAME "blaze")

target_compile_options (blaze_shared PRIVATE
    -Wall -Werror -Wextra -Wpedantic
    $<$<CONFIG:Debug>:
    -O0 -g3>
    $<$<CONFIG:Release>:
    -O3 -march=native -mtune=native>)

target_compile_definitions (blaze_shared PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>)

target_link_options (blaze_shared PRIVATE
    $<$<CONFIG:Debug>:
    -fsanitize=address>
    $<$<CONFIG:Release>:>)

target_include_directories (blaze_shared
    PRIVATE ${_DIRS}
    PUBLIC ${OpenSSL_INCLUDE_DIRS})

target_link_libraries (blaze_shared
    PUBLIC ${OpenSSL_LIBRARIES})

# # export cmake config
install (TARGETS blaze_shared
    # EXPORT blaze_shared
    LIBRARY DESTINATION lib)
# install(EXPORT blaze_shared
#   DESTINATION lib/cmake
#   FILE blazeConfig.cmake)

# copy headers
foreach (H ${_HEADERS})
    get_filename_component (D ${H} DIRECTORY)
    install (FILES ${H} DESTINATION include/${PROJECT_NAME}/${D})
endforeach ()

# pkg-config
configure_file (${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION lib/pkgconfig)
